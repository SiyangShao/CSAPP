# Phase_1
``` assembly
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq   
```
Key: <strings_not_equal>
``` assembly
0000000000401338 <strings_not_equal>:
  401338:	41 54                	push   %r12
  40133a:	55                   	push   %rbp
  40133b:	53                   	push   %rbx
  40133c:	48 89 fb             	mov    %rdi,%rbx
  40133f:	48 89 f5             	mov    %rsi,%rbp
  401342:	e8 d4 ff ff ff       	callq  40131b <string_length>
  401347:	41 89 c4             	mov    %eax,%r12d
  40134a:	48 89 ef             	mov    %rbp,%rdi
  40134d:	e8 c9 ff ff ff       	callq  40131b <string_length>
  401352:	ba 01 00 00 00       	mov    $0x1,%edx
  401357:	41 39 c4             	cmp    %eax,%r12d
  40135a:	75 3f                	jne    40139b <strings_not_equal+0x63>
  40135c:	0f b6 03             	movzbl (%rbx),%eax
  40135f:	84 c0                	test   %al,%al
  401361:	74 25                	je     401388 <strings_not_equal+0x50>
  401363:	3a 45 00             	cmp    0x0(%rbp),%al
  401366:	74 0a                	je     401372 <strings_not_equal+0x3a>
  401368:	eb 25                	jmp    40138f <strings_not_equal+0x57>
  40136a:	3a 45 00             	cmp    0x0(%rbp),%al
  40136d:	0f 1f 00             	nopl   (%rax)
  401370:	75 24                	jne    401396 <strings_not_equal+0x5e>
  401372:	48 83 c3 01          	add    $0x1,%rbx
  401376:	48 83 c5 01          	add    $0x1,%rbp
  40137a:	0f b6 03             	movzbl (%rbx),%eax
  40137d:	84 c0                	test   %al,%al
  40137f:	75 e9                	jne    40136a <strings_not_equal+0x32>
  401381:	ba 00 00 00 00       	mov    $0x0,%edx
  401386:	eb 13                	jmp    40139b <strings_not_equal+0x63>
  401388:	ba 00 00 00 00       	mov    $0x0,%edx
  40138d:	eb 0c                	jmp    40139b <strings_not_equal+0x63>
  40138f:	ba 01 00 00 00       	mov    $0x1,%edx
  401394:	eb 05                	jmp    40139b <strings_not_equal+0x63>
  401396:	ba 01 00 00 00       	mov    $0x1,%edx
  40139b:	89 d0                	mov    %edx,%eax
  40139d:	5b                   	pop    %rbx
  40139e:	5d                   	pop    %rbp
  40139f:	41 5c                	pop    %r12
  4013a1:	c3                   	retq   
```

At first, two strings are stored in %rdi and %rsi. Use lldb to see

``` bash
lldb bomb
b strings_not_equal
r
# input any string
# * thread #1, name = 'bomb', stop reason = breakpoint 1.1
#     frame #0: 0x0000000000401338 bomb`strings_not_equal
# bomb`strings_not_equal:
# ->  0x401338 <+0>: pushq  %r12
#     0x40133a <+2>: pushq  %rbp
#     0x40133b <+3>: pushq  %rbx
#     0x40133c <+4>: movq   %rdi, %rbx
p (char*) $rdi  # Print the content in %rdi as char
# (char *) $0 = 0x0000000000603780 "String you input before"
# Thus, $rdi saves string we input and $rsi saves answer
p (char*) $rsi
# (char *) $1 = 0x0000000000402400 "Border relations with Canada have never been better."
```

The answer is "Border relations with Canada have never been better."

# Phase 2
``` assembly
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq   
```
+ 1. read six numbers -> guess it's 6 numbers in an array (%rsp).  
+ 2. jmp 400f40 <phase_2 + 0x34> is jump to next judgement (not bomb!)
+ 3. If not jump, it will bomb

cmpl: 0x1 == %rsp, the first number must be 1, 

Let's only take look at the array:
``` assembly
cmpl $0x1, (%rsp) # arr[0] == 1
# Here jump to <phase+2 + 0x34>
lea    0x4(%rsp),%rbx # rbx = arr[1]
lea    0x18(%rsp),%rbp  # rbp = arr[6]
# Jump back to <phase_2 + 0x1b>
mov    -0x4(%rbx),%eax # eax = arr[x-1] 
add    %eax,%eax  # eax += eax
cmp    %eax,(%rbx) # cmp arr[x],arr[x-1]*2 (eax is 2 arr[x-1])
je     400f25 <phase_2+0x29>
callq  40143a <explode_bomb>
add    $0x4,%rbx # arr[x+1]
cmp    %rbp,%rbx
jne    400f17 <phase_2+0x1b> # Loop until end of array
jmp    400f3c <phase_2+0x40>
```
In C Language:
``` c
if(arr[0]!=1) bomb();
for(ll i = 1 ; i < 6; ++i){
  if(arr[i]!=arr[i-1] * 2){
    bomb;
  }
}
```
so the answer should be: "1 2 4 8 16 32"

# Phase 3
``` assembly
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq   
```
Key :
+ check $eax (return value of scanf), number of data which is correctly formatted, answer is 2. => there need two numbers
+ check 0x8($rsp) (x $rsp+0x8), we can see that it's the first number
+ The first one should be less then 7, or it would jump to explode_bomb

``` assembly
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
```
Key:
+ eax is the 32-bit of rax
+ \*0x402470(,%rax,8) is 0x402470[8\*a]
See status from \*0x402470 to \*0x402470 + 8*7: (like array)

| num |  goto | num | goto |
| --- | --- | --- | ---|
|0|0x0000000000400f7c  |1|    0x0000000000400fb9|
|2|0x0000000000400f83  |3|    0x0000000000400f8a|
|4|0x0000000000400f91   |5|   0x0000000000400f98|
|6|0x0000000000400f9f    |7|  0x0000000000400fa6|

Thus, it's like switch
``` c
if(a > 7){
  explode_bomb();
}
switch(a){
  case 0:
    a = 0xcf;
    break;
  case 1:
    a = 0x137;
    break;
  case 2:
    a = 0x2c3;
    break;
  case 3:
    a = 0x100;
    break;
  case 4:
    a = 0x185;
    break;
  case 5:
    a = 0xce;
    break;
  case 6:
    a = 0x2aa;
    break;
  case 7:
    a = 0x174;
    break;
}
if(a == b){
  success();
}
explode_bomb();
```
Thus, the answer of Phase 3 is:
| a | b |
| -- | -- |
| 0 | 0xcf|
|1 | 0x137 |
|2 | 0x2c3|
|3 |0x100|
|4|0x185|
|5|0xce|
|6|0x2aa|
|7|0x147|