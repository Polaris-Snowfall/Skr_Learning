# Lab1:DataLab
用位运算实现一些基本操作
```c
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


```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305111056381.png)
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305111057616.png)

# Lab2:Bomb Lab
拆炸弹,实际上就是逆向找绕过条件.
用IDA感觉直接秒了,不过练一下看汇编吧.
之前还真没有嗯看过汇编.
先根据跳转的地址将代码分成几个部分,从外向内识别每个循环体.
## phase_1
一个简单的字符串比较
payload:'Border relations with Canada have never been better.'
## phase_2
读入六个数字,第一个为1,下一个为上一个的两倍
payload:'1 2 4 8 16 32'
## phase_3
读入两个数字,第一个数字不大于7.
实现了一张跳转表,跳到跳转表的第一个条目,检查第二个数字是否是311.
payload:'1 311'
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305111501156.png)
## phase_4
主要逆这个函数了,可见var2\==arg1的时候拆除炸弹.
payload:'7 0'
```c
func4(arg1,arg2,arg3)
{
    var1 = arg3-arg2;
    var2 = arg2>>31;
    var1 += var2;
    var1 /= 2;
    var2 = var1+arg2;
    if(var2>arg1)
        return 2*func4();
    if(var2<arg1)
        return 2*func4()+1;
    return 0;
}
```
## phase_5
payload:ionefg
```c
phase_5(a1)
{
    //a1即input_string的指针
    var1 = a1;
    if(string_lenth(a1)!=6)
        exploade_bomb();
    //将input_string中每个字符的第四位作为下标,从stringarray中取出字符,拷贝到str数组中.
    i = 0;
    do{
            str[i] = StringArray[a1[i++]&0xf];
    }while(i<6);
    str[6]='\0';
    if(strings_not_equal(str,"flyers"))
        exploade_bomb();
    return 
}

```
解密脚本
```python
def phase_5(goal):
    stringarray = [ch for ch in string.ascii_lowercase + string.ascii_uppercase + string.digits]
    for i in goal:
        for test in stringarray:
            if ord(test)&0xf == i:
                print(test)
                break 

phase_5([9,15,14,5,6,7])
```
## phase_6
函数较长,分成__部分逆向.
代码部分是仅表示逻辑的伪代码,分支和循环尽量还原源码,使用下标代替指针增减.
### 逆向
#### 第一部分
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305121206844.png)
```c
phase_6(a1)
{
    nums[6];
    read_six_numbers(a1,nums);

    i = 0;
    k = 0;
    while(1)
    {
        var1 = nums[k];
        if(--var1>5)
            explode_bomb();
        if(++i == 6)
            break;
        do{
                j = i;
                if(nums[j]==num[k])
                    explode_bomb();
        }while(++j<=5);
            k+=1;
    }
}
```
#### 第二部分
要注意的是,如果循环体的条件在开始时一定成立或为恒真式,汇编代码会省掉(一步)判断,这影响到对while、for等不同循环结构的判断.
如for(int i = 0;i!=24;),while(1);


![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305121500820.png)

```c
phase_6(a1)
{
    //第一部分
    nums[6];
    read_six_numbers(a1,nums);

    i = 0;
    k = 0;
    while(1)
    {
        var1 = nums[k];
        if(--var1>5)
            explode_bomb();
        if(++i == 6)
            break;
        do{
                j = i;
                if(nums[j]==num[k])
                    explode_bomb();
        }while(++j<=5);
            k+=1;
    }

    //第二部分
    last = 7;
    i = 0;
    do{
        nums[i] = 7-nums[i];
    }while(++i!=7);
    for(i = 0; i!=6;++i)
    {
       if(nums[i]>1)
       {
            var3 = 1;
            var2 = nodes;
            do{
                    var2=var2->next;
            }while(++var3!=nums[i]);
       }
       else
       {
            var2 =  nodes;
       }
       arr[i] = var2;
    }
```
#### 第三部分
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305141513827.png)
```c
    i = 1;
    last = 6;
    j = 0;
    while(1)
    {
        var2 = arr[i];
        arr[j]->next = var2;
        j = i;
        if(++i==last)
            break;
    }
    arr[5]->next = NULL;
    i = 5;
    var3 = arr[0];
    do{
        if(*var3<*(var3->next))
            explode_bomb();
        var3 = var3->next;
    }while(--i);
}
```
### 分析
整个流程:
第一部分读入六个数字到栈上的nums数组,检查每个数字都不大于6且各不相等.
(其实就是读1-6的数字排列)
第二部分将nums数组中每个数num = 7-num.并以此在栈上的arr指针数组中存放对应数字的结点指针.
第三部分根据arr指针数组的顺序重构nodes链表.最后遍历链表检查链表中值是否为递减排序.
递增排序nodes链表:"3 4 5 6 1 2",故payload:"4 3 2 1 6 5"
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305141525078.png)
## secret_phase
呃呃有一个隐藏关卡.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305141553892.png)
发现进入条件是从0x603870读取两个数字和一个字符串,且字符串要为"DrEvil".查看0x603870内容发现为phase_4的payload(逆一下read_line函数也能算出来),即只需该phase_4的payload为"7 0 DrEvil"即可进入.

读入一个小于1000的数字.进入fun7.
一个递归,观察ptr可以发现是一个二叉排序树的根节点.最后要使返回值为2.num应为22.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305141656566.png)

