#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free, strtod */
#include <math.h>   /* pow */
#include <assert.h> /* assert */

#include "calculator.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) ((type *)malloc(sizeof(type) * nelements));

static void InitVactorOfStatesHandler(int init_handlers_flag);
static void InitCalculatorFSM(int init_fsm_lut_flag);
static void InitVactorOfOperatorHandler(int init_op_lut_flag);
static int  ExecutonScheme(const char *str, double *result, char stop_condition,
	                                                             char **endptr);

static void InitializeErrorStateFSM(void);
static void InitializeWaitingForNumStateFSM(void );
static void InitializeWaitingForOpStateFSM(void );

static double ErrorHandler(char *str, char **endptr, char **op_stack_ptr, 
	                              char *begin_op_stack, double **num_stack_ptr);
static double NumberHandler(char *str, char **endptr, char **op_stack_ptr, 
	                              char *begin_op_stack, double **num_stack_ptr);
static double OperatorHandler(char *str, char **endptr, char **op_stack_ptr,  
	                              char *begin_op_stack, double **num_stack_ptr);

static double Pow(double num1, double num2);
static double Add(double num1, double num2);
static double Sub(double num1, double num2);
static double Div(double num1, double num2);
static double Multi(double num1, double num2);
static double Dummy(double num1, double num2);

static handler_ty vactor_handler[STATES_NUM];
static int fsm_lut[STATES_NUM][ASCII_SIZE];

static operator_ty operator_handler[NUMBER_OF_OP];
static int op_lut[ASCII_SIZE];  

static int init_handlers_flag = 0;
static int init_fsm_lut_flag = 0;
static int init_op_lut_flag = 0;

static int current_status = SUCCESS;



/*******************************************************************************
********************************** Calculate **********************************/

int Calculate(const char *str, double *result)
{
	char *endptr = NULL;
	char stop_condition = NULL_ASCII;
	current_status = SUCCESS;

	assert(NULL != str);
	assert(NULL != result);

	/* Initialize the handlers of states */
	InitVactorOfStatesHandler(init_handlers_flag);

	/* Initialize the FSM LUT */
	InitCalculatorFSM(init_fsm_lut_flag);

	/* Initialize the operator LUT */
	InitVactorOfOperatorHandler(init_op_lut_flag);

	/* The main calculation */
	ExecutonScheme(str, result, stop_condition, &endptr);

	printf("result = %lf", *result);

    return current_status;
}


/*******************************************************************************
************************* Init Vactor Of States Handler ***********************/

static void InitVactorOfStatesHandler(int init_handlers_flag)
{ 
	/*  The initialization will occur only once, during all the lifetime 
		of the program */ 
	if (0 != init_handlers_flag) return;

	vactor_handler[ERROR].handler = ErrorHandler;
	vactor_handler[ERROR].next_state = ERROR; 
	
	vactor_handler[WAITING_FOR_NUM].handler = NumberHandler;
	vactor_handler[WAITING_FOR_NUM].next_state = WAITING_FOR_OP;
	
	vactor_handler[WAITING_FOR_OP].handler = OperatorHandler;
	vactor_handler[WAITING_FOR_OP].next_state = WAITING_FOR_NUM; 

	++init_handlers_flag;
}

/*******************************************************************************
****************************** Init Calculator FSM ****************************/

static void InitCalculatorFSM(int init_fsm_lut_flag)
{ 
	/*  The initialization will occur only once, during all the lifetime 
		of the program */ 
	if (0 != init_fsm_lut_flag) return;

	/* initialization of ERROR state */
	InitializeErrorStateFSM();

	/* initialization of WAITING FOR NUM state */
	InitializeWaitingForNumStateFSM();

	/* initialization of WAITING FOR OP state */
	InitializeWaitingForOpStateFSM();

	++init_fsm_lut_flag;
}

/**************************** Init Error State FSM ****************************/

static void InitializeErrorStateFSM(void)
{
	unsigned int i = 0;
	
	for ( i = 0 ; i < ASCII_SIZE ; i++ )
	{
		fsm_lut[ERROR][i] = ERROR;  
	}
}

/*********************** Init Waiting For Num State FSM ***********************/

