#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <unistd.h>	

#include "..\includes\Stack\stack.h"

enum ERRORS
{
	WRONG_INPUT_PARAMS = -1,
	RUN_PROG_ERROR = -2,
	UNKNOWN_COMMAND = -3,
	NULL_PTR_ERROR = -4,
	READ_FILE_ERROR = -5,
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



const size_t START_CAPACITY = 32;
const size_t MAX_SIZE_CODE_SEGMENT = 1024; //TODO dynamic resize

#include "commands.c"

int (*commands_arr[])(struct Stack*, double arg) = { hatl, //0
										 push, //1 - push (many args)
										 add,  //2
										 sub,  //3
										 mult, //4
			 							 divis,  //5
										 out}; //6

int text_beater(char** text_ptr, char* string_pointers[]) //Разделение всего текста  на строки и занесение в массив указателей на строки string_pointers[]
{
	FUNC_START

	size_t pos = 0;

	size_t i = 0;

	char* text = *text_ptr;

	//printf("text = <%s>\n", text);

	string_pointers[0] = text;

	while (text[pos] != '\0' && text[pos] != EOF) //Идем до конца текста
	{
		//printf("char in text in pos <%d> = <%c>\n", pos, text[pos]);
		if (text[pos] == '\n')
		{
			i++;
			string_pointers[i] = &(text[pos]);

			//printf("PTR added to the str_ptrs into i = %d\n", i);
		}
		pos++;
	}

	FUNC_END
	return i + 1;
}

size_t count_lines(char* text)
{
	FUNC_START
	size_t i = 1;

 	char* cur = strchr(text, '\n');

  	while (cur != NULL) 
  	{
    	i++;
    	cur = strchr(cur + 1, '\n');
  	}

  	return i;
}

int read_programm_file(const char* programm_file_name, char** programm_text)
{
	FUNC_START

	int programm_file_descriptor = open(programm_file_name, O_BINARY);

	if (programm_file_descriptor < 0)
	{
		fprintf(stderr, "!! Wrong file name, please enter a whole path to the file\n");
		return WRONG_INPUT_PARAMS;
	}

	char cur_char = '\0';
	struct stat programm_file_info = {};

	if (stat(programm_file_name, &programm_file_info)) return -fprintf(stderr, "cant get stat of file\n");

	*programm_text = (char*) calloc(((size_t) programm_file_info.st_size  + 2), sizeof(char));

	read(programm_file_descriptor, *programm_text, programm_file_info.st_size);
	(*programm_text)[programm_file_info.st_size] = '\0';
	//printf("just after read() text = <%s>\n", *programm_text);
	close(programm_file_descriptor);

	FUNC_END

	return count_lines(*programm_text);
}

size_t read_programm(const char* programm_file_name, double* code_segment)
{
	char* programm_text = NULL;
	size_t lines_num = 0;

	if (lines_num = read_programm_file(programm_file_name, &programm_text) < 0) {fprintf(stderr, "READ file error\n"); return READ_FILE_ERROR;}   

	char** line_pointers = (char**) calloc(lines_num + 3, sizeof(char*));

	text_beater(&programm_text, line_pointers);

	//printf("buffer = <%s>\n", programm_text);

	int opcode = -1;
	double arg = NAN;
	size_t line = 0;
	size_t i = 0;
	int instruction_size = 0;

	//printf("code_segment = [");

	while (instruction_size =  sscanf(line_pointers[line], "%d %lg", &opcode, &arg) > 0)
	{
		//printf("line = <%s>, instruction_size = <%d>, ", line_pointers[line], instruction_size);
		code_segment[i] = (double) opcode;
		//printf("opcode = <%d>(%lg), ", opcode, code_segment[i]);
		i++;

		if (opcode == 1)
		{
			code_segment[i] = arg;
			//printf("arg = <%lg>(%lg), ", arg, code_segment[i]);
			i++;
		}

		line++;
	}
	puts("]\n");

	free(line_pointers);
	free(programm_text);

	return i;
}

void print_array(double* arr, size_t size)
{
	putchar('[');
	for (int i = 0; i < size; i++)
	{
		printf("<%d>,", (int) arr[i]);
	}
	puts("\b]\n");
}

int run_programm(struct Stack* stk, double* code_segment, size_t code_segment_size)
{
	size_t prog_counter = 0;
	int opcode = -1;
	double arg = NAN;

	print_array(code_segment, 12);

	while(prog_counter < code_segment_size)
	{
		opcode = (int) code_segment[prog_counter];
		arg = NAN;

		switch(opcode)
		{
			case HLT:
				printf("BREAK!!\n");
				return 0;

			case PUSH: 
				prog_counter++;
				arg = code_segment[prog_counter];
		}

		printf("opcode = <%d>, arg = <%lg>\n", opcode, arg);

		commands_arr[opcode](stk, arg);

		prog_counter+=1;

		print_stack(stk);
	}
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "!! Wrong input parametrs. Expected: <calc \"path-to-objectfile\" \n");
		return WRONG_INPUT_PARAMS;
	}

	double* code_segment = (double*) calloc(MAX_SIZE_CODE_SEGMENT, sizeof(double)); //TODO dynamic realloc

	struct Stack main_stack = {};
	stack_init(&main_stack, START_CAPACITY);

	size_t code_segment_size = read_programm(argv[1], code_segment);

	run_programm(&main_stack, code_segment, code_segment_size);

	stack_destroy(&main_stack);
	free(code_segment);

	FUNC_END
	return 0;
}