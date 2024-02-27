/******************************************************************************
* Project: memory managment                                                   *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
******************************************************************************/

#include <stdlib.h> /* free, malloc */
#include <stdio.h>  /* printf, puts */

#include "utils.h"

/* test functions */
void TextAddress(void);
void DataAddress(void);
void HeapAddress(void);
int *StackAddress(int var_1, char var_2, long double var_3, int *var_4);
int *StackAddress2(int var_1, char var_2, long double var_3, int *var_4);


/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void BeginingOfTest(const char *tests_name);
static void EndOfTests(void);

/* functions for the tests */
void Dummy1(void);
int Dummy2(void);
static int Dummy3(void);

/* global variable */ 
int g_a = 5;
int *g_b = &g_a;  
int g_c;
int *g_d; 
int *mall;

 

/*******************************************************************************
********************************* The Main ************************************/

int main (int argc, char **argv)
{  
    int var_1 = 5; 

    
    int *e = &var_1;
    const char *tests_name = "Memory Managment Tests";
    char var_2 = 'a';
    long double var_3 = 7.07; 
    int *var_4 = &var_1;

    printf("\n      argc address  =   %p      (%lu)\n", &argc, (size_t)&argc - (size_t)&main);

    printf("\n      argv address  =   %p      (%lu)\n", &argv, (size_t)&argv - (size_t)&main);

    printf("      main address  =   %p      (%lu)\n", &var_2, (size_t)&var_2 - (size_t)&main);

    BeginingOfTests(tests_name);

    /* The main test functions */
    TextAddress(); 
    DataAddress();
    HeapAddress();
    e = StackAddress(var_1, var_2, var_3, var_4);


    printf("      e return address  =   %p      (%lu)\n", &e, (size_t)&e - (size_t)&main);

    EndOfTests();
    
    return 0; 
}

/*******************************************************************************
********************************** Text Address *******************************/

void TextAddress(void)
{

    const char *test_name = "Address In The Text Segment"; 

    Foo1();
    Bar1();

    BeginingOfTest(test_name);

    printf(GREEN);

    printf("      main address  =   %p      (%lu)\n", &main, (size_t)&main - (size_t)&main);

    printf("      Bar1 address  =   %p      (%lu)\n", &Bar1, (size_t)&Bar1 - (size_t)&main);
    printf("      Foo1 address  =   %p      (%lu)\n", &Foo1, (size_t)&Foo1 - (size_t)&main);

    printf("       int address  =   %p      (%lu)\n", &Dummy2, (size_t)&Dummy2 - (size_t)&main);

    printf("    void 1 address  =   %p      (%lu)\n", &Dummy1, (size_t)&Dummy1 - (size_t)&main);
    printf("    void 2 address  =   %p      (%lu)\n", &StackAddress, (size_t)&StackAddress - (size_t)&main);

    printf("  static 1 address  =   %p      (%lu)\n", &Dummy3, (size_t)&Dummy3 - (size_t)&main);
    printf("  static 2 address  =   %p      (%lu)\n", &BeginingOfTests, (size_t)&BeginingOfTests - (size_t)&main);
    printf("  static 3 address  =   %p      (%lu)\n", &BeginingOfTest, (size_t)&BeginingOfTest - (size_t)&main);
    printf("  static 4 address  =   %p      (%lu)\n", &EndOfTests, (size_t)&EndOfTests - (size_t)&main);


    printf(WHITE);
}

/******************************** Dummy func **********************************/

void Dummy1(void)
{
    ;
}

int Dummy2(void)
{
    return 0;
}

static int Dummy3(void)
{
    return 0;
}

/*******************************************************************************
********************************* Data Address *******************************/

