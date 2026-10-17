#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>



enum ERRORS
{
	WRONG_INP_PARAMS = -1,
	TRANSLATION_ERR = -2,
};

enum OPCODES
{
	PUSH = 1,
	ADD = 2,
	SUB = 3,
	MULT = 4,
	DIV = 5,
	OUT = 6, 
	HLT = 0,
};

const char*  OPCODES_names[] = {"hlt", "push", "add", "sub", "mult", "div", "out"};

char* str_lower(char* str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		str[i] = tolower(str[i]);
		i++;
	}

	return str;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "!! Wrong input parametrs. Expected: <assler \"path to objective file\" \"path to programm file\"\n");
		return WRONG_INP_PARAMS;
	}

	FILE* objective_file = fopen(argv[1], "r");
	FILE* programm_file = fopen(argv[2], "w");

	if (programm_file == NULL)
	{
		fprintf(stderr, "!! Wrong programm file name, please enter a whole path to the file\n");
		return WRONG_INP_PARAMS;
	}

	if (objective_file == NULL)
	{
		fprintf(stderr, "!! Wrong objective file name, please enter a whole path to the file\n");
		return WRONG_INP_PARAMS;
	}

	int cur_command = 0;

	size_t line = 0;

	while (fscanf(objective_file, "%d", &cur_command) > 0)
	{
		line++;
		printf("line = %d, command = <%d>\n", line, cur_command);


 		switch (cur_command)
 		{
 			case PUSH:

	 			double value = NAN;

				fscanf(objective_file, "%lg", &value);
				fprintf(programm_file, "%s %lg\n", OPCODES_names[cur_command], value);

				break;	

			case ADD: case SUB: case MULT: case DIV: case OUT: case HLT:

				fprintf(programm_file, "%s\n", OPCODES_names[cur_command]);

				break;

			default:

				fprintf(stderr, "Translation ERROR in %s:%zu\n", argv[1], line);

				return TRANSLATION_ERR;
 		}	



	}

	fclose(programm_file);
	fclose(objective_file);

	return 0;
}