```c
fun7(ptr,num)
{
    if(!ptr)
        return -1;
    if(*ptr>num)
    {
        return 2*fun7(*(ptr+8),num);
    }
    if(*ptr!=num)
    {
        return 2*fun7(*(ptr+16),num)+1;
    }
        return 0;
    
}
```
# Lab3:Attack Lab
这个就懒得做了,Linux下基本的ROP和shellcode还是比较熟的.

# Lab4:Architecture Lab
说实话处理器这一章本来就看的云里雾里的.
## Part A
### sum_list
```
            .pos 0
            irmovq stack,%rsp
            call main
            halt


            .align 8
ele1:
            .quad 0x00a
            .quad ele2
ele2:       
            .quad 0x0b0
            .quad ele3
ele3:
            .quad 0xc00
            .quad 0
main:
            irmovq ele1,%rdi
            call sum_list
            pushq %rax
            ret
sum_list:
            xorq %rax,%rax
            jmp test
test:
            andq %rdi,%rdi
            jne loop
            ret
loop:
            mrmovq (%rdi),%rbx
            addq %rbx,%rax
            mrmovq 8(%rdi),%rdi
            jmp test
            
            .pos 0x200
stack:

```
### rsum_list
```
            .pos 0
            irmovq stack,%rsp
            call main
            halt

            .align 8
ele1:
            .quad 0x00a
            .quad ele2
ele2:
            .quad 0x0b0
            .quad ele3
ele3:
            .quad 0xc00
            .quad 0
main:
            irmovq ele1,%rdi
            call rsum_list
            pushq %rax
            ret
rsum_list:
            xorq %rbx,%rbx
            pushq %rbx
            andq %rdi,%rdi
            jne else
            xorq %rax,%rax
            popq %rbx
            ret
else:
            mrmovq (%rdi),%rcx
            rmmovq %rcx,(%rsp)
            mrmovq 8(%rdi),%rdi
            call rsum_list
            mrmovq (%rsp),%rbx
            addq %rbx,%rax
            popq %rbx
            ret

            .pos 0x200
stack:            

```
### copy_block
```
            .pos 0
            irmovq stack,%rsp
            call main
            halt

            .align 8
src:
            .quad 0x00a
            .quad 0x0b0
            .quad 0xc00
dest:        
            .quad 0x111
            .quad 0x222
            .quad 0x333
main:
            irmovq $3,%rdx
            irmovq src,%rdi
            irmovq dest,%rsi
            call copy_block
            pushq %rax
            ret
copy_block:
            xorq %rax,%rax
            jmp condition
condition:
            andq %rdx,%rdx
            jne loop
            ret
loop:
            irmovq $8,%r8
            mrmovq (%rdi),%rcx
            addq %r8,%rdi
            rmmovq %rcx,(%rsi)
            addq %r8,%rsi
            xorq %rcx,%rax
            irmovq $1,%r8
            subq %r8,%rdx
            jmp condition

            .pos 0x200
stack:

```
## Part B
根据IADDQ指令执行的六个阶段,添加一下需要用到的sig就行了.
挺简单的,这里就不贴出来了.
## Part C
### iaddq
和Part B一样添加iaddq指令
### 循环展开
先6路+3路循环展开得到CPE9.15、Score27.0
```
# You can modify this portion
	# Loop header
	xorq %rax,%rax		# count = 0;
	andq %rdx,%rdx		# len <= 0?
	jmp test		# if so, goto Done:

# Loop header
    andq %rdx,%rdx      # len <= 0?
    jmp test
Loop:
    mrmovq (%rdi),%r8
    rmmovq %r8,(%rsi)
    andq %r8,%r8
    jle Loop1
    iaddq $1,%rax
Loop1:
    mrmovq 8(%rdi),%r8
    rmmovq %r8,8(%rsi)
    andq %r8,%r8
    jle Loop2
    iaddq $1,%rax
Loop2:
    mrmovq 16(%rdi),%r8
    rmmovq %r8,16(%rsi)
    andq %r8,%r8
    jle Loop3
    iaddq $1,%rax
Loop3:
    mrmovq 24(%rdi),%r8
    rmmovq %r8,24(%rsi)
    andq %r8,%r8
    jle Loop4
    iaddq $1,%rax
Loop4:
    mrmovq 32(%rdi),%r8
    rmmovq %r8,32(%rsi)
    andq %r8,%r8
    jle Loop5
    iaddq $1,%rax
Loop5:
    mrmovq 40(%rdi),%r8
    rmmovq %r8,40(%rsi)
    iaddq $48,%rdi
    iaddq $48,%rsi
    andq %r8,%r8
    jle test
    iaddq $1,%rax  

test:
    iaddq $-6, %rdx         # 先减，判断够不够6个
    jge Loop                # 6路展开
    iaddq $6, %rdx
    jmp test2               #剩下的

L:
    mrmovq (%rdi),%r8
    rmmovq %r8,(%rsi)
    andq %r8,%r8
    jle L1
    iaddq $1,%rax
L1:
    mrmovq 8(%rdi),%r8
    rmmovq %r8,8(%rsi)
    andq %r8,%r8
    jle L2
    iaddq $1,%rax
L2:
    mrmovq 16(%rdi),%r8
    rmmovq %r8,16(%rsi)
    iaddq $24,%rdi
    iaddq $24,%rsi
    andq %r8,%r8
    jle test2
    iaddq $1,%rax
test2:
    iaddq $-3, %rdx         # 先减，判断够不够3个
    jge L
    iaddq $2, %rdx          # -1则不剩了，直接Done,0 剩一个, 1剩2个
    je R0
    jl Done
    mrmovq (%rdi),%r8
    rmmovq %r8,(%rsi)
    andq %r8,%r8
    jle R2
    iaddq $1,%rax
R2:
    mrmovq 8(%rdi),%r8
    rmmovq %r8,8(%rsi)
    andq %r8,%r8
    jle Done
    iaddq $1,%rax
    jmp Done
R0:
    mrmovq (%rdi),%r8
    rmmovq %r8,(%rsi)
    andq %r8,%r8
    jle Done
    iaddq $1,%rax
```
### 消除气泡
注意到程序中的这个操作,会触发加载/使用数据冒险,导致插入一个气泡指令.
所以我们可以一次性复制两个数据,避免加载/使用数据冒险
```
mrmovq (%rdi), %r8
mrmovq 8(%rdi), %r9
rmmovq %r8, (%rsi)
rmmovq %r9, 8(%rsi)
```

