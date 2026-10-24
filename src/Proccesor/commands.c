bool dbl_eq(double a, double b)
{
	return fabs(a - b) < 10e-5; 
}

int hatl(SPU* spu, double empty_arg) //0
{
	exit(0);

	return 0;
}

int push(SPU* spu, double arg) //1
{
	stack_push(&spu->main_stack, arg);

	return 0;
}

int push_from_ram(SPU* spu, double reg_num)
{
	size_t ram_adress = (size_t) spu->registers[(int) reg_num];
	stack_push(&spu->main_stack, spu->RAM[ram_adress]);

	return 0;
}

int pop_to_ram(SPU* spu, double reg_num)
{
	double val = NAN;
	size_t ram_adress = (size_t) spu->registers[(int) reg_num];

	stack_pop(&spu->main_stack, &val);
	spu->RAM[ram_adress] = val;

	return 0;
}

int push_from_reg(SPU* spu, double _reg) //2
{
	int reg = (int) _reg;
	//Проверка на номер регистра!!
	stack_push(&spu->main_stack, spu->registers[reg]);

	return 0;
}

int pop(SPU* spu, double _reg)
{
	int reg = (int) _reg;
	//Проверка на номер регистра!!

	double val = NAN;
	stack_pop(&spu->main_stack, &val);

	spu->registers[reg] = val;

	return 0;
}

int add(SPU* spu, double empty_arg) //2
{
	FUNC_START

	double a =  NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);

	stack_push(&spu->main_stack, a + b);

	FUNC_END
	return 0;
}

int sub(SPU* spu, double empty_arg) //3
{
	double a =  NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);

	stack_push(&spu->main_stack, a - b);

	return 0;
}

int mult(SPU* spu, double empty_arg) //4
{
	double a =  NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);

	stack_push(&spu->main_stack, a * b);

	return 0;
}

int divid(SPU* spu, double empty_arg) //5
{
	double a =  NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	//проверка на деление на 0
	stack_push(&spu->main_stack, a / b);

	return 0;
}

int sqroot(SPU* spu, double empty_arg) //5
{
	double a =  NAN;

	stack_pop(&spu->main_stack, &a);
	//проверка на отриц число
	stack_push(&spu->main_stack, sqrt(a));

	return 0;	
}

int square(SPU* spu, double empty_arg) //5
{
	double a =  NAN;

	stack_pop(&spu->main_stack, &a);

	stack_push(&spu->main_stack, a * a);

	return 0;
}


int jump(SPU* spu, double adress)
{
	spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int jump_eq(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (dbl_eq(a, b)) {printf("jump equal is working a = <%lg>, b = <%lg>\n", a, b); spu->prog_counter = (size_t) adress;}
	//Проверка на адрес
	return 0;
}

int jump_not_eq(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (!dbl_eq(a, b)) spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int jump_less(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (a < b) spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int jump_eq_less(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (a < b || dbl_eq(a, b)) spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int jump_eq_more(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (a > b || dbl_eq(a, b)) spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int jump_more(SPU* spu, double adress)
{
	double a = NAN;
	double b = NAN;

	stack_pop(&spu->main_stack, &a);
	stack_pop(&spu->main_stack, &b);
	
	if (a > b) spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}


int call(SPU* spu, double adress)
{
	stack_push(&spu->return_stack, spu->prog_counter);

	spu->prog_counter = (size_t) adress;
	//Проверка на адрес
	return 0;
}

int return_(SPU* spu, double empty_arg)
{
	double return_adress = NAN;

	stack_pop(&spu->return_stack, &return_adress);

	spu->prog_counter = (size_t) return_adress;
	return 0;
}

int input(SPU* spu, double empty_arg)
{
	printf("INPUT:: ");

	double val = NAN;
	scanf("%lg", &val);
	printf("input val = %lg\n", val);

	stack_push(&spu->main_stack, val);

	return 0;
}


int output(SPU* spu, double empty_arg) //6
{
	double val = NAN;

	stack_pop(&spu->main_stack, &val);

	printf("OUTPUT :: ----->>>>> <%lg> <<<<<-------\n", val);

	return 0;
}

int draw(SPU* spu, double empty_arg)
{
	char* shades = " .:-=+*#%@";
	int shade = -1;

	for (int y = 0; y < SIZE_Y; y++)
	{
		for (int x = 0; x < SIZE_X; x++)
		{
			switch(shade = (int) spu->RAM[y * SIZE_X  + x])
			{
				case 0 ... 9:
					putchar(shades[shade]);
					break;

				default:
					putchar(' ');
					break;
			}
		}

		putchar('\n');
	}

	return 0;
}