static void InitializeWaitingForNumStateFSM(void)
{
	unsigned int i = 0;

	for ( i = 0 ; i < ASCII_SIZE ; i++ )
	{
		fsm_lut[WAITING_FOR_NUM][i] = ERROR;  
	}

	for ( i = ZERO_ASCII ; i < ZERO_ASCII + 10 ; i++ )
	{
		fsm_lut[WAITING_FOR_NUM][i] = WAITING_FOR_NUM;   
	}

	/* Unary operators */
	fsm_lut[WAITING_FOR_NUM][SUB_ASCII] = WAITING_FOR_NUM; 
	fsm_lut[WAITING_FOR_NUM][ADD_ASCII] = WAITING_FOR_NUM;

	fsm_lut[WAITING_FOR_NUM][OPEN_PARENTHESIS_ASCII] = WAITING_FOR_NUM;
}

/*********************** Init Waiting For Op State FSM ************************/

static void InitializeWaitingForOpStateFSM(void)
{
	unsigned int i = 0;
	
	for ( i = 0 ; i < ASCII_SIZE ; i++ )
	{
		fsm_lut[WAITING_FOR_OP][i] = ERROR;  
	}

	fsm_lut[WAITING_FOR_OP][MULTI_ASCII] = WAITING_FOR_OP;

	fsm_lut[WAITING_FOR_OP][ADD_ASCII] = WAITING_FOR_OP;	
	fsm_lut[WAITING_FOR_OP][SUB_ASCII] = WAITING_FOR_OP;	
	fsm_lut[WAITING_FOR_OP][DIV_ASCII] = WAITING_FOR_OP;	
	fsm_lut[WAITING_FOR_OP][POW_ASCII] = WAITING_FOR_OP;	   
}

/*******************************************************************************
************************* Init Vactor Of Operator Handler *********************/

static void InitVactorOfOperatorHandler(int init_op_lut_flag)
{
	unsigned int i = 0;
		
	/*  The initialization will occur only once, during all the lifetime 
		sof the program */	
	if (0 != init_op_lut_flag) return;

	/* Initialize the op LUT */
	for ( i = 0 ; i < ASCII_SIZE ; i++ )
	{
		op_lut[i] = NUMBER_OF_OP;  
	} 

	/* Initialize the add operation characteristics */

	operator_handler[ADD].operation_precedence = ADD_PRE;
	operator_handler[ADD].operation_function = Add;
	operator_handler[ADD].associativity = RL;

	op_lut[ADD_ASCII] = ADD;

	/* Initialize the sub operation characteristics */

	operator_handler[SUB].operation_precedence = SUB_PRE;
	operator_handler[SUB].operation_function = Sub;
	operator_handler[SUB].associativity = RL;

	op_lut[SUB_ASCII] = SUB;

	/* Initialize the div operation characteristics */

	operator_handler[DIV].operation_precedence = DIV_PRE;
	operator_handler[DIV].operation_function = Div;
	operator_handler[DIV].associativity = RL;

	op_lut[DIV_ASCII] = DIV;

	/* Initialize the multi operation characteristics */

	operator_handler[MULTI].operation_precedence = MULTI_PRE;
	operator_handler[MULTI].operation_function = Multi;
	operator_handler[MULTI].associativity = RL;

	op_lut[MULTI_ASCII] = MULTI;

	/* Initialize the pow operation characteristics */

	operator_handler[POW].operation_precedence = POW_PRE;
	operator_handler[POW].operation_function = Pow;
	operator_handler[POW].associativity = LR;

	op_lut[POW_ASCII] = POW;

	/* Initialize the stop condition characteristics */

	operator_handler[STOP_CONDITION].operation_precedence = ADD_PRE;
	operator_handler[STOP_CONDITION].operation_function = Dummy;
	operator_handler[STOP_CONDITION].associativity = RL;

	op_lut[CLOSE_PARENTHESIS_ASCII] = STOP_CONDITION;
	op_lut[NULL_ASCII] = STOP_CONDITION;

	++init_op_lut_flag;
}

/*******************************************************************************
********************************** Error Handler ******************************/

static double ErrorHandler(char *str, char **endptr, char **op_stack_ptr, 
	                               char *begin_op_stack, double **num_stack_ptr)
{
	current_status = SYNTAX_ERROR;
	return 0;
}

/*******************************************************************************
********************************** Number Handler *****************************/

