#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "..\includes\stack.h"

#include "commands.c"

enum ERRORS
{
	WRONG_INPUT_PARAMS = -1,
	RUN_PROG_ERROR = -2,
	UNKNOWN_COMMAND = -3,
	NULL_PTR_ERROR = -4,
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

typedef struct _command
{
	int code;

	int arg_count;

	double arg;

	struct _command* next_command;

} Command;

#define MAX_PROGRAMM_SIZE  5 * 1024


const int START_CAPACITY = 32;


int (*commands_arr[])(struct Stack*) = {hatl, //0
															 NULL, //1 - push (many args)
															 add,  //2
															 sub,  //3
															 mult, //4
															 divis,  //5
															 out}; //6

int read_programm_file(const char* programm_file_name, char** buffer)
{
	FILE* programm_file = fopen(programm_file_name, "r");

	if (programm_file == NULL)
	{
		fprintf(stderr, "!! Wrong file name, please enter a whole path to the file\n");
		return WRONG_INPUT_PARAMS;
	}

	*buffer = (char*) calloc(MAX_PROGRAMM_SIZE + 1, sizeof(char)); //TODO нормальный размер каллока

	fread(*buffer, sizeof(char), MAX_PROGRAMM_SIZE, programm_file);

	fclose(programm_file);
}



int read_programm(const char* programm_file_name, Command* programm_list)
{
	char* buffer = NULL;

	read_programm_file(programm_file_name, &buffer);

	printf("buffer = <%s>\n", buffer);


	int code = -1;
	int arg_count = 0;
	double arg = NAN;

	size_t line = 0;

	char* _buffer = buffer;

	int offset = 0;

	while (sscanf(_buffer, "%d%n", &code, &offset) > 1)
	{	
		_buffer += offset;

		line++;

		if (code == PUSH)
		{
			arg_count = 1;

			sscanf(buffer, "%lg ", &arg);
		}

		printf("line = %zu, Command = {code = %d, arg_count = %d, args = %lg}\n", line, code, arg_count, arg);

		switch (code)
		{
			case PUSH: case ADD: case SUB: case MULT: case DIV: case OUT: case HLT:

				Command* new_command = (Command*) malloc(sizeof(Command));

				if (new_command == NULL) {fprintf(stderr, "ERROR while malloc new programm\n"); return -4;}

				*new_command = (Command) {.code = code, .arg_count = arg_count, .arg = arg, .next_command = programm_list};

				programm_list = new_command;

				break;

			default:

				fprintf(stderr, "Calculation ERROR - Unknowm Command in %s:%zu\n", programm_file_name, line);

				return UNKNOWN_COMMAND;
		}		


	}

	free(buffer);

	return 0;


}


int run_programm(struct Stack* stk, Command* prog_lst)
{
	if (prog_lst == NULL) return 0;

	if (prog_lst->arg_count == 0) 
	{	
		commands_arr[prog_lst->code](stk);

		return run_programm(stk, prog_lst->next_command);
	}

	if (prog_lst->arg_count == 1)
	{
		stack_push(stk, prog_lst->arg);

		return run_programm(stk, prog_lst->next_command);
	}

	fprintf(stderr, "ERROR in run_programm\n");

	return -2;
}

int free_programm(Command* list)
{
	if (list == NULL) return 0;

	Command* next_command = list->next_command;

	free(list);

	return free_programm(next_command);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "!! Wrong input parametrs. Expected: <assler \"path-to-file\"\n");
		return WRONG_INPUT_PARAMS;
	}



	Command* programm_list = NULL;

	struct Stack main_stack = {};
	stack_init(&main_stack, START_CAPACITY);

	read_programm(argv[1], programm_list);

	run_programm(&main_stack, programm_list);

	stack_destroy(&main_stack);
	free_programm(programm_list);

	return 0;
}