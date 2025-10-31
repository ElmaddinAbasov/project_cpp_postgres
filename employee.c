#include "employee.h"
void employee::extract(const char* birth, int& arg, int& pos)
{
        while (*(birth + pos) != '-' && *(birth + pos))
        {
                arg = arg * 10 + (*(birth + pos) - '0');
                pos++;
        }
}

void employee::extract(char* buf, const char* string_literal, int& pos)
{
	int position;
	position = 0;
        while (*(string_literal + pos) != ' ' && *(string_literal + pos))
        {
		*(buf + position) = *(string_literal + pos);
		pos++;
		position++;
        }
	*(buf + position) = 0;
	pos++;	
}

void employee::get_digits(char* abuf, int age, int pos)
{
	if (age)
	{
		*(abuf + pos) = (age % 10) + '0';
		pos--;
		get_digits(abuf + pos, age / 10, pos);
	}
}

employee::employee()
{
	printf(" ");
}

employee::employee(const char* string_literal, const char* brth_dt, const char* sx)
{
	size_t len;
	int pos;
	pos = 0;
	extract(first_name, string_literal, pos);		
	extract(paternal_name, string_literal, pos);
	extract(last_name, string_literal, pos);
	len = strlen(brth_dt);
	strcpy(birth_date, brth_dt);
	*(birth_date + len) = 0;
	len = strlen(sx);
	strcpy(sex, sx);
	*(sex + len) = 0;
}

employee::employee(const char* fname, const char* pname, const char* lname, const char* brth_dt, const char* sx)
{
	size_t len;
	len = strlen(fname);
	strcpy(first_name, fname);
	*(first_name + len) = 0;
	len = strlen(pname);
	strcpy(paternal_name, pname);
	*(paternal_name + len) = 0;
	len = strlen(lname);
	strcpy(last_name, lname);
	*(last_name + len) = 0;
	len = strlen(brth_dt);
	strcpy(birth_date, brth_dt);
	*(birth_date + len) = 0;
	len = strlen(sx);
	strcpy(sex, sx);
	*(sex + len) = 0;
}

void employee::send_to_database(pqxx::work& tr)
{
	try
	{
		pqxx::params p;
		p.append(first_name);
		p.append(paternal_name);
		p.append(last_name);
		p.append(birth_date);
		p.append(sex);
		pqxx::result r = tr.exec_params("INSERT INTO employee (first_name, paternal_name, last_name, birth_date, sex) VALUES ($1, $2, $3, $4, $5)", p);
                tr.commit();		
	}
	catch (const pqxx::sql_error& e)
	{
		fprintf(stderr, "SQL querry error: In function void employee::send_to_database(const pqxx::work& tr)->pqxx::result r = tr.exec() %s\n", e.what());
                exit(1);
	}
	catch (const std::exception& e)
	{
		fprintf(stderr, "ERROR: In function void employee::send_to_database(const pqxx::work& tr)->pqxx::result r = tr.exec(); %s\n", e.what());
		exit(2);
	}
}

void employee::get_employee_age(const char* date, char* abuf)	
{
	int year, month, day, pos, cyear, cmonth, cday, age;
	time_t t;
	struct tm* st;
	year = 0;
	month = 0;
	day = 0;
	pos = 0;
	extract(date, year, pos);
	extract(date, month, pos);
	extract(date, day, pos);
	t = time(NULL);
	st = localtime(&t);
	cyear = 1900 + st->tm_year;
	cmonth = 1 + st->tm_mon;
	cday = st->tm_mday;
	age = cyear - year;
	if (cmonth < month || (cmonth == month && cday < day))
        	age--;
	pos = 1;
	get_digits(abuf, age, pos);
}

employee::~employee()
{
        *first_name = 0;
        *paternal_name = 0;
        *last_name = 0;
        *birth_date = 0;
        *sex = 0;
}

