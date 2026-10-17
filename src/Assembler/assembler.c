#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "..\Proccesor\proccesor.h"

enum ERRORS
{
	WRONG_INP_PARAMS = -1,
	TRANSLATION_ERR = -2,
};

#define MAX_COMMAND_NAME_LEN  16
#define START_SIZE_OF_TEXT  256
#define MAX_LABEL_NUM  256
#define OBJ_COMMAND_MAX_LINE_SIZE  16

#define CHECK(opcode) 			if (!strcmp(str_upper(command_name), #opcode))\
								{\
									return opcode;\
								}

const char* opcode_names[OPCODES_COUNT] = {};


typedef struct
{
	char* label_table;
	int label_counter;

	size_t lines_num;
	char* programm_text;
	char** prog_line_pointers;

	size_t obj_text_size;
	char* obj_text;
	char* obj_text_pointer;
} AContext; //AsemblerContext

typedef struct 
{
	char* pointer_in_line;
	size_t line;
	
	char command_name[MAX_COMMAND_NAME_LEN];
	int opcode;
	double arg;		
} LContext; //LineContext


char* str_upper(char* str)
{
	//FUNC_START

	//printf("command to up = <%s>", str);
	int i = 0;
	while (isalpha(str[i]))
	{
		//printf("Command char to up = <%c>\n", str[i]);
		str[i] = toupper(str[i]);
		i++;
	}
	//printf("command uped = <%s>", str);
	//FUNC_END

	return str;
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
  	FUNC_END
  	return i;
}


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
			while (isspace(text[pos])) pos++;
			i++;
			string_pointers[i] = &(text[pos]);
			//printf("PTR added to the str_ptrs into i = %d\n", i);
		}
		pos++;
	}

	FUNC_END
	return i + 1;
}

