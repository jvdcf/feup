# Classical Encryption: CTF #10

This challenge presents us with a ciphertext in Portuguese.  Upon close inspection, we can see that the cyphertext is composed by only 24 different characters, which suggests that it is a **substitution cipher**. So, the task at hand is to analyze the frequency of the characters in the ciphertext and try to map them to the most common characters in the Portuguese language, with the goal of decrypting the flag at the end of the message.

---

Using the python script from the past Logbook, we determined the frequency of the n-grams in the encrypted text:

```python
#!/usr/bin/env python3

from collections import Counter
import re

TOP_K  = 20
N_GRAM = 3

# Generate all the n-grams for value n
def ngrams(n, text):
    for i in range(len(text) -n + 1):
        # Ignore n-grams containing white space
        if not re.search(r'\s', text[i:i+n]):
           yield text[i:i+n]

# Read the data from the ciphertext
with open('L02G06.cph') as f:
    text = f.read()

# Count, sort, and print out the n-grams
for N in range(N_GRAM):
   print("-------------------------------------")
   print("{}-gram (top {}):".format(N+1, TOP_K))
   counts = Counter(ngrams(N+1, text))        # Count
   sorted_counts = counts.most_common(TOP_K)  # Sort 
   for ngram, count in sorted_counts:                  
       print("{}: {}".format(ngram, count))   # Print
```

It outputs the following:

```
-------------------------------------
1-gram (top 20):
^: 79
:: 62
): 50
;: 38
]: 37
!: 31
*: 30
?: 28
=: 25
&: 25
@: 23
,: 22
+: 16
$: 13
_: 8
/: 6
|: 5
>: 4
(: 4
-: 4
-------------------------------------
2-gram (top 20):
);: 13
^]: 12
!^: 12
:;: 12
;:: 11
?^: 11
=!: 8
:=: 8
@:: 8
]^: 7
^,: 7
)]: 6
&^: 6
^+: 6
:?: 6
^?: 6
^;: 6
]:: 6
^): 6
]): 6
-------------------------------------
3-gram (top 20):
:=!: 5
^]^: 4
]^^: 4
);:: 4
*?^: 4
=);: 3
;:;: 3
:?!: 3
!^?: 3
+^]: 3
^;:: 3
;:=: 3
:;:: 3
=!^: 3
^]&: 3
>&:: 3
:;!: 3
)?:: 3
!]): 3
^+]: 3
```

Consulting the portuguese frequency tables on <https://www.gta.ufrj.br/grad/06_2/alexandre/criptoanalise.html>, we can conclude that the characters '^', ':' and ')' map to characters 'A', 'E' and 'O', respectively.

| Digram | Frequency |
|--------|-----------|
| DE     | 1.76      |
| **RA**     | **1.67**      |
| **ES**     | **1.65**      |
| **OS**     | **1.51**      |
| **AS**     | **1.49**      |
| DO     | 1.41      |
| **AR**     | **1.33**      |
| CO     | 1.31      |
| EN     | 1.23      |
| QU     | 1.20      |
| ER     | 1.18      |
| DA     | 1.17      |
| **RE**     | **1.14**      |
| CA     | 1.11      |
| TA     | 1.10      |
| **SE**     | **1.08**      |

Given also the most common portuguese digrams and observing the most frequent digrams in the encrypted text, we can arrive to the conclusion that the characters ';' and '\]' map to the characters 'S' and 'R', respectively.

There are also two letters in the portuguese alphabet whose frequencies are more distinct from the rest, which are 'L' and 'P', which might correspond to  characters '+' and '$' in our cryptogram. However, trying to decrypt with the current table and analysing the words, we realized that it did not make sense that the character 'L' corresponded to either '+' or '\$' and that '+' very likely mapped for character 'P'.

We developed a python script to decrypt the message using a dictionary:

```python
from typing import List
from typing import Tuple

table: {str, str} = {
    '^': 'A',
    ':': 'E',
    ')': 'O',
    '+': 'P',
    ']': 'R',
    ';': 'S'
}

abc: List[str] = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']

have: List[str] = list(table.values())

print(f"Chars we have:\n{have}\n")
print(f"Chars remaining:\n{[c for c in abc if c not in have]}\n")

text: str = ""
with open('L02G06.cph') as f:
    text = f.read()

decrypt: str = text
for k, v in table.items():
    decrypt = decrypt.replace(k, v)

print(decrypt)
```

With a few characters already solved, we then tried to identify the words in the partially decrypted text. Here's what we got:

> REPRESE=!A --> REPRESENTA
> '=' -> N
> '!' -> T
>
> POR>&E --> PORQUE
> '>' -> Q
> '&' -> U
>
> AUTO?ARRO --> AUTOCARRO
> '?' -> C
>
> /*/,*OTECA --> BIBLIOTECA
> '/' -> B
> '*' -> I
> ',' -> L
>
> '@' -> D
>
> E#PLICA --> EXPLICA
> '#' -> X
>
> OSSEUSANOSDE<ISTORIA --> OS SEUS ANOS DE HISTORIA
> '<' -> H
>
> U$RECITALDE$USICA --> UM RECITAL DE MUSICA
> '$' -> M
>
> INDI_IDUALAOLON-ODASEMANA --> INDIVIDUAL AO LONGO DA SEMANA
> '\_' -> V
> '-' -> G
>
> ORGANI|ADO --> ORGANIZADO
> '|' -> Z
>
> CENTRODO(ESTIVAL --> CENTRO DO FESTIVAL
> '(' -> F
>
> INFANCIAE.UVENTUDE --> INFANCIA E JUVENTUDE
> '.' -> J

As a result, we now have an encryption table:

| Encrypted | Decrypted |
|:---------:|:---------:|
|     ^     |     A     |
|     /     |     B     |
|     ?     |     C     |
|     @     |     D     |
|     :     |     E     |
|     (     |     F     |
|     \-     |     G     |
|     <     |     H     |
|     *     |     I     |
|     .     |     J     |
|     ,     |     L     |
|     $     |     M     |
|     =     |     N     |
|     )     |     O     |
|     +     |     P     |
|     \>     |     Q     |
|     ]     |     R     |
|     ;     |     S     |
|     !     |     T     |
|     &     |     U     |
|     \_     |     V     |
|     \#     |     X     |
|    \|     |     Z     |

Using the python script mentioned above with the updated dictionary, we can decrypt the full message:

> TERIORCONTINUARAAPOSTARNOSESPECTACULOSPARAASESCOLASENASREPRESENTACOESNARUAPORQUEENARUAQUEESTAOCENTRODOFESTIVALEPORQUEAPROCURAINDIVIDUALAOLONGODASEMANAEESCASSAEXPLICAJOSELEITAODIRECTORDOCERTAMEPARAALEMDOSESPECTACULOSDETEATROOFESTIVALVAIAPRESENTARUMAEXPOSICAODECARTAZESDEDICADAAOSSEUSANOSDEHISTORIAUMRECITALDEMUSICAPELOGRUPOOSGAMBOZINOSEVARIOSCOLOQUIOSSOBREASITUACAOACTUALDOTEATROPARAAINFANCIAEJUVENTUDESERATAMBEMMONTADOUMCARROCELEUMAUTOCARROBIBLIOTECAINFANTILNAPRACADALIBERDADEEORGANIZADOUMDIADEAVENTU{CLVNELSLTZJLEZEN}

Thus, our flag is:

#### flag{clvnelsltzjlezen}