void DataAddress(void)
{

    const char *test_name = "Address In The Data (DS + BSS) Segment";
    
    char *a= "eva";

    int static b2 = 0; 
    int static *c2 = NULL;

    static int b1 = 756; 
    static int *c1 = &b1;

    static int d; 
    static int *e;
 

    BeginingOfTest(test_name);

    printf(GREEN);

    printf("      const string 1 address  =   %p      (%lu)       (%lu)     DS \n", test_name, (size_t)test_name - (size_t)&main,  (size_t)test_name - (size_t)test_name);
    printf("      const string 2 address  =   %p      (%lu)       (%lu)    DS \n\n", a, (size_t)a - (size_t)&main,  (size_t)a - (size_t)test_name);

    printf("          global var address  =   %p      (%lu)      (%lu)  DS\n", &g_a, (size_t)&g_a - (size_t)&main,  (size_t)&g_a - (size_t)test_name);
    printf("          global ptr address  =   %p      (%lu)      (%lu)  DS\n", &g_b, (size_t)&g_b - (size_t)&main,  (size_t)&g_b - (size_t)test_name);

    printf("          static var address  =   %p      (%lu)      (%lu)  DS\n", &b1, (size_t)&b1 - (size_t)&main,  (size_t)&b1 - (size_t)test_name);
    printf("          static ptr address  =   %p      (%lu)      (%lu)  DS\n", &c1, (size_t)&c1 - (size_t)&main,  (size_t)&c1 - (size_t)test_name);

    printf("          static var address  =   %p      (%lu)      (%lu)  DS\n", &b2, (size_t)&b2 - (size_t)&main,  (size_t)&b2 - (size_t)test_name);
    printf("          static ptr address  =   %p      (%lu)      (%lu)  DS\n\n", &c2, (size_t)&c2 - (size_t)&main,  (size_t)&c2 - (size_t)test_name);


    printf("   static var uninit address  =   %p      (%lu)      (%lu)  BSS \n", &d, (size_t)&d - (size_t)&main,  (size_t)&d - (size_t)test_name);
    printf("   static ptr uninit address  =   %p      (%lu)      (%lu)  BSS \n", &e, (size_t)&e - (size_t)&main,  (size_t)&e - (size_t)test_name);
    printf("   global var uninit address  =   %p      (%lu)      (%lu)  BSS \n", &g_c, (size_t)&g_c - (size_t)&main,  (size_t)&g_c - (size_t)test_name);
    printf("   global ptr uninit address  =   %p      (%lu)      (%lu)  BSS \n", &g_d, (size_t)&g_d - (size_t)&main,  (size_t)&g_d - (size_t)test_name);

    printf(WHITE);
}


/*******************************************************************************
********************************* Heap Address *******************************/

