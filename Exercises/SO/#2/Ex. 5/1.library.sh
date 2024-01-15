gcc -Wall -c complex_*.c
ar -rc libcomplex.a complex_*.o
ar -t libcomplex.a
nm libcomplex.a