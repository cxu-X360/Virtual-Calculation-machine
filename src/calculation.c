#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "C:\Users\Роман\Desktop\Stack\Stack_git\include\stack.h"

enum ERRORS
{
	WRONG_INPUT_PARAMS = -1,

};

typedef struct _command
{
	char name[16];

	int code;

	int length;

	int arg;

	struct _command* next_command;
} Command;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "!! Wrong input parametrs. Expected: <assler \"path-to-file\"\n");
		return -1;
	}

	FILE* programm = fopen(argv[1], "r");

	if (programm == NULL)
	{
		fprintf(stderr, "!! Wrong file name, please enter a whole path to the file\n");
		return -1;
	}

	Command* programm_list = NULL;







}