消除后得分45.4
# Lab5:Cache Lab
## Part A
模拟Cache的实现.
最开始的时候没有看到实验材料里的这句话,考虑复杂了...
> For this this lab, you should assume that memory accesses arealigned properly,  such that a singlememory  access  never  crosses  block  boundaries.   By  making  this  assumption,  you  can  ignore  therequest sizes in thevalgrindtraces

大概实现了这样的数据结构,实验采取的是LRU策略,可以用链表来组织实现,我这里就用时间戳代替了.(ps:用time(NULL)获取的时间戳不够准确会造成多个行的时间戳相同,使用clock()代替或使用全局变量记录次数.)
> LRU，最近最少使用策略。替换最后一次访问时间最久远的哪一行
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305182207207.png)

```c
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <getopt.h>

typedef struct CacheLine{
    int valid;
    unsigned long long time;
    long long tag;
}CacheLine;

CacheLine** Cache = NULL;
int groupcount = 0;
int blocksize = 0;
int s,E,b,t;
char verbose = 0;
int misses = 0;
int hits = 0;
int evictions = 0;

//因为不需要实际访问数据内容,所以不需要分配2^b字节的数据空间.
//只要地址位于某Set中且标志位相同,则hit



void Cache_Init(int s,int E,int b)
{
    groupcount = pow(2,s);
    blocksize = pow(2,b);
    Cache = (CacheLine**)malloc(sizeof(CacheLine*)*groupcount);
    for(int i = 0;i<groupcount;++i)
    {
        Cache[i] = (CacheLine*)malloc(sizeof(CacheLine)*E);
        for(int j = 0;j<E;++j)
        {
            Cache[i][j].valid = 0;
            Cache[i][j].time = 0;
        }
    }
}

void hit()
{
    if(verbose==1)
        printf(" hit");
    ++hits;
}

void miss()
{
    if(verbose==1)
        printf(" miss");
    ++misses;
}

void eviction()
{
    if(verbose==1)
        printf(" eviction");
    ++evictions;
}

void AccessMemory(long long addr,int size)
{
    //这个地方使用掩码提取而不是直接移位,是因为算术右移会使标记发生变化.
    long long mask;
    //注意制作掩码的时候的常数类型LL
    mask = (1LL << (s + b)) - 1;
    int Setindex = (mask & addr) >> b;
    // mask = (1LL << b) - 1;
    // int blockoffset = mask & addr;
    mask = (1LL << t) - 1;
    long long tag = (addr >> (s + b)) & mask;


    CacheLine* CacheSet = Cache[Setindex];
    int flag = 0;
    unsigned long long LRtime = CacheSet[0].time;
    int LRid = 0;
    for(int i = 0;i<E;++i)
    {
        if(CacheSet[i].valid==0)
        {
            miss();
            CacheSet[i].tag = tag;
            CacheSet[i].valid = 1;
            CacheSet[i].time = clock();
            flag = 1;
            break;
        }
        if(CacheSet[i].tag==tag)
        {
            hit();
            CacheSet[i].time = clock();
            flag = 1;
            break;
        }
        if(CacheSet[i].time<LRtime)
        {
            LRtime = CacheSet[i].time;
            LRid = i;
        }
    }
    if(flag==0)
    {
        miss();
        CacheSet[LRid].tag = tag;
        CacheSet[LRid].time = clock();
        eviction();
    }
}

int main(int argc,char* argv[])
{

    char* trace = NULL;
    int optc;
    while((optc = getopt(argc,(char* const *)argv,"vs:E:b:t:"))!=-1)
    {
        switch(optc)
        {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace = (char*)malloc(strlen(optarg)+1);
                strcpy(trace,optarg);
                break;
            case 'v':
                verbose = 1;
                break;                
        }
    }
    t = 64-s-b;

    Cache_Init(s,E,b);

    FILE* tracefile = fopen(trace,"r");
    char buf[50];

    char opt;
    long long addr;
    int size;
    while(1)
    {
        memset(buf,0,50);
        fgets(buf,50,tracefile);
        if(sscanf(buf," %c %llx,%d",&opt,&addr,&size)!=3)
            break;
        switch(opt)
        {
            case 'I':
                continue;
            case 'L':
            case 'S':
                if(verbose==1)
                {
                    printf("%c %llx,%d",opt,addr,size);
                    AccessMemory(addr,size);
                    printf(" \n");
                }
                else
                    AccessMemory(addr,size);
                break;
            case 'M':
                if(verbose==1)
                {
                    printf("%c %llx,%d",opt,addr,size);
                    AccessMemory(addr,size);
                    AccessMemory(addr,size);
                    printf(" \n");
                }
                else
                {
                    AccessMemory(addr,size);
                    AccessMemory(addr,size);
                }
                break;
        }
    }
    printSummary(hits,misses,evictions);

    free(trace);
    trace = 0;
    for(int i = 0;i<groupcount;++i)
    {
        free(Cache[i]);
        Cache[i] = NULL;
    }
    free(Cache);
    return 0;
}
```