void HeapAddress(void)
{

    const char *test_name = "Address In The Heap Segment";

    mall = (int *)malloc(35);
 
    BeginingOfTest(test_name);

    printf(GREEN);

    printf("      malloc address  =   %p      (%lu)       (%lu)     Heap \n", mall, (size_t)mall - (size_t)&main, (size_t)mall - (size_t)mall);

    printf(WHITE);
}
/*******************************************************************************
********************************* Stack Address *******************************/
int *StackAddress(int var_1, char var_2, long double var_3, int *var_4)
{

    const char *test_name = "Address In The Stack Segment";

    char *eva = "eva";

    long double j = 10.7;
    long b1 = 756;  
    double e1 = 7.7;
    long b2 = 756; 
    double e2 = 7.7;

    char n3[300] = "eva12111333444";
    int g3[300] = {1,2,3,4,5};
    int g1[5] = {1,2,3,4,5};
    char n1[5] = "eva12";
    int g2[50] = {1,2,3,4,5};
    char n2[50] = "eva1299999999999999000000000000000000";

    int a1 = 1;
    int a2 = 1;
    int a3 = 1;
    short h = 7;

    size_t f = 70;


    char c = 5; 
    const char d = 5;

    double *ptr_e1 = &e1;
    int *ptr_a1 = &a1;
    char *ptr_c = &c; 
    const char *ptr_d = &d;

    const char *ptr_n1 = n1;



    BeginingOfTest(test_name);

    printf(GREEN);
    printf("       var_4 address  =   %p      (%lu)      (%lu)     ptr    (8 byte) (%lu)\n", &var_4, (size_t)&var_4 - (size_t)&main, (size_t)&var_4 - (size_t)&var_4, (size_t)&var_4 - (size_t)mall);
    printf("       var_2 address  =   %p      (%lu)      (%lu)     char   (1 byte) \n", &var_2, (size_t)&var_2 - (size_t)&main, (size_t)&var_2 - (size_t)&var_4);
    printf("       var_1 address  =   %p      (%lu)      (%lu)    int    (4 byte) \n\n", &var_1, (size_t)&var_1 - (size_t)&main, (size_t)&var_1 - (size_t)&var_4);
    
    printf("        char address  =   %p      (%lu)      (%lu)    char   (1 byte) \n", &c, (size_t)&c - (size_t)&main, (size_t)&c - (size_t)&var_4);
    printf("  const char address  =   %p      (%lu)      (%lu)    char   (1 byte) \n\n", &d, (size_t)&d - (size_t)&main, (size_t)&d - (size_t)&var_4);

    printf("       short address  =   %p      (%lu)      (%lu)    short  (2 byte) \n", &h, (size_t)&h - (size_t)&main, (size_t)&h - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n", &a1, (size_t)&a1 - (size_t)&main, (size_t)&a1 - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n", &a2, (size_t)&a2 - (size_t)&main, (size_t)&a2 - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n\n", &a3, (size_t)&a3 - (size_t)&main, (size_t)&a3 - (size_t)&var_4);

    printf("l string ptr address  =   %p      (%lu)      (%lu)    litrel string ptr  (8 byte) \n", &test_name, (size_t)&test_name - (size_t)&main, (size_t)&test_name - (size_t)&var_4);
    printf("l string ptr address  =   %p      (%lu)      (%lu)    litrel string ptr  (8 byte) \n\n", &eva, (size_t)&eva - (size_t)&main, (size_t)&eva - (size_t)&var_4);
    

    printf("        long address  =   %p      (%lu)      (%lu)\n", &a3, (size_t)&a3 - (size_t)&main, (size_t)&a3 - (size_t)&var_4);
    printf("        long address  =   %p      (%lu)      (%lu)\n", &b1, (size_t)&b1 - (size_t)&main, (size_t)&b1 - (size_t)&var_4);
    printf("      double address  =   %p      (%lu)      (%lu)\n", &e1, (size_t)&e1 - (size_t)&main, (size_t)&e1 - (size_t)&var_4);
    printf("        long address  =   %p      (%lu)      (%lu)\n", &b2, (size_t)&b2 - (size_t)&main, (size_t)&b2 - (size_t)&var_4);
    printf("      double address  =   %p      (%lu)      (%lu)\n", &e2, (size_t)&e2 - (size_t)&main, (size_t)&e2 - (size_t)&var_4);
    printf("      size_t address  =   %p      (%lu)      (%lu)\n\n", &f, (size_t)&f - (size_t)&main, (size_t)&f - (size_t)&var_4);

    printf("  double ptr address  =   %p      (%lu)      (%lu)    ptr\n", &ptr_e1, (size_t)&ptr_e1 - (size_t)&main, (size_t)&ptr_e1 - (size_t)&var_4);
    printf("     int ptr address  =   %p      (%lu)      (%lu)    ptr\n", &ptr_a1, (size_t)&ptr_a1 - (size_t)&main, (size_t)&ptr_a1 - (size_t)&var_4);
    printf("    char ptr address  =   %p      (%lu)      (%lu)   ptr\n", &ptr_c, (size_t)&ptr_c - (size_t)&main, (size_t)&ptr_c - (size_t)&var_4);
    printf("    char ptr address  =   %p      (%lu)      (%lu)   ptr\n", &ptr_d, (size_t)&ptr_d - (size_t)&main, (size_t)&ptr_d - (size_t)&var_4);
    printf("  string ptr address  =   %p      (%lu)      (%lu)   ptr\n\n", &ptr_n1, (size_t)&ptr_n1 - (size_t)&main, (size_t)&ptr_n1 - (size_t)&var_4);

        printf(" long double address  =   %p      (%lu)      (%lu)  long double (16 byte) \n\n", &j, (size_t)&j - (size_t)&main, (size_t)&j - (size_t)&var_4);

    printf("     int arr address  =   %p      (%lu)      (%lu)   arr\n", &g1, (size_t)&g1 - (size_t)&main, (size_t)&g1 - (size_t)&var_4);
    printf("     int arr address  =   %p      (%lu)      (%lu)   arr\n", &g2, (size_t)&g2 - (size_t)&main, (size_t)&g2 - (size_t)&var_4);
    printf("     int arr address  =   %p      (%lu)      (%lu)   arr\n\n", &g3, (size_t)&g3 - (size_t)&main, (size_t)&g3 - (size_t)&var_4);

    printf("      string address  =   %p      (%lu)      (%lu)  string\n", &n1, (size_t)&n1 - (size_t)&main, (size_t)&n1 - (size_t)&var_4);
    printf("      string address  =   %p      (%lu)      (%lu)  string\n", &n2, (size_t)&n2 - (size_t)&main, (size_t)&n2 - (size_t)&var_4);
    printf("      string address  =   %p      (%lu)      (%lu)  string\n\n", &n3, (size_t)&n3 - (size_t)&main, (size_t)&n3 - (size_t)&var_4);

    printf("       var_3 address  =   %p      (%lu)      (%lu)  long double   (16 byte) \n\n", &var_3, (size_t)&var_3 - (size_t)&main, (size_t)&var_3 - (size_t)&var_4);

    printf(WHITE);

    StackAddress2(var_1, var_2,var_3, var_4);

    return (&a3);
}

/******************************* Stack Address 2 ******************************/

int *StackAddress2(int var_1, char var_2, long double var_3, int *var_4)
{

    int a1 = 1;
    int a2 = 1;
    int a3 = 1;
    short h = 7;

    printf(GREEN);
    printf("       var_4 address  =   %p      (%lu)      (%lu)     ptr    (8 byte) \n", &var_4, (size_t)&var_4 - (size_t)&main, (size_t)&var_4 - (size_t)&var_4);
    printf("       var_2 address  =   %p      (%lu)      (%lu)     char   (1 byte) \n", &var_2, (size_t)&var_2 - (size_t)&main, (size_t)&var_2 - (size_t)&var_4);
    printf("       var_1 address  =   %p      (%lu)      (%lu)    int    (4 byte) \n\n", &var_1, (size_t)&var_1 - (size_t)&main, (size_t)&var_1 - (size_t)&var_4);


    printf("      return address  =   %p\n\n" , ((char *)&var_1 + 4));
    printf("       short address  =   %p      (%lu)      (%lu)    short  (2 byte) \n", &h, (size_t)&h - (size_t)&main, (size_t)&h - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n", &a1, (size_t)&a1 - (size_t)&main, (size_t)&a1 - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n", &a2, (size_t)&a2 - (size_t)&main, (size_t)&a2 - (size_t)&var_4);
    printf("         int address  =   %p      (%lu)      (%lu)    int    (4 byte) \n\n", &a3, (size_t)&a3 - (size_t)&main, (size_t)&a3 - (size_t)&var_4);

    printf("       var_3 address  =   %p      (%lu)      (%lu)  long double   (16 byte) \n\n", &var_3, (size_t)&var_3 - (size_t)&main, (size_t)&var_3 - (size_t)&var_4);
    printf(WHITE);

    return (&var_1);
}


/*******************************************************************************
******************************* Begining Of Tests *****************************/

static void BeginingOfTests(const char *tests_name)
{
    printf(YELLOW);
    printf("\n____________________ %s ____________________\n\n", tests_name);
}

/*******************************************************************************
******************************* Begining Of Test ******************************/

static void BeginingOfTest(const char *tests_name)
{
    printf(WHITE);
    printf("\n\n > %s                   \n\n", tests_name);
    
}

/*******************************************************************************
********************************** End Of Tests *******************************/

static void EndOfTests(void)
{
    const char *end = "End Of Tests";

    printf(YELLOW);
    printf("\n__________________________ %s ________________________\n\n", end);
}
