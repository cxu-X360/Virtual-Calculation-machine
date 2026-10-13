int hatl(struct Stack* stk, double empty_arg) //0
{
	exit(0);

	return 0;
}

int push(struct Stack* stk, double arg) //1
{
	stack_push(stk, arg);

	return 0;
}

int add(struct Stack* stk, double empty_arg) //2
{
	FUNC_START

	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);



	stack_push(stk, a + b);

	FUNC_END
	return 0;
}

int sub(struct Stack* stk, double empty_arg) //3
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a - b);

	return 0;
}

int mult(struct Stack* stk, double empty_arg) //4
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a * b);

	return 0;
}

int divis(struct Stack* stk, double empty_arg) //5
{
	double a =  NAN;
	double b = NAN;

	stack_pop(stk, &a);
	stack_pop(stk, &b);

	stack_push(stk, a / b);

	return 0;
}

int out(struct Stack* stk, double empty_arg) //6
{
	double val = NAN;

	stack_pop(stk, &val);

	printf("OUTPUT :: ----->>>>> <%lg> <<<<<-------\n", val);

	return 0;
}