完成的截图
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305182159330.png)

## Part B
https://zhuanlan.zhihu.com/p/387662272
### 32x32
#### 暴力转置
```c
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
}

```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305241452391.png)
#### 分块转置
```c
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    int i1,j1;
    for(j = 0;j<M;j+=8)
        for(i = 0;i<M;i+=8)
            for(j1 = j;j1<j+8;++j1)
                for(i1 = i;i1<i+8;++i1)
                    B[j1][i1] = A[i1][j1];
}
```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305241452306.png)
#### 分块+变量存储
A和B矩阵相同下标的元素映射到缓存的同一组(回忆一下缓存的分组机制)
所以对于对角线上的元素,AB的连续访问发生冲突.
这里可以用空间换时间,一次将进入缓存的一整行读出来保存到临时变量中.

```c
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    int j1;
    int val1,val2,val3,val4,val5,val6,val7,val0;
    for(j = 0;j<M;j+=8)
        for(i = 0;i<M;i+=8)
            for(j1 = j;j1<j+8;++j1)
            {
                val0 = A[j1][i];
                val1 = A[j1][i+1];
                val2 = A[j1][i+2];
                val3 = A[j1][i+3];
                val4 = A[j1][i+4];
                val5 = A[j1][i+5];
                val6 = A[j1][i+6];
                val7 = A[j1][i+7];
                B[i][j1] = val0;
                B[i+1][j1] = val1;
                B[i+2][j1] = val2;
                B[i+3][j1] = val3;
                B[i+4][j1] = val4;
                B[i+5][j1] = val5;
                B[i+6][j1] = val6;
                B[i+7][j1] = val7;
            }
}
```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305241453434.png)
### 64x64
按照8x8分块,块的内部会发生冲突,于是使用4x4分块.
```c
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

    int i, j;
    int i1,j1;
    for(j = 0;j<M;j+=4)
        for(i = 0;i<M;i+=4)
            for(j1 = j;j1<j+4;++j1)
                for(i1 = i;i1<i+4;++i1)
                    B[j1][i1] = A[i1][j1];

}
```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202305241548256.png)

# Lab6:Shell lab
实现一个有工作分配,信号处理,进程回收的shell.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306190955121.png)
## 部分实现
### 函数声明
```cpp
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*Some function defined by myself*/
pid_t Fork();//即CSAPP上提到的错误处理封装函数,不过用起来并不是很顺手
```
### eval
调用parseline函数解析命令行得到argv参数列表,获得前台或后台运行的标志state.
调用builtin_cmd检测是否是内置命令,若是则在其中处理,否则返回eval函数fork出子进程,子进程execve方式启动新程序,父进程将job通过addjob加入全局jobs列表,若为前台运行则调用waitfg等待前台程序结束.

需要注意的问题有两个.
第一个:
> When you run your shell from the standard Unix shell, your shell is running in the foreground processgroup.  If your shell then creates a child process, by defaultthat child will also be a member of theforeground process group. Since typingctrl-csends a SIGINT to every process in the foregroundgroup, typingctrl-cwill send a SIGINT to your shell, as well as to every process that your shellcreated, which obviously isn’t correct.Here  is  the  workaround:   After  thefork,  but  before  theexecve,  the  child  process  should  callsetpgid(0, 0), which puts the child in a new process group whose group ID is identical to thechild’s PID. This ensures that there will be only one process,  your shell, in the foreground processgroup.   When you typectrl-c, the shell should catch the resulting  SIGINT and then forward itto the appropriate foreground job (or more precisely, the process group that contains the foregroundjob).

因为我们的shell(tsh)是运行在Unix shell之上的,所以当kernel发出一个SIGINT之类的信号,Unix shell会将信号同时发送给tsh以及所有tsh创建的进程(因为tsh是当前shell的前台进程,shell的默认行为会将SIGINT信号发送给整个`前台进程组`),而我们想要实现的只是将信号发送给tsh的前台进程组.所以在fork子进程后,需要setpgid使得子进程的进程组与父进程独立.
> int setpgid(pid_t pid, pid_t pgid);
> 该函数可以用于将一个进程加入到指定的进程组中，或者创建一个新的进程组。具体的行为取决于 pid 参数的取值：
    1. 如果 pid 参数为 0，则表示将调用进程加入到与调用进程的PID相同的进程组中。
    2. 如果 pgid 参数为 0，则表示将 pid 指定的进程的进程组ID设置为其自身的PID。
    3. 如果 pgid 参数不为 0，则表示将 pid 指定的进程的进程组ID设置为 pgid。
    setpgid 函数的返回值为 0 表示成功，返回值为 -1 表示出现错误，此时可以通过查看 errno 变量来获取具体的错误信息。

