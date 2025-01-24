# CTF #12: RSA

In this CTF, was provided a message encrypted with RSA, the public key (e) and the modulus (n):

> Public exponent: 65537
>
> Modulus: 1404447761611184302913519680303925573139044514798677009948672509044786529730476274474284979081308875165000889686495260606709295068862629863225370551872799695367341623231494732153668260111752017556570265545731391907007705749330332915651062912158090234391813523541494751111712708342047929867028059999367808229
>
> Cipher: a4b13f86bf9d34e106d206bafdaf00be2065029bfbbecf39ef05c1872fdf2ca43f99ca0973c48d0718d4e7b3164ae5c6c1db9b353fbf9bb1c2203ed4ed64183132d5a424ae204cf9c6c4e312484a78057f9547ace8a3bb8fa9c6404328e9de04f70b4558914ecdbe9e40b3cf55fc602ce6f5f5dc2960f3640efa6cb2b99b50010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

We also know that the two primes, p and q, involved in creating the keys were close to:

p: 2 ^ (500+(((t-1)\*10 + g) // 2)) = 2 ^ 508

q: 2 ^ 501+(((t-1)\*10 + g) // 2) = 2 ^509

Knowing the values of these two large primes will allow us to calculate the value of the private key, thus, deciphering the message.

We needed to rely on an algorithm that quickly determines is a number is prime. We used the Miller-Rabin algorithm found in <https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/>:

```python
# Python3 program Miller-Rabin primality test
import random 

# Utility function to do
# modular exponentiation.
# It returns (x^y) % p
def power(x, y, p):
	
	# Initialize result
	res = 1
	
	# Update x if it is more than or
	# equal to p
	x = x % p
	while (y > 0):
		
		# If y is odd, multiply
		# x with result
		if (y & 1):
			res = (res * x) % p

		# y must be even now
		y = y>>1 # y = y/2
		x = (x * x) % p
	
	return res

# This function is called
# for all k trials. It returns
# false if n is composite and 
# returns false if n is
# probably prime. d is an odd 
# number such that d*2<sup>r</sup> = n-1
# for some r >= 1
def miillerTest(d, n):
	
	# Pick a random number in [2..n-2]
	# Corner cases make sure that n > 4
	a = 2 + random.randint(1, n - 4)

	# Compute a^d % n
	x = power(a, d, n)

	if (x == 1 or x == n - 1):
		return True

	# Keep squaring x while one 
	# of the following doesn't 
	# happen
	# (i) d does not reach n-1
	# (ii) (x^2) % n is not 1
	# (iii) (x^2) % n is not n-1
	while (d != n - 1):
		x = (x * x) % n
		d *= 2

		if (x == 1):
			return False
		if (x == n - 1):
			return True

	# Return composite
	return False

# It returns false if n is 
# composite and returns true if n
# is probably prime. k is an 
# input parameter that determines
# accuracy level. Higher value of 
# k indicates more accuracy.
def isPrime( n, k):
	
	# Corner cases
	if (n <= 1 or n == 4):
		return False
	if (n <= 3):
		return True

	# Find r such that n = 
	# 2^d * r + 1 for some r >= 1
	d = n - 1
	while (d % 2 == 0):
		d //= 2

	# Iterate given number of 'k' times
	for i in range(k):
		if (miillerTest(d, n) == False):
			return False

	return True
```

Our strategy was to find a set of primes near the values of 2 ^508 and 2 ^ 509. To do this, we decreased and increased one unit at a time two integers, starting both with our reference values and verifying if the number was prime. Whenever this occurred, we would add the found prime to the set.

```python
near_p: int = int(2 ** 508)
near_q: int = int(2 ** 509)
print(f"near p: {near_p}")
print(f"near q: {near_q}")

p_set = set()
q_set = set()

l_p = near_p
h_p = near_p
l_q = near_q
h_q = near_q

for i in range(0, 10):
    while True:
        if(isPrime(l_p, 10)):
            p_set.add(l_p)
            l_p -= 1
            break
        l_p -= 1
    while True:
        if(isPrime(h_p, 10)):
            p_set.add(h_p)
            h_p += 1
            break
        h_p += 1
    while True:
        if(isPrime(l_q, 10)):
            q_set.add(l_q)
            l_q -= 1
            break
        l_q -= 1
    while True:
        if(isPrime(h_q, 10)):
            q_set.add(h_q)
            h_q += 1
            break
        h_q += 1
```

After having generated 20 candidates for each prime, we would try all the combinations between the two of them to check if their multiplication would be equal to the modulus (which is publicly known).

```python
e: int = int(65537)
n: int = int(1404447761611184302913519680303925573139044514798677009948672509044786529730476274474284979081308875165000889686495260606709295068862629863225370551872799695367341623231494732153668260111752017556570265545731391907007705749330332915651062912158090234391813523541494751111712708342047929867028059999367808229)
c = bytearray.fromhex("a4b13f86bf9d34e106d206bafdaf00be2065029bfbbecf39ef05c1872fdf2ca43f99ca0973c48d0718d4e7b3164ae5c6c1db9b353fbf9bb1c2203ed4ed64183132d5a424ae204cf9c6c4e312484a78057f9547ace8a3bb8fa9c6404328e9de04f70b4558914ecdbe9e40b3cf55fc602ce6f5f5dc2960f3640efa6cb2b99b5001")
p = 0
q = 0
found = False

for i in p_set:
    for j in q_set:
        if i * j == n:
            p = i
            q = j
            found = True
            break
    if found:
        break
```

The final results for p and q were:

> p: 837987995621412318723376562387865382967460363787024586107722590232610251879596686050117143635431464230626991136655378178359617675746660621652103062881291
>
> q: 1675975991242824637446753124775730765934920727574049172215445180465220503759193372100234287270862928461253982273310756356719235351493321243304206125760719

This allowed to calculate Φ(n) = (p - 1) \* (q - 1):

> phi: 1404447761611184302913519680303925573139044514798677009948672509044786529730476274474284979081308875165000889686495260606709295068862629863225370551872797181403354758994538562023981096515603115175478904471973068739237007918574694125592912560727183939999121642568084784977177629489020689885163103690179166220

To calculate the secret key, we would just need to calculate the modular inverse of e (public key) in mod Φ(n). To achieve this, we used the Extended Euclidean Algorithm found in <https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/> :

```python
def modInverse(A, M):
    m0 = M
    y = 0
    x = 1

    if (M == 1):
        return 0

    while (A > 1):

        # q is quotient
        q = A // M

        t = M

        # m is remainder now, process
        # same as Euclid's algo
        M = A % M
        A = t
        t = y

        # Update x and y
        y = x - q * y
        x = t

    # Make x positive
    if (x < 0):
        x = x + m0

    return x
```

To calculate d (secret key), we did:

```python
phi = (p - 1) * (q - 1)
d = modInverse(e, phi)
```

Since the message was encrypted with the public key, it can only be decrypted with the secret key. As we now have this secret key, we can successfully decipher the message:

```
int_c = int.from_bytes(c, "little")
m = pow(int_c, d, n)
m = m.to_bytes(256, 'little')
m = m.decode("utf-8")
print(f"m: {m}")
```

The deciphered message was our flag:

### flag{zdpivcanzlwwayfr}
