#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

enum ERRORS
{
	WRONG_INP_PARAMS = -1,
	TRANSLATION_ERR = -2,
};

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
		fprintf(stderr, "!! Wrong input parametrs. Expected: <assler \"path to programm file\" \"path to objective file\"\n");
		return WRONG_INP_PARAMS;
	}

	FILE* programm_file = fopen(argv[1], "r");
	FILE* objective_file = fopen(argv[2], "w");

	if (programm_file == NULL)
	{
		fprintf(stderr, "!! Wrong programm file name, please enter a whole path to the file\n");
		return WRONG_INP_PARAMS;
	}

	if (objective_file == NULL)
	{
		fprintf(stderr, "!! Wrong objective file name\n");
		return WRONG_INP_PARAMS;
	}

	char cur_command[16] = {};

	size_t line = 0;

	while (fscanf(programm_file, "%s", cur_command) > 0)
	{
		
		line++;
		printf("line = %d, command = <%s>\n", line, cur_command);

		str_lower(cur_command);

		if (!strcmp(cur_command, "push"))
		{
			double value = NAN; 

			fscanf(programm_file, "%lg", &value);

			fprintf(objective_file, "%d %lg\n", PUSH, value);

			continue;
		}

		if (!strcmp(cur_command, "add"))
		{
			fprintf(objective_file, "%d\n", ADD);
			continue;
		}

		if (!strcmp(cur_command, "sub"))
		{
			fprintf(objective_file, "%d\n", SUB);
			continue;
		}
		if (!strcmp(cur_command, "mult"))
		{
			fprintf(objective_file, "%d\n", MULT);
			continue;
		}
		if (!strcmp(cur_command, "div"))
		{
			fprintf(objective_file, "%d\n", DIV);
			continue;
		}
		if (!strcmp(cur_command, "out"))
		{
			fprintf(objective_file, "%d\n", OUT);
			continue;
		}
		if (!strcmp(cur_command, "hlt"))
		{
			fprintf(objective_file, "%d\n", HLT);
			continue;
		}

		fprintf(stderr, "Translation ERROR in %s:%zu\n", argv[1], line);
		return TRANSLATION_ERR;

	}

	fclose(programm_file);
	fclose(objective_file);

	return 0;
}