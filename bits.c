/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  //数字逻辑异或的与非转换
  return ~((~((~x)&y))&(~(x&(~y))));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
   //最小的数是仅有最高有效位为1的数
  return 1<<31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  //返回真值,关键在于将满足条件的数(即最大数)唯一地处理成0
  //利用xor比较是否相等
  //最大数+1和取反相同:!((~x)^(x+1));  但是-1同样有这一性质,要扣掉
  //利用!将数值转换为布尔值,再结合&实现&&的功能
  //如果有移位操作:!((~x)^(1<<31))
  int x_plus_1 = x + 1;
  return (!((~x) ^ (x_plus_1)))&(!!(x_plus_1));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  //类似掩码吧
  int all = 0xAA+(0xAA<<8)+(0xAA<<16)+(0xAA<<24);
  return !((x&all)^all);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  //负数补码:绝对值取反+1
  //反过来正数求相反数:-1再取反
  //不过~(x-1)==(~x)+1
  return (~x)+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  //该范围的有效位位:110000-111001
  //前两位及更高位固定不变,单独异或验证
  //末4位采用4位溢出的方式验证
  int bool1 = (x>>4)^3;
  int last_byte = x&15;
  int bool2 = (last_byte+6)&16;
  return !bool1&!bool2;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  //将x转换成0和-1,与x,y进行&.(1会清空高位,选用-1保留所有位)
  //从flag1->flag2: 0异或任何数等于任何数,-1异或-1为0.实现0和-1的转化
    int flag1 = (~!!x) + 1;
    int flag2 = flag1 ^ (~0);
    return (flag1 & y) | (flag2 & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  //若不考虑溢出,则检测x-y的符号位
  //考虑溢出,因为溢出仅在x负y正和x正y负的情况下出现,而这两种情况又可直接比较大小,单独设置flag判断
  int mask = 1<<31;
  int var1 = !(x&mask);
  int var2 = !(y&mask);
  int flag1 = !var1&var2;
  int flag2 = !var2&var1;
  int x_sub_y = x+(~y)+1;
  int negate_SF = !((mask)&x_sub_y);
  //感觉应该能化简..
  return (!negate_SF|!x_sub_y|flag1)&!flag2;

}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // (-x|x),若不为零,该值符号位必为1.
    return (~((((~x)+1)|x)>>31)+1)^1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  //二分法,若为负找最高0,若为正找最高1
    int flag, cnt_16, cnt_8, cnt_4, cnt_2, cnt_1, cnt_0;
    int sign = x >> 31;
    x = (sign & (~x)) | (~sign & (x));
    flag = !!(x >> 16);
    cnt_16 = flag << 4;
    x >>= (cnt_16);
    flag = !!(x >> 8);
    cnt_8 = flag << 3;
    x >>= (cnt_8);
    flag = !!(x >> 4);
    cnt_4 = flag << 2;
    x >>= cnt_4;
    flag = !!(x >> 2);
    cnt_2 = flag << 1;
    x >>= cnt_2;
    flag = !!(x >> 1);
    cnt_1 = flag;
    x >>= cnt_1;
    cnt_0 = x;
    return 1 + cnt_0 + cnt_1 + cnt_2 + cnt_4 + cnt_8 + cnt_16;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) 
{
  //分别处理三部分,*2就是exp+1;
    unsigned exp = (uf << 1 >> 24);
    unsigned frac = uf << 9 >> 9;
    int mask = uf >> 31<<31;
    if (exp == 255 && frac)
        return uf;
    if (exp >= 254)
        return mask| 0x7f800000;
    if (exp == 0)
        return mask | uf << 1;
    exp++;
    return mask|(exp<<23)|frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
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
int floatFloat2Int(unsigned uf) {
    int exp = (uf >> 23) & 0xff;
    int sign = (uf >> 31) & 1;
    int frac = uf & 0x7fffff;
    int shiftBits = 0;
    // 0比较特殊，先判断0(正负0都算作0)
    if (!(uf & 0x7fffffff))
        return 0;
    // 判断是否为NaN还是无穷大
    if (exp == 0xff)
        return 0x80000000u;
    // 指数减去偏移量，获取到真正的指数
    exp -= 127;
    // 需要注意的是，原来的frac一旦向左移位，其值就一定会小于1，所以返回0
    if (exp < 0)
        return 0;
    // 获取M，注意exp等于-127和不等于-127的情况是不一样的。当exp != -127时还有一个隐藏的1
    if (exp != -127)
        frac |= (1 << 23);
    // 要移位的位数。注意float的小数点是点在第23位与第22位之间
    shiftBits = 23 - exp;
    // 需要注意一点，如果指数过大，则也返回0x80000000u
    if (shiftBits < 31 - 23)
        return 0x80000000u;
    // 获取真正的结果
    frac >>= shiftBits;
    // 判断符号
    if (sign == 1)
        return ~frac + 1;
    else
        return frac;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    // 判断指数是否上溢或者下溢
    int exp = x + 127;
    if(exp > 0xfe)
        return 0x7f800000;
    else if(exp < 0)
        return 0;
    return exp << 23;
}

