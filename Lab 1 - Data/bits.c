/* 
 * CS:APP Data Lab 
 * 
 * Hunter Casillas (casillas)
 * Tanner Ostenson (tjo222)
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
/* Rating 1 -- 2 points each */
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
	/* In binary 5 is 1010 with a 1 at each even power of two.
	   Ox55 is 10101010 in binary which represents one byte.
	   You shift 0x55 into every hex spot and then combine them to get 0x55555555.
	   This will give you a 32 bit return value with all even-numbered bits set to 1. 
	*/
	int one = 0x55;
	int two = 0x55<<8;
	int three = 0x55<<16;
	int four = 0x55<<24;
	
	return four|three|two|one; 
}
/* 
 * fitsShort - return 1 if x can be represented as a 
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
	/* We know that a short is 16 bits. So only integers that can be represented in <= 16 bits can be a short.
	   If we take the integer in binary, and first left shift it followed by a right shift, only integers less than 16 bits will remain unchanged.  
	   If the integer is greater than 16 bits and we left and right shift it, the integer will be different when returned. 
	   Using "^", if the value was unchanged it will return 0, but if the value is not the same it will return 1. 
	   Using "!" gives the desired result. 
	*/
	return !(((x << 16) >> 16) ^ x); 	 
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
	/* Taking the bitwise NOT, which inverts the bits of its operand, and applying it to 0 changes the value to 1.
	   Since integers are inherently signed, the most significant bit will signify the value as a negative.
	   So after everything, it returns -1.
	*/
	return ~0;	
}
/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
	/* The first operand "!!" will turn whatever n was into a single 1. Left shifting that 31 bits will yeild a
	   1 followed by 31 zeroes. Because we know ~0 is the same as -1, n + ~0 is the same as n-1, so we then
	   right shift it n-1 bits. Since x has a leading 1, the bits filled in with the right shift are also 1's.
	*/
	int x = !!n << 31;
	
	return x >> (n + ~0);  
}
/* Rating 2 -- 3 points each */
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
	/* n << 3 will yield the same result as n*8 and by shifting x right that many, we get the byte at the index we want to be the final byte.
	   Then, by "&" it with 0xff (or 0x000000ff) we make sure we only get the last byte.
	*/
	return (x >> (n<<3)) & 0xff;
}
/* 
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
	/* Since !x will give you 0 for all inputs except for 0 and !!y will give you 1 for all cases except for 0, by "|" them together,
	   it will return 1 for all cases except for 1->0 which is not implied and therefore false.
	*/
	return !x|(!!y);    
}
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
	/* First, x ^ ~y will only yield 1 if the two are equal. Then, the preceding ~ handles the case when y = 0, finally 
	   the !! will make sure either 0 (if x and y were equal) or 1 (if they weren't equal).
	*/
	return (!!(~(x ^ ~y)));
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
	/* To determine if x is negative, we shift it right 31, meaning neg will contain the most significant bit. 
	   Therefore neg will contain -1 if negative and 0 if it is not.
	   To determine if x is positive, we !!x which will return 1 if it is positve and 0 if it's 0. 
	   By "OR"ing neg and pos together if neg is -1 the final result will be -1.
	   If neg and pos are both 0 the result will be 0.
	   Finally if pos is 1, the result will be 1.
	*/
	int neg = x >> 31;
	int pos = !!x;
	
	return neg|pos;
}
/* Rating 3 -- 2 points each */
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	/* First we !!x which makes x 1 for all cases, unless x is 0 in which case it is 0. 
	   f will hold 0 if x was 0, or 16 if x was 1. 
	   t will hold 0 if x was 1, or 16 if x was 0.
	   y will not be shifted if the original x was 1, otherwise it's shifted 32, meaning it will be 0.
	   The same goes for z, but only if x was 0 originally; therefore, when | is used on them, only
	   the one that wasn't shifted will return.
	*/
	x = !!x;
	int f = x << 4;
     	int t  = 17 + ~f;
     	y = (y << t) << t;
     	z = (z << f) << f;
     
     	return y|z;  
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
	/* First, the variable labled same will return 0 if x and y are the same.
	   Then, sum calculates the same thing as x - y (without using minus).
	   xor will handle negative cases.
	   The return statement will make the most significant bit a 0 if the two integers were equal or less than, and returns
	   a 1 if x is greater than y. 
	*/
	int xor;
	int sum;
	int same;
	
  	xor = x ^ y;
  	same = xor & y;
  	sum = ~x + 1 + y;

  	return 1&(((sum & ~xor)|same)>>31);
}
/* Rating 4 -- 1 point each */
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
	/* Sign will give us -1 if it is a negative number and 0 in all other cases.
	   "&"ing that with compliment will give us 0 if positive and the original number if it's negative. 
	   Taking the negative number and adding together twice will give us the positive value of x.
	   Or if the value was positive, together will be 0 and therefore give us the original value of x.  
	*/
	int negative = ((x >> 31) & 1);	
	int sign = (~(negative)) + 1;
	int compliment = (~x) + 1;
	int together = sign & compliment;
	
	return x + together + together;  
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
	/* 5 masks are required to count a 32 bit integer. Each mask follows a pattern which, when "&"ed will give us the count of bits.  
	   0x55 has every other position = 1.
	   0x33 has every two positions = 1.
	   0xf has every four positions = 1.
	   0xff shifted left 16 has every 8 positions = 1.
	   0xff shifted left 8 has every 16 positions = 1.
	   This method is called population count. 
	   By "&"ing the original number through each of these masks, every bit will be 0 except the ones representing the count of how many
	   1's existed in the original integer.
	*/
	int num = 0x55 + (0x55 << 8);
    	int mask = num + (num << 16);
    	x = (x & mask) + ((x >> 1) & mask);
    
    	num = 0x33 + (0x33 << 8);
    	mask = num + (num << 16);
    	x = (x & mask) + ((x >> 2) & mask);
    
    	num = 0xf + (0xf << 8);
    	mask = num + (num << 16);
    	x = (x & mask) + ((x >> 4) & mask);
    
    	mask = 0xff + (0xff << 16);
    	x = (x & mask) + ((x >> 8) & mask);
    
    	mask = 0xff + (0xff << 8);
    	x = (x & mask) + ((x >> 16) & mask);
    
    	return x;  
}
/* Float Rating 2 -- 3 points each */
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
	/* Max is the maximum value of an integer and min is the minimum value of an integer before it is NaN.
	   By "&"ing uf with max ensures that the result is positive, so that when it is compared with min a negative number won't interfere
	   with the result. If the result is greater than the minimum value, we will return uf, otherwise we return result.  
	*/	
	unsigned max = 0x7FFFFFFF;		
  	unsigned min = 0x7F800001;
  	unsigned result = uf & max;		
  	if (result >= min)
    		return uf;
  	else
    		return result;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
 	/* Exponent is the 24th to 32nd bit. Fraction is everything before that.
	   As long as exponent is equal to 0xFF and the fraction is not 0, that means that the float is negative so we return it. 
	   1u shifted left 31 gives us a 1 in the most significant bit position.
	   By "^" uf with (1u << 31) we turn the positive uf into a negative uf and return it.
	*/
	unsigned exponent = (uf >> 23) & 0xFF;
    	unsigned fraction = uf & ((1 << 23) + ~0);
    	if (exponent == 0xFF && fraction)
        	return uf;
    	else 
        	return uf = uf ^(1u << 31); 
}
/* Float Rating 4 -- 1 point each */
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
	return 0;
} 