第二个:
> In eval, the parent must usesigprocmaskto block SIGCHLD signals before it forks the child,and then unblock these signals, again usingsigprocmaskafter it adds the child to the job list by calling addjob. Since children inherit the blocked vectors of their parents, the child must be sureto then unblock SIGCHLD signals before it execs the new program.6
    The parent needs to block theSIGCHLDsignals in this way in order to avoid the race condition wherethe child is reaped by sigchld handler(and thus removed from the job list) before the parent calls addjob.

由于进程间执行顺序是不确定的,子进程在被fork之后在最极端的情况下可以一直执行到结束而父进程还刚从fork函数返回.也就是说,父进程一旦fork子进程,随时可能收到SIGCHLD信号(子进程随时可能暂停或终止).设想一下在父进程fork子进程后,addjob之前,子进程结束发出SIGCHLD信号,父进程捕获信号并在信号处理程序中deletejob删除一个不存在的job,这可能引发错误或被deletejob无视(取决于deletejob的实现),父进程从信号处理函数返回后再调用addjob则会将一个已经终止的进程加入任务列表中.这显然是错误的.所以需要在fork函数之前阻塞SIGCHLD信号,addjob后恢复.需要注意的是子进程会继承父进程的阻塞状态,所以需要在execve之前恢复阻塞.
```cpp
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
//  * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{
    char* argv[MAXARGS];
    pid_t pid;
    int state;
    sigset_t mask,prev;

    sigemptyset(&mask);
    sigaddset(&mask,SIGCHLD);

    if(parseline(cmdline,argv))
        state = BG;
    else
        state = FG;

    if(argv[0]==NULL)
        return;

    if(!builtin_cmd(argv))
    {
        sigprocmask(SIG_BLOCK,&mask,&prev);
        if((pid = Fork())==0)
        {
           sigprocmask(SIG_SETMASK,&prev,NULL);
            if(setpgid(0, 0) < 0)
                unix_error("setpgid error");
            if(execve(argv[0],argv,environ)<0)
            {
                unix_error("%s: command not found");
                exit(-1);
            }
        }
        addjob(jobs,pid,state,cmdline);
        sigprocmask(SIG_SETMASK,&prev,NULL);
        if(state==FG)
            waitfg(pid);
        else
            printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);
    }
    return;
}
```
### builtin_cmd
```cpp
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit"))
    {
        exit(0);
    }

    if(!strcmp(argv[0],"fg")||!strcmp(argv[0],"bg"))
    {
        do_bgfg(argv);
        return 1;
    }
    if(!strcmp(argv[0],"jobs"))
    {
        listjobs(jobs);
        return 1;
    }        
    return 0;     /* not a builtin command */
}
```
### dofgbg
```cpp
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    pid_t pid;
    int jid;
    struct job_t* job;
    if(argv[1]==NULL)
    {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }

    if(argv[1][0]=='%')
    {
        if((jid = atoi(&argv[1][1]))<=0)
        {
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);
            return;
        }
        if((job = getjobjid(jobs,jid))==NULL)
        {
            printf("%%%d: No such job\n", jid);
            return;
        }
    }
    else
    {
        if((pid = atoi(&argv[1][1]))<=0)
        {
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);
            return;
        }
        if((job = getjobpid(jobs,pid))==NULL)
        {
            printf("%%%d: No such process\n", pid);
            return;
        }
    }
    if(!strcmp(argv[0],"bg"))
    {
        job->state = BG;
        if(kill(-job->pid,SIGCONT)<0)
            unix_error("kill error");
        printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
    }
    else if(!strcmp(argv[0],"fg"))
    {
        job->state = FG;
        if(kill(-job->pid,SIGCONT)<0)
            unix_error("kill error");
        printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
        waitfg(job->pid);
    }
    else
    {
        puts("do_bgfg: Internal error");
        exit(0);
    }
    return;
}
```
### waitfg
我使用了和write up上不同的处理,具体见注释.
```cpp
/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    struct job_t* job = getjobpid(jobs,pid);
    
    //根据write up上的描述,waitfg使用 use a busy loop around thesleepfunction.的方式实现
    // while(1)
    // {
    //     if(job->state==FG)
    //         sleep(1);
    //     else
    //         break;
    // }

    //但CSAPP书上提到这种方法执行太慢,故采用sigsuspend函数
    sigset_t mask,prev;
    //获取当前set存入prev
    sigemptyset(&mask);
    sigprocmask(SIG_BLOCK,&mask,&prev);
    while(job->state==FG)
    {
        sigsuspend(&prev);
    }
    sigprocmask(SIG_SETMASK,&prev,NULL);
    return;
}
```
### sigchld_handler
注意Linux的显式信号阻塞可能丢弃掉一部分SIGCHLD信号,所以在sigchld_handler函数的一次调用中需要尽可能多的回收子进程.
```cpp
/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    pid_t pid;
    int jid;
    int status;
    struct job_t* job;

    if(verbose)
        puts("sigchld_handler: entering");



    while((pid = waitpid(-1,&status,WNOHANG|WUNTRACED))>0)
    {
        if((job = getjobpid(jobs,pid))==NULL)
        {
            fprintf("Lost track of (%d)\n",pid);
            return;
        }
        jid = job->jid;
        if(WIFSTOPPED(status))
        {
            printf("Job [%d] (%d) stopped by signal %d\n",jid,job->pid,WSTOPSIG(status));
            job->state = ST;
        }
        else if(WIFEXITED(status))
        {
            if(deletejob(jobs,pid))
                if(verbose)
                {
                    printf("sigchld_handler: Job [%d] (%d) deleted\n", jid, pid);
                    printf("sigchld_handler: Job [%d] (%d) terminates OK (status %d)\n", jid, pid, WEXITSTATUS(status));
                }
        }
        else
        {
            if(deletejob(jobs,pid))
            {
                if(verbose)
                    printf("sigchld_handler: Job [%d] (%d) deleted\n", jid, pid);
            }
            printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status));
        }
    }

    if(verbose)
        puts("sigchld_handler: exiting");
    
    return;
}
```
### sigint_handler、sigtstp_handler
```cpp
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
    if(verbose)
        puts("sigint_handler: entering");
    pid_t pid = fgpid(jobs);

    if(pid){
        //pid设置为负,将信号发送给整个进程组.
        if(kill(-pid, SIGINT) < 0)
            unix_error("kill (sigint) error");
        if(verbose){
            printf("sigint_handler: Job (%d) killed\n", pid);
        }
    }

    if(verbose)
        puts("sigint_handler: exiting");

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    if(verbose)
        puts("sigstp_handler: entering");
    pid_t pid = fgpid(jobs);
    struct job_t* job = getjobpid(jobs,pid);

    if(pid)
    {
        //pid设置为负,将信号发送给整个进程组.
        if(kill(-pid,SIGTSTP)<0)
        {
            unix_error("kill (tstp) error");
        }
        if(verbose){
            printf("sigstp_handler: Job [%d] (%d) stopped\n", job->jid, pid);
            }
    }

    if(verbose)
        puts("sigstp_handler: exiting");
    return;
}
```
### parseline
lab直接给出的,学一下实现.
```cpp
/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}
```
# Lab7:Malloc lab
## 设计
参照ptmalloc.
### 堆块结构
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306221858686.png)
### 空闲块组织
分为fastbins和bins,fastbins不参与合并不进行切割.两者均采用单向链表的组织结构,fastbins有7个,由于堆块对齐的原因各个fastbins中chunk大小相同,故不需排序,从头部取出或放入.bins有8个,需排序.
## 实验
第一次跑过所有测试,80分,但此时还没有加入空闲块合并的功能.
查看util极低的数据,观察发现确实是空闲块未合并导致的.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306211450673.png)

