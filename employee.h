#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <stdio.h>
#include <stdlib.h>
#include <pqxx/pqxx>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <string>
enum {length = 255};
class employee
{
	char first_name[length];
	char paternal_name[length];
	char last_name[length];
	char birth_date[length];
	char sex[length];
	void extract(const char* birth, int& arg, int& pos);
	void extract(char* buf, const char* string_literal, int& pos);
	void get_digits(char* abuf, int age, int pos);
public:
	employee();
	employee(const char* string_literal, const char* brth_dt, const char* sx);
	employee(const char* fname, const char* pname, const char* lname, const char* brth_dt, const char* sx);
	void send_to_database(pqxx::work& tr);
	void get_employee_age(const char* date, char* abuf);
	~employee();
};
#endif
