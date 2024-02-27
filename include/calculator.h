/*********************************************************************
* Description: API for calculator
* Name: HRD20
* Date: 6/10/2021
* Versions: 1.0 - Upon Approval
*********************************************************************/
#ifndef __CALC_H__
#define __CALC_H__

/*******************************************************************************
* Function Description: This function recieve a string with mathmetical expression and compute 
                        its value. 
			       
* Arguments: str: mathematical expression 
            result: reference to the variable that store the result of the calculation. 

* Return value:  The calculator returns status which can be one of the next:
	                SUCCESS,
	                SYNTAX_ERROR, 
	                MATH_ERROR, 
	                SYS_ERROR
   
* Notes:Number of operands in the string should not exceed 256.
		Number of operators in the string should not exceed 256.
        Brackets () are also supported.
	
		An expression consists of numbers and operators
		Binary operators : -, +, *, /
		Unary operators : -, +
		numbers : integers and floating numbers (including double)
		
        
* Time Complexity: Time complexity O(n) n = length of string recieved , Space Complexity O(1)
*******************************************************************************/
int Calculate(const char *str, double *result);


#endif /* __CALC_H__ */