加入空闲块合并之后,好的,降了10分
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306221745035.png)
发现原因是因为我的设计大部分参考的是glibc中ptmalloc的实现,注重查找的效率,但由于我本地的机器较快导致性能一直是满分,所以评分仅取决于空间利用率.

所以一些设计比如增加bin的数量(将空闲块按大小分区间组织加快查找速度),设计fastbin不参与合并加快速度,分配较大的top chunk减少mem_sbrk的调用次数(其实没有必要,ptmalloc这样实现是为了减少sbrk或mmap系统调用的开销,而本实验中的mm_malloc是建立在一个模拟的mem_sbrk之上,并不会进行系统调用)等,反而降低了空间利用率.

这是将fastbins和bins数量均减为1,topchunk默认大小改为0x1000后的成绩.可见空间利用率大幅提升.

![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306221847904.png)

还有很多可以完善的地方,不过现阶段对算法和数据结构的理解还不够,没办法自己设计,照着ptmalloc2写一份也没有太多意义.等之后看有没有机会实现一个完整的,直接使用系统调用的.
### 一些debug插曲
#### 1
(高达5220%的内存利用率的超级内存分配器
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306202159747.png)

一部分trace测试因分配到了brk更高地址被终止.
另一部分则发生段错误造成crash,测试这部分样例.
直接run起来,观察崩溃点,发现程序在访问(eax+4)内存时发生段错误,该表达式对应为top->size.即eax表示top的值为0.
top指针在初始化之后肯定是不可能为0的.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306202147605.png)
设置观察点:watch (top\==0),重新运行程序.
发现程序在此处停住,但源码中并没有top作为左值的语句.
瞬间反应过来,bins下标越界.
改掉程序中对i的检查.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306202152686.png)

#### 2
然而我调试了接近两个小时后,发现最关键的问题在mdriver程序会多次调用mm_init函数,且在调用前将mem_brk复位,而我的init函数只是为调用一次使用的,并没有清空bins和top,导致多次运行时使用大量mem_brk之外的内存......
这才是上面内存利用率超高的原因.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306210005305.png)

不过两小时也没白费,学了好多诸如watchpoints的调试命令和找到一些调试技巧.

#### 3
重写init函数,再次运行,不出意外的异常退出.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306210007226.png)
不过只在部分样例中异常退出,原因是ran out of memory.这倒是很正常,因为此时还没有编写空闲块合并和realloc的功能
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306210008744.png)

#### 4
编写realloc功能后运行崩溃,调试发现是realloc时没有做边界检查,如果下一个chunk是topchunk将会在切割后unlink(topchunk),而topchunk自然是不会在bin中的故引发"mm_unlink: nonexistent mptr"
#### 5
一个调了接近6小时才发现的错误,在mm_realloc和mm_malloc里调用mm_free时,应该使用用户态的指针而不是堆块头部指针.....感觉这是个挺容易犯错的地方
# Lab8:Proxy lab
## Part A
### 设计
使用的结构为
```cpp
typedef struct 
{
    char method[MAXLINE];
    char host[MAXLINE];
    char port[MAX_PORTLEN];
    char path[MAXLINE];
    char cgiargs[MAXLINE];
    char version[MAXLINE];
}RequestLine;

typedef struct 
{
    char name[MAXLINE];
    char value[MAXLINE];
}RequestHeader;
```
这是整个PartA的处理流程,其中Forward2Client未做处理,直接将从服务器收到的数据原封不动转发给客户端.Part A完成后程序已经可以在浏览器中实现代理访问.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306240020148.png)
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306240023578.png)
### 代码
```cpp
#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define MAX_PORTLEN 10
#define MAX_HEADERS 30

#define PREFIX_PRINT "HanQi_Proxy > "
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3";


typedef struct 
{
    char method[MAXLINE];
    char host[MAXLINE];
    char port[MAX_PORTLEN];
    char path[MAXLINE];
    char cgiargs[MAXLINE];
    char version[MAXLINE];
}RequestLine;

typedef struct 
{
    char name[MAXLINE];
    char value[MAXLINE];
}RequestHeader;



```

```cpp
#include "proxy.h"

void parse_uri(char uri[],RequestLine* requestline)
{
    char* address_ptr = NULL;
    char* port_ptr = NULL;
    char* path_ptr = NULL;

    if(address_ptr = strstr(uri,"//"))
        address_ptr += 2;
    else
        address_ptr = uri;
    if(port_ptr = strstr(address_ptr,":"))
        port_ptr += 1;
    path_ptr = strstr(address_ptr,"/");

    if(path_ptr!=NULL)
        strncpy(requestline->path,path_ptr,MAXLINE);
    else
    {
        strncpy(requestline->path,"/",2);
        path_ptr = address_ptr+strlen(address_ptr);
    }
    if(port_ptr!=NULL)
        strncpy(requestline->port,port_ptr,path_ptr-port_ptr);
    else
    {
        strncpy(requestline->port,"80",3);
        port_ptr = path_ptr;
    }
    strncpy(requestline->host,address_ptr,port_ptr-address_ptr-1);
    printf(PREFIX_PRINT"RequestTarget: %s:%s%s\n",requestline->host,requestline->port,requestline->path);
}



int read_requesthdrs(rio_t* rio,RequestHeader requestheaders[MAX_HEADERS])
{
    int i = 0;
    char buf[MAXLINE];
    Rio_readlineb(rio,buf,MAXLINE);
    while(strcmp("\r\n",buf))
    {
        sscanf(buf,"%[^:]: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        printf("%s: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        ++i;
        if(i==MAX_HEADERS)
            break;
        Rio_readlineb(rio,buf,MAXLINE);
    }
    return i;
}

void add_headers(RequestHeader requestheaders[MAX_HEADERS],int* headers_num,char* headername,char* headervalue)
{
    for(int i = 0;i<MAX_HEADERS;++i)
    {
        if(!strcmp(headername,requestheaders[i].name))
        {
            strncpy(requestheaders[i].value,headervalue,MAXLINE);
            return;
        }
    }
    strncpy(requestheaders[*headers_num].name,headername,MAXLINE);
    strncpy(requestheaders[*headers_num].value,headervalue,MAXLINE);
    ++(*headers_num);
    return;
}

void Write2Server(int server_fd,RequestLine* requestline,RequestHeader requestheaders[],int headers_num)
{
    printf(PREFIX_PRINT"Forwarding to %s:%s,ing...\n",requestline->host,requestline->port);
    char buf[3*MAXLINE+5];

    //这里不是很懂,最后转发给服务器的uri只留下文件路径?
    snprintf(buf,3*MAXLINE+5,"%s %s %s\r\n",requestline->method,requestline->path,requestline->version);    
    printf("%s",buf);
    Rio_writen(server_fd,buf,strlen(buf));

    for(int i = 0;i<headers_num;++i)
    {
        snprintf(buf,3*MAXLINE+5,"%s: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        printf("%s",buf);
        Rio_writen(server_fd,buf,strlen(buf));
    }
    Rio_writen(server_fd,"\r\n",2);
    printf("\r\n");
}

void Forward2Client(int client_fd,int server_fd)
{
    int headers_num = 0;
    char buf[2*MAXLINE+5];
    rio_t server_rio;
    int size = 0;

    printf(PREFIX_PRINT"Forwarding to Client,ing...\n");

    Rio_readinitb(&server_rio,server_fd);
    Rio_readlineb(&server_rio,buf,2*MAXLINE+5);
    printf("%s",buf);
    Rio_writen(client_fd,buf,strlen(buf));
    // headers_num = read_requesthdrs(&server_rio,headers);

    Rio_readlineb(&server_rio,buf,2*MAXLINE+5);
    while(strcmp("\r\n",buf))
    {
        printf("%s",buf);
        Rio_writen(client_fd,buf,strlen(buf));
        Rio_readlineb(&server_rio,buf,2*MAXLINE+5);
    }
    Rio_writen(client_fd,"\r\n",2);
    printf("\r\n");


    
    while(size = Rio_readlineb(&server_rio,buf,MAXLINE))
    {
        printf("%s",buf);
        Rio_writen(client_fd,buf,size);
    }
}

void* StartWork(int connfd)
{
    char buf[MAXLINE],uri[MAXLINE];
    RequestLine requestline;
    RequestHeader requestheaders[MAX_HEADERS];
    rio_t client_rio;
    int server_fd;


    Rio_readinitb(&client_rio,connfd);
    if(!Rio_readlineb(&client_rio,buf,MAXLINE))
        return NULL;
    printf("%s",buf);
    //writeup中提到这里有个多行请求行的问题,待处理
    /*
     Ideally  your  HTTP  request  parser  will  be  fullyrobust according to the relevant sections of RFC 1945, except for one detail: while the specification allowsfor multiline request fields
     */
    sscanf(buf,"%s %s %s",requestline.method,uri,requestline.version);
    int headers_num = read_requesthdrs(&client_rio,requestheaders);

    parse_uri(uri,&requestline);
    add_headers(requestheaders,&headers_num,"Host",requestline.host);
    add_headers(requestheaders,&headers_num,"User-Agent",user_agent_hdr);
    add_headers(requestheaders,&headers_num,"Connection","close");
    add_headers(requestheaders,&headers_num,"Proxy-Connection","close");

    server_fd = open_clientfd(requestline.host,requestline.port);
    printf(PREFIX_PRINT"Connect to %s:%s.(serverFd: %d)\n",requestline.host,requestline.port,server_fd);

    Write2Server(server_fd,&requestline,requestheaders,headers_num);
    Forward2Client(connfd,server_fd);

    return NULL;
}

int main(int argc,char* argv[])
{
    int listenfd,connfd;
    char client_hostname[MAXLINE];
    char client_port[MAX_PORTLEN];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    

    if(argc!=2)
    {
        fprintf(stderr,PREFIX_PRINT"usage: %s <port>\n",argv[0]);
        exit(1);
    }
    listenfd = open_listenfd(argv[1]);

    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd,(SA*)&clientaddr,&clientlen);
        Getnameinfo((SA*)&clientaddr,clientlen,client_hostname,MAXLINE,client_port,MAX_PORTLEN,0);
        printf(PREFIX_PRINT"Acceptd connection from (%s,%s),clientFd: %d\n",client_hostname,client_port,connfd);
        StartWork(connfd);
    }
    return 0;
}

```