int read_programm_file(const char* programm_file_name, char** programm_text)
{
	FUNC_START
	int programm_file_descriptor = open(programm_file_name, O_BINARY);
	if (programm_file_descriptor < 0)
	{
		fprintf(stderr, "!! Wrong file name, please enter a whole path to the file\n");
		return WRONG_INP_PARAMS;
	}

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

int djb2_hash(char* str)
{
	int hash = 5381;
	int ch = '\0';

	while (ch = *str++)
	{
		hash = ((hash << 5) + hash) + ch;	
	}
	return hash;
}

int check_command(char* command_name)
{
	FUNC_START

	//printf("command to check = <%s>\n", command_name);

	CHECK(HLT)
	CHECK(PUSH)
	CHECK(POP)
	CHECK(ADD)
	CHECK(SUB)
	CHECK(MULT)
	CHECK(DIV)
	CHECK(SQRT)
	CHECK(SQR)
	CHECK(JMP)
	CHECK(JE)
	CHECK(JNE)
	CHECK(JB)
	CHECK(JA)
	CHECK(JAE)
	CHECK(CALL)
	CHECK(RET)
	CHECK(IN)
	CHECK(OUT)
	CHECK(PUSHM)
	CHECK(POPM)

	FUNC_END
	return TRANSLATION_ERR;
}

int check_arg(AContext* ctx, LContext* line_ctx)
{
	FUNC_START
	int offset = 0;
	//printf("opcode in arg = <%d>\n", line_ctx->opcode);

	switch(line_ctx->opcode)
		{
			case PUSH: 
				if (sscanf(line_ctx->pointer_in_line, "%lg%n", &(line_ctx->arg), &offset) != 1)
				{
					fprintf(stderr, "WROND ARG <%lg> in line %zu\n", line_ctx->arg, line_ctx->line); 
					return TRANSLATION_ERR;
				}
				break;	

			case PUSHR: case POP:
				int reg_num = -1;

				if (sscanf(line_ctx->pointer_in_line, " R%[0-5]%n", &reg_num, &offset) != 1)
				{
					fprintf(stderr, "WROND REG_NUM <%d> in line %zu\n", reg_num, line_ctx->line); 
					return TRANSLATION_ERR;
				}

				line_ctx->arg = (double) reg_num;
				break;

			case JMP...CALL:
				size_t jump_adress = -1;

				if(sscanf(line_ctx->pointer_in_line, " %zu%n", &jump_adress, &offset) != 1)
				{
					fprintf(stderr, "WROND JUMP_ADDRES <%zu> in line %zu\n", jump_adress, line_ctx->line); 
					return TRANSLATION_ERR;
				}

				(line_ctx->arg) = (double) jump_adress;
				break;

			case PUSHM: case POPM:
				size_t ram_adress = -1;

				if(sscanf(line_ctx->pointer_in_line, " [%zu]%n", &ram_adress, &offset) != 1)
				{
					fprintf(stderr, "WROND RAM_ADDRES <%zu> in line %zu\n", ram_adress, line_ctx->line); 
					return TRANSLATION_ERR;
				}

				(line_ctx->arg) = (double) ram_adress;
				break;

			//case JMP...CALL
		}
	//printf("offset in chaeck_arg = <%d>\n", offset);
	line_ctx->pointer_in_line += offset;
	FUNC_END
	return 0;
}

int check_trash(char* str)
{
	FUNC_START

	//printf("residual line = <%s>\n", str);
	int i = 0;

	while(str[i] != '#' && str[i] != '\n' && str[i] != '\0')
	{
		if (isspace(str[i++])) continue;


		return -1;
	}
	FUNC_END
	return 0;
}

bool is_empty(char* str)
{
	int i = 0;
	while (str[i] != '\n')
	{
		if (!isspace(str[i])) return false;

		i++;
	}
	return true;
}

int translation(AContext* ctx)
{
	FUNC_START

	for (size_t line = 0; line < ctx->lines_num; line++)
	{
		LContext line_ctx = {	 .pointer_in_line = ctx->prog_line_pointers[line],
								 .line = line,
								 .opcode = -1,
		 						 .arg = NAN};
		int offset = 0;

		if(*line_ctx.command_name == '\n') continue;

		//printf("cur_line = <%s>\n", line_ctx.pointer_in_line);

		if(sscanf(line_ctx.pointer_in_line, "%s%n", line_ctx.command_name, &offset) != 1) continue;
		//printf("cur comman_name = <%s>\n", line_ctx.command_name);



		if((line_ctx.opcode = check_command(line_ctx.command_name)) < 0) 
		{
			fprintf(stderr, "WROND COMMAND <%s> in line %zu\n", line_ctx.command_name, line); 
			return TRANSLATION_ERR;
		}
		//printf("opcode in translation = <%d>\n", line_ctx.opcode);
		line_ctx.pointer_in_line += offset + 1;

		if (check_arg(ctx, &line_ctx) < 0)
		{
			fprintf(stderr, "WROND ARGUMENT in line %zu\n", line_ctx.line); 
			return TRANSLATION_ERR;
		}

		if (check_trash(line_ctx.pointer_in_line))
		{
			fprintf(stderr, "WRONG EXPRESSION in line %zu\n", line_ctx.line); 
			return TRANSLATION_ERR;
		}

		//printf("arg = <%lg>\n", line_ctx.arg);

		ctx->obj_text_pointer += sprintf(ctx->obj_text_pointer, (isnan(line_ctx.arg)) ? "%d\n" : "%d %lg\n" , line_ctx.opcode, line_ctx.arg);
		//printf("obj_text = <%s>\n", ctx->obj_text);
	}

	FUNC_END
	return 0;
}

int write_objective_file(const char* objective_file_name, char* obj_text)
{
	FUNC_START

	//int obj_file_descriptor = open(objective_file_name, O_CREAT, S_IWUSR); //UNKNOWN BUG
	FILE* obj_file = fopen(objective_file_name, "w");
	if (!obj_file)
	{
		fprintf(stderr, "!! Wrong file name, please enter a whole path to the file\n");
		return WRONG_INP_PARAMS;
	}

	//printf("obj_text_len = <%zu>\n", strlen(obj_text));
	//write(obj_file_descriptor, obj_text, strlen(obj_text) + 1);
	fwrite(obj_text, sizeof(char), strlen(obj_text) - 1, obj_file);

	//close(obj_file_descriptor);
	fclose(obj_file);
	FUNC_END

	return 0;
}

void free_context(AContext* ctx)
{
	FUNC_START

	free(ctx->label_table);
	free(ctx->programm_text);
	free(ctx->prog_line_pointers);
	free(ctx->obj_text);
	FUNC_END
}

int main(int argc, char* argv[])
{
	FUNC_START

	if (argc != 3)
	{
		fprintf(stderr, "!! Wrong input parametrs. Expected: <assler \"path to programm file\" \"path to objective file\"\n");
		return WRONG_INP_PARAMS;
	}

	const char* programm_file_name = argv[1];
	const char* objective_file_name = argv[2];

	char* programm_text = NULL;

	AContext context = {.label_table = calloc(MAX_LABEL_NUM, sizeof(size_t)),
						.label_counter = 0,


						.lines_num = read_programm_file(programm_file_name, &(programm_text)),
						.prog_line_pointers = (char**) calloc(context.lines_num + 3, sizeof(char*)),

						.obj_text_size = (context.lines_num + 3) * OBJ_COMMAND_MAX_LINE_SIZE,
						.obj_text = (char*) calloc(context.obj_text_size, sizeof(char)),
						.obj_text_pointer = context.obj_text};

	text_beater(&(programm_text), context.prog_line_pointers);

	if (translation(&context))
	{
		fprintf(stderr, "TRANSLATION_ERR\n");
		return TRANSLATION_ERR;
	}

	//printf("obj_text = <%s>\n", context.obj_text);

	if(write_objective_file(objective_file_name, context.obj_text))
	{
		fprintf(stderr, "WRITING ERROR\n");
		return WRONG_INP_PARAMS;
	}

	free_context(&context);

	system(argv[2]);
	FUNC_END

	return 0;
}