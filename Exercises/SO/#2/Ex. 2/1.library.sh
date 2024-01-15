# Dinamic library
gcc -Wall -c -fPIC complex_*.c
gcc -shared -o libcomplex.so complex_*.o
ar -rc libcomplex.a complex_*.o
ar -t libcomplex.a

nm libcomplex.a