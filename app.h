#ifndef APP_H
#define APP_H
#include "employee.h"
class my_application
{
	enum {length = 255};
#if 1	
	enum {lines_to_fill = 1000000};
#else
	enum {lines_to_fill = 1000};
#endif	
	enum {sz = 100};
	enum {num_of_names = 20};
	char buffer[length];
	char database[length];
	const char* default_database = "dbname=postgres user=postgres password=db1996 host=localhost port = 5432";
	const char* last_names[sz / 10] = {"Fedaev", "Fesenko", "Frolov", "Fomenko", "Fedosov", "Fetisov", "Figurnov", "Fedyuk", "Felimonov"};
	const char* last_namess[num_of_names] = {"Ivanov", "Smirnov", "Popov", "Sokolov", "Petrov", "Lebedev", "Morozov", "Sidorov", "Kovalev", "Rybalov", "Ivanova", "Smirnova", "Popova", "Sokolova", "Petrova", "Lebedeva", "Morozova", "Sidorova",
		"Kovaleva", "Rybalova"};
	const char* fnames[num_of_names] = {"Maxim", "Aleksandr", "Lev", "Mikhail", "Artyom", "Mark", "Ivan",
			"Matvey", "Daniil", "Dmitriy", "Olga", "Krysina", "Elena", "Masha", "Julia", "Dasha",
			"Alina", "Irina", "Alena", "Vera"};
	const char* pnames[num_of_names / 2] = {"Vladimirovich", "Mikhailovich", "Dmitrievich", "Nikolaevich", "Viktorovich",
			"Ivanovna", "Nikolaevna", "Mikhailovna", "Nikolaevna", "Viktorovna"};
	const char* date[sz / 10] = {"2009-11-11", "1996-01-09", "1997-05-10", "1947-03-03", "1961-03-11", "1980-01-01",
			"2000-01-11", "2001-01-03", "1990-02-05", "1979-03-04"};
	void generate_string(char* buffer, const char* string1, ...);
	const char* create_table_sql_query();
	void check_database_exsistence(pqxx::connection& c, const char* db_name);
	void print_database(pqxx::work& tr);
	void store_object(employee** e, int index, pqxx::work& tr);
	void automate_db_fill(employee** a_employee, pqxx::work& tr);
	void clean_up(employee** a_employee);
	void run_query(pqxx::work& tr);
public:
	my_application(const char* database_name);
	void run_app(int mode_number, const char* arg1, const char* arg2, const char* arg3);
};
#endif