## Part B
并发做的是预线程化的方式.照书上实现了sbuf包实现对client_fd(connfd)的管理.将之前程序中的StartWork作为线程例程.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306251624471.png)
### sbuf包
```cpp
#ifndef SBUF_H
#define SBUF_H

#include <semaphore.h>
#include <stdlib.h>

typedef struct {
    int* buf;
    int count;
    int front;  //buf[(front+1)%count]为第一个item
    int rear;   //buf[rear%count]为最后一个元素
    sem_t mutex;
    sem_t slots;
    sem_t items;
}sbuf_t;

void sbuf_init(sbuf_t* sp,int n);
void sbuf_deinit(sbuf_t* sp);
void sbuf_insert(sbuf_t* sp,int item);
int sbuf_remove(sbuf_t* sp);

#endif



void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = calloc(n,sizeof(int));
    sp->count = n;
    sp->front = sp->rear = 0;
    sem_init(&sp->mutex,0,1);
    sem_init(&sp->slots,0,n);
    sem_init(&sp->items,0,0);
}

void sbuf_deinit(sbuf_t *sp)
{
    free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item)
{
    sem_wait(&sp->slots);
    sem_wait(&sp->mutex);
    sp->buf[(++sp->rear)%(sp->count)] = item;
    sem_post(&sp->mutex);
    sem_post(&sp->items);
}

int sbuf_remove(sbuf_t *sp)
{
    int item;
    sem_wait(&sp->items);
    sem_wait(&sp->mutex);
    item = sp->buf[(++sp->front)%(sp->count)];
    sem_post(&sp->mutex);
    sem_post(&sp->slots);
    return item;
}

```
###  main
```cpp
int main(int argc,char* argv[])
{
    int listenfd,connfd;
    char client_hostname[MAXLINE];
    char client_port[MAX_PORTLEN];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if(argc!=2)
    {
        fprintf(stderr,PREFIX_PRINT"usage: %s <port>\n",argv[0]);
        exit(1);
    }

    sbuf_init(&sbuf,SBUFSIZE);
    for(int i = 0;i<NTHREADS;++i)
        pthread_create(&tid,NULL,StartWork,NULL);
    
    listenfd = open_listenfd(argv[1]);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd,(SA*)&clientaddr,&clientlen);        
        Getnameinfo((SA*)&clientaddr,clientlen,client_hostname,MAXLINE,client_port,MAX_PORTLEN,0);
        printf(PREFIX_PRINT"Acceptd connection from (%s,%s),clientFd: %d\n",client_hostname,client_port,connfd);

        sbuf_insert(&sbuf,connfd);
    }
    return 0;
}

```
## Part C
类似于Cache lab的设计,读写cache使用的是读者优先的读者-写者模型.要注意的一点是读者读完后其实也会进行写(更新lru),所以也要上锁.
想清楚读者-写者模型的特征,每一个地方上锁是为了避免怎样的竞争,之后便可以根据需求做出变化.
```cpp
typedef struct 
{
    char uri[MAXLINE];
    char content_type[MAXLINE];
    char* data;
    char server[MAXLINE];
    size_t size;
    size_t lru;
}CacheLine;

typedef struct
{
    CacheLine* cachelines;
    size_t current_lru;
    size_t cache_num;
    size_t cache_size;
}Cache;

```
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202306271005363.png)

