#include "app.h"

void my_application::generate_string(char* buffer, const char* string1, ...)
{
	size_t len = 0;
	const char* p;
	va_list vl;
	p = string1;
	va_start(vl, string1);
	for (; p; p = va_arg(vl, const char*))
	{
		strcpy(buffer + len, p);
		len = strlen(buffer);
	}	
	len = strlen(buffer);
	*(buffer + len) = 0;
	va_end(vl);
}

const char* my_application::create_table_sql_query()
{
        const char* sql_query = "CREATE TABLE IF NOT EXISTS employee ("
                                       "emp_id SERIAL PRIMARY KEY,"
                                       "first_name VARCHAR(80) NOT NULL,"
                                       "paternal_name VARCHAR(80) NOT NULL,"
                                       "last_name VARCHAR(80) NOT NULL,"
                                       "birth_date DATE NOT NULL,"				
                                       "sex VARCHAR(80) NOT NULL"
                                       ");";
	return sql_query;
}

void my_application::check_database_exsistence(pqxx::connection& c, const char* db_name)
{
	try	
	{	
		pqxx::nontransaction ntr(c);
		pqxx::result r = ntr.exec("SELECT datname FROM pg_database;");
		for (const auto& rr : r)
		{
			if (0 == strcmp(rr["datname"].as<const char*>(), db_name))
				return;
		}
		generate_string(buffer, "CREATE", " DATABASE", " ", db_name, " ENCODING", " UTF8", " TEMPLATE", " template0;", NULL);
                r = ntr.exec(buffer);
	}
	catch (const pqxx::sql_error& e)
        {
                fprintf(stderr, "SQL querry error: pqxx::result r = ntr.exec(buffer) %s\n", e.what());
                exit(3);
        }
}

void my_application::print_database(pqxx::work& tr)
{
	employee e;
	char age_buf[3];
	try
	{
		pqxx::result r = tr.exec("SELECT first_name, paternal_name, last_name, birth_date, sex FROM employee ORDER BY first_name ASC, paternal_name ASC, last_name ASC;");
       		for (const auto row : r)
		{	
			e.get_employee_age(row["birth_date"].as<const char*>(), age_buf);
                	printf("first_name - %s\n paternal_name - %s\n last_name - %s\n birth_date - %s\n sex - %s\n age - %s\n\n", row["first_name"].as<const char*>(), row["paternal_name"].as<const char*>(), row["last_name"].as<const char*>(), row["birth_date"].as<const char*>(), row["sex"].as<const char*>(), age_buf);
			*age_buf = 0;		
		}
		tr.commit();
	}
        catch (const pqxx::sql_error& e)
        {
                fprintf(stderr, "SQL querry error: void my_application::print_database() %s\n", e.what());
                exit(4);
        }	
}

void my_application::store_object(employee** ea, int index, pqxx::work& tr)
{
	if (index < sz)
	{
		ea[index] = new employee(fnames[rand() % ((num_of_names - 10) - 1)], pnames[rand() % 4], last_names[rand() % ((num_of_names - 10) - 1)], date[rand() % ((sz / 10) - 1)], "Male");
       		if (!ea[index])
        	{
			fprintf(stderr, "my_application::automate_db_fill(employee**) failed to create an object\n");
                	exit(5);
        	}
        	ea[index]->send_to_database(tr);
		return;
	}
        ea[index] = new employee(index % 2 == 0 ? fnames[rand() % ((num_of_names - 10) - 1)] : fnames[rand() % ((num_of_names - 10) + 10)], index % 2 == 0 ? pnames[rand() % 4] : pnames[(rand() % 5) + 5], index % 2 == 0 ? last_names[rand() % ((num_of_names / 10) - 1)] : last_names[(rand() % 10) + 10], date[rand() % ((sz / 10) - 1)], index % 2 == 0 ? "Male" : "Female");
        if (!ea[index])
        {
		fprintf(stderr, "my_application::automate_db_fill(employee**) failed to create an object new employee();\n");
                exit(6);
        }
        ea[index]->send_to_database(tr);
}

void my_application::automate_db_fill(employee** a_employee, pqxx::work& tr)
{
	int i;
	srand(sz / 10);
	for (i = 0; i < lines_to_fill; i++)
		store_object(a_employee, i, tr);		
}

void my_application::clean_up(employee** a_employee)
{
	int i;
	for (i = 0; i < lines_to_fill; i++)
		delete a_employee[i];
	free(a_employee);
}

void my_application::run_query(pqxx::work& tr)
{
	clock_t start, end;
        try
        {
		start = clock();
                pqxx::result r = tr.exec("SELECT last_name, sex FROM employee WHERE last_name LIKE ('F%') and sex LIKE 'Male%';");
                tr.commit();
		end = clock();
        }
        catch (const pqxx::sql_error& e)
        {
                fprintf(stderr, "SQL querry error: void my_application::run_query(pqxx::work& tr). Failed to select from a table by criteria %s\n", e.what());
                exit(4);
        }
	printf("time passed to run a query to select last name that starts with F and sex Male - %.5f\n", double(end - start) / CLOCKS_PER_SEC);
}

my_application::my_application(const char* database_name)
{
	try
	{
		pqxx::connection c(default_database);
		check_database_exsistence(c, database_name);
		*buffer = 0;
	}
	catch (const pqxx::broken_connection& e)
	{
		fprintf(stderr, "Connection Error: pqxx::connection c(default_database) %s\n", e.what());
		exit(5);
	}
	catch (const pqxx::sql_error& e)
	{
		fprintf(stderr, "SQL querry error: pqxx::result r = ntr.exec(buffer) %s\n", e.what());
		exit(6);
	}
	generate_string(database, "dbname=", database_name, " user=postgres password=db1996 host=localhost port = 5432", NULL);
}

void my_application::run_app(int mode_number, const char* arg1, const char* arg2, const char* arg3)	
{
	employee** a_employee;
	try
	{
		pqxx::connection c(database);
		pqxx::work tr(c);
		if (mode_number == 1)
		{	
			pqxx::result r = tr.exec(create_table_sql_query());
			tr.commit();
			return;
		}
		if (mode_number == 2)
		{	
			employee e(arg1, arg2, arg3);
			e.send_to_database(tr);	
			return;
		}		
		if (mode_number == 3)
		{
				print_database(tr);
				return;
		}
		if (mode_number == 4)
		{
			a_employee = (employee**)malloc(sizeof(*a_employee) * lines_to_fill);
			if (!a_employee)
			{
				perror("ERROR: void my_application::run_app failed to create an array of objects\n");
				exit(7);
			}
			automate_db_fill(a_employee, tr);
			clean_up(a_employee);
			return;
		}
		if (mode_number == 5)
		{
			run_query(tr);
			return;
		}
	}
	catch (const pqxx::broken_connection& e)
	{
                fprintf(stderr, "Connection Error: In function void my_application::run_app(const char* arg)->pqxx::connection c(database) %s\n", e.what());
                exit(4);
	}
	catch (const pqxx::sql_error& e)
	{
                fprintf(stderr, "SQL querry error: In function failed to create a table void my_application::run_app(const char* arg)->pqxx::result r = tr.exec() %s\n", e.what());
                exit(5);		
	}
}


