int hatl(struct Stack* stk) //0
{
	exit(0);

	return 0;
}


int add(struct Stack* stk) //2
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a + b);

	return 0;
}

int sub(struct Stack* stk) //3
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a - b);

	return 0;
}

int mult(struct Stack* stk) //4
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a * b);

	return 0;
}

int divis(struct Stack* stk) //5
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a / b);

	return 0;
}

int out(struct Stack* stk) //6
{
	double val = NAN;

	stack_pop(stk, &val);

	printf("%lg\n", val);

	return 0;
}


