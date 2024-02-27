#include <stdio.h> /* printf */

typedef struct Node Node_ty;

struct Node
{
	Node_ty* next; 
};

typedef struct stack stack_ty;

struct stack
{
	int *array;   
	size_t size;
	size_t capacity;
};

void StringTest(void );
void FlipListTest(void);

int Strlen(char *str,int counter);
int Strcmp(char *str1, char *str2, int comper_status);

stack_ty* SortStack(stack_ty* _stack);
stack_ty* void SortedInsert(stack_ty* _stack, int num);

stack_ty *StackCreate(size_t capacity);
void StackDestroy(stack_ty *_stack);
void StackPush(stack_ty *_stack, int *num);
void StackPop(stack_ty *_stack);
void *StackPeek(const stack_ty *_stack);
int StackIsEmpty(const stack_ty *_stack);



int main()
{
	/*StringTest();*/
	FlipListTest();

	return 0;
}

/*******************************************************************************
******************************** Fibonacci Recursion **************************/

int FibonacciRecursion(unsigned int num)
{
   if (num <= 1)
   {
      return num;
   }

   return FibonacciRecursion(n-1) + FibonacciRecursion(n-2);
}

/*******************************************************************************
******************************* Fibonacci No Recursion ************************/

int FibonacciNoRecursion(unsigned int num)
{
  int fibonacci[3] = {0};  /* TODO not in array only 3 */
  int i = 0;
 
  /* Initialize the first two case 0th & 1st */
  fibonacci_array[0] = 0;
  fibonacci_array[1] = 1;
 
  for (i = 2; i <= num; i++)
  {
      fibonacci_array[i] = fibonacci_array[i-1] + fibonacci_array[i-2];
  }
 
  return fibonacci_array[num];
}


/*******************************************************************************
************************************ Flip List ********************************/

Node_ty* FlipList(Node_ty * node)
{
	Node_ty *new_begin = NULL;
	Node_ty *next_node = NULL;

	assert(node);

	if(NULL == node->next)
	{
		return node; 
	}

	new_begin = FlipList(node->next);

	next_node = node->next;
	next_node->next = node;

	node->next = NULL;

	return(new_begin);
}

/*******************************************************************************
********************************* Sort Int Stack ******************************/
 
stack_ty* SortStack(stack_ty* _stack)
{
	int num = 0; 

    if (!StackIsEmpty(_stack)) 
    {
        num = StackPeek(_stack);

        StackPop(_stack);

        SortStack(_stack);
 
        SortedInsert(_stack, num);
    }

    return _stack;
}

stack_ty* void SortedInsert(stack_ty* _stack, int num)
{
	int bigger_num = 0; 

    if (!StackIsEmpty(_stack)|| num > StackPeek(_stack))
    {
        StackPush(_stack, num);

        return _stack;
    }

    bigger_num = StackPeek(_stack);
 
    StackPop(_stack);

    SortedInsert(_stack, num);

    StackPush(_stack, bigger_num);

    return _stack;
}

/*******************************************************************************
************************************* strlen **********************************/

int Strlen(char *str,int counter)  /* TODO wrepper*/
{
	if('\0' == *str)
	{
		return counter;
	}

	return(Strlen(++str, counter + 1));
}


/*******************************************************************************
************************************* strcmp **********************************/

int Strcmp(char *str1, char *str2, int comper_status)  /* TODO */ 
{
	if('\0' == *str1 || '\0' == *str2 )
	{
		return comper_status;
	}

	if(*str1 != *str2)
	{
		return (*str1 - *str2);
	}

	return(Strcmp(++str1, ++str2, comper_status));
}

/*******************************************************************************
************************************* StrCpy **********************************/

char* StrCpy(char *dest, char *src)  /* TODO counst */
{
	if('\0' == *src)
	{
		*dest = *src;
		return dest;
	}

	*dest = *src;

	return(StrCpy(++dest, ++src));
}

/*******************************************************************************
************************************* StrCat **********************************/

char* StrCat(char *dest, char *src)
{
	char *dest_runner = dest;

	dest_runner  = dest + Strlen(dest);

	StrCpy(dest_runner, src);

	return (dest);
}


/*******************************************************************************
************************************* StrStr **********************************/

char* StrStr(char *haystack, char *needle)
{
	char *needle_runner = needle;
	char *haystack_runner = needle;

	while('\0' != *needle_runner  && *needle_runner  == *haystack_runner)
	{
		++needle_runner;
		++haystack_runner;
	}

	if('\0' == *needle_runner)
	{
		return haystack_runner;
	}

	return(StrStr(haystack, needle));
}

/******************************************************************************/







/*******************************************************************************
************************************** Tests **********************************/

void StringTest(void )
{
	char str[100] = "smoOOOOopysmoOOOOopysmoOOOOopysmoOOOOopysmoOOOOopy";

	char str1[10] = "smonpy";
	char str2[10] = "smoopy";

	int comper_status = 0;

	int counter = 0; 

	counter = Strlen(str, counter);

	printf("counter = %d\n ", counter);

	comper_status = Strcmp(str1, str2, comper_status);

	printf("comper_status = %d\n ", comper_status);
}

void FlipListTest(void)
{
	Node_ty *begin = NULL; 

	Node_ty A = {0};
	Node_ty B = {0};
	Node_ty C = {0};
	Node_ty D = {0};

	begin =  &A;
	A.next = &B;
	B.next = &C;
	C.next = &D;
	D.next = NULL;

	while(NULL != begin)
	{
		printf("\nnode = %p\n", (void *)begin);
		
		begin = begin->next; 
	}

	begin =  &A;


	begin = FlipList(begin);

	while(NULL != begin)
	{
		printf("\nnode = %p\n", (void *)begin);
		
		begin = begin->next; 
	}

}

/*******************************************************************************
******************************** Stack Create *********************************/

stack_ty *StackCreate(size_t capacity)
{
	stack_ty *stack = (stack_ty *)malloc(sizeof(stack_ty)); 
	stack->array = malloc(capacity, int);
	stack->size = 0;
	stack->capacity = capacity;
	
	return stack;
}

/*******************************************************************************
******************************* Stack Destroy *********************************/

void StackDestroy(stack_ty *_stack)
{
	free(stack->array);
	free(stack);
}

/*******************************************************************************
********************************** Stack Push *********************************/

void StackPush(stack_ty *_stack, int *num)
{
	assert(stack->capacity > stack->size);
	stack->array[stack->size] = num;
	++stack->size;
}


/*******************************************************************************
********************************** Stack Pop **********************************/

void StackPop(stack_ty *_stack)
{
	--stack->size;
	stack->array[stack->size] = NULL;
}

/*******************************************************************************
******************************** Stack Peek ***********************************/

void *StackPeek(const stack_ty *_stack)
{
	return(stack->array[stack->size - 1]);
}


/*******************************************************************************
******************************* Stack Is Empty ********************************/

bool_ty StackIsEmpty(const stack_ty *_stack)
{
	return (stack->size == 0 ? 1 : 0);
}