static double NumberHandler(char *str, char **endptr, char **op_stack_ptr, 
	                               char *begin_op_stack, double **num_stack_ptr)
{
	double number = 0;

	assert(NULL != str);
	assert(NULL != endptr);
	assert(NULL != op_stack_ptr);
	assert(NULL != begin_op_stack);
	assert(NULL != num_stack_ptr);
	
	if(OPEN_PARENTHESIS_ASCII == *str)
	{
		char stop_condition = CLOSE_PARENTHESIS_ASCII;
		ExecutonScheme(str+1, &number, stop_condition, endptr);
	}
	else
	{
		number = strtod(str, endptr);

		if(str == *endptr)
		{
			current_status = SYNTAX_ERROR;
			return 0;
		}
	}

	**num_stack_ptr = number;
	*num_stack_ptr += 1;

	return number;
}

/*******************************************************************************
******************************** Operator Handler *****************************/

static double OperatorHandler(char *str, char **endptr, char **op_stack_ptr, 
	                               char *begin_op_stack, double **num_stack_ptr)
{ 

	assert(NULL != str);
	assert(NULL != endptr);
	assert(NULL != op_stack_ptr);
	assert(NULL != begin_op_stack);
	assert(NULL != num_stack_ptr);

	while(operator_handler[op_lut[(int)*str]].operation_precedence <= 
		  operator_handler[op_lut[*(*op_stack_ptr-1)]].operation_precedence &&
		  operator_handler[op_lut[(int)*str]].associativity == RL  &&
        (*op_stack_ptr != begin_op_stack))

	{
		*(*num_stack_ptr - 2) = operator_handler[op_lut[*(*op_stack_ptr-1)]].
		         operation_function(*(*num_stack_ptr-1), *(*num_stack_ptr - 2));
		*(op_stack_ptr) -= 1;
		*(num_stack_ptr) -= 1;
	}

	**(op_stack_ptr) = *str;
	*(op_stack_ptr) += 1;

	*endptr += 1;

	return 0;
}

/*******************************************************************************
******************************** Execute Scheme *******************************/

static int ExecutonScheme(const char *str, double *result, char stop_condition,
                                                                  char **endptr)
{
	char *runner = (char *)str;
	char *begin_op_stack = NULL;
	double *begin_num_stack = NULL;
	char *op_stack_ptr = NULL;
	double *num_stack_ptr = NULL;

	state_ty state = WAITING_FOR_NUM;

	assert(NULL != str);

	*endptr = (char *)str;

	op_stack_ptr = MALLOC(NUMBER_OF_OP, char);

	if(NULL == op_stack_ptr)
	{
		return SYS_ERROR;
	}

	begin_op_stack = op_stack_ptr;

	num_stack_ptr = MALLOC(NUMBER_OF_OP, double);

	if(NULL == num_stack_ptr)
	{
		FREE(op_stack_ptr);
		return SYS_ERROR;
	}

	begin_num_stack = num_stack_ptr;

	while ((stop_condition != *runner) && (SYNTAX_ERROR != current_status))
	{
		if(' ' == *runner)
		{
			++*endptr;
			++runner;
			continue;
		}

		vactor_handler[fsm_lut[state][(int)*runner]].handler(runner, endptr,
		                         &op_stack_ptr, begin_op_stack, &num_stack_ptr);
		state = vactor_handler[fsm_lut[state][(int)*runner]].next_state;
		runner = *endptr;
	}

	if(WAITING_FOR_OP != state || SYNTAX_ERROR == current_status)
	{
		current_status = SYNTAX_ERROR;
		*result = 0;
		FREE(begin_op_stack)
		FREE(begin_num_stack)
		return current_status;
	}

	vactor_handler[state].handler(runner, endptr, &op_stack_ptr, 
			                                    begin_op_stack, &num_stack_ptr);


	*result = *begin_num_stack;

	FREE(begin_op_stack)
	FREE(begin_num_stack)

	return current_status;
}


/*******************************************************************************
******************************* Operator Functions ****************************/

static double Add(double num1, double num2)
{
	return (num1 + num2);
}

static double Sub(double num1, double num2)
{
	return (num1 - num2);
}

static double Div(double num1, double num2)
{
	return (num2 / num1);
}
static double Multi(double num1, double num2)
{
	return (num1 * num2);
}
static double Pow(double num1, double num2)
{
	return pow(num2, num1);
	return 0;
}

static double Dummy(double num1, double num2)
{
	return 0;
}