#ifndef PROCCESOR_H
#define PROCCESOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <unistd.h>	

#include "..\..\includes\Stack\stack.h"


enum OPCODES
{
	HLT = 0,

	PUSH = 1,
	PUSHR = 2,
	POP = 3,

	ADD = 4,
	SUB = 5,
	MULT = 6,
	DIV = 7,
	SQRT = 8,
	SQR = 9,

	JMP = 10,
	JE = 11,
	JNE = 12,
	JB = 13,
	JBE = 14,
	JA = 15,
	JAE = 16,

	CALL = 17,
	RET = 18,

	IN = 19,
	OUT = 20,

	PUSHM = 21,
	POPM = 22,

	OPCODES_COUNT,
};

#define COM_WITH_ARGS_FOR_SWITCH PUSH: case PUSHR: case POP: case JMP...JAE: case CALL

#define NUM_REGISTERS 6

typedef struct 
{
	struct Stack main_stack;

	double* code_segment;
	size_t code_segment_size;
	size_t prog_counter;

	double registers[NUM_REGISTERS];

	struct Stack return_stack;
} SPU;

const size_t START_CAPACITY = 32;
const size_t MAX_SIZE_CODE_SEGMENT = 8192; //TODO dynamic resize

#include "commands.c"

int (*commands_arr[])(SPU* spu, double arg) = { 
										 hatl, //0
										 push, //1 
										 push_from_reg, //2
										 pop,  //3

										 add,   //4
										 sub,   //5
										 mult,  //6
			 							 divid, //7
			 							 sqroot,  //8
			 							 square,//9

			 							 jump,         //10
			 							 jump_eq,      //11
			 							 jump_not_eq,  //12
			 							 jump_less,    //13
			 							 jump_eq_less, //14
			 							 jump_more,    //15
			 							 jump_eq_more, //16

			 							 call, //17
			 							 return_,  //18

			 							 input,   //19
										 output   //20
									     };   


#endif