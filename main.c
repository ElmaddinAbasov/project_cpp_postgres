#include "app.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Provide command line arguments\n");
		exit(1);
	}
	my_application app("emp_db");
	switch (argv[1][0] - '0')
	{
		case 1 :
			app.run_app(argv[1][0] - '0', NULL, NULL, NULL);
			break;
		case 2 :
			app.run_app(argv[1][0] - '0', argv[2], argv[3], argv[4]);
			break;	
		case 3 :
			app.run_app(argv[1][0] - '0', NULL, NULL, NULL);
			break;	
		case 4 :
			app.run_app(argv[1][0] - '0', NULL, NULL, NULL);
			break;
		case 5 :
			app.run_app(argv[1][0] - '0', NULL, NULL, NULL);	
			break;
	}
	exit(0);
}
