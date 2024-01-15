# 1. Create libraries libvector.a and libvector.so
gcc -Wall -c -fPIC vector_*.c
gcc -shared -o libvector.so vector_*.o
ar -rc libvector.a vector_*.o

# Print the contents of the library
ar -t libvector.a
nm libvector.a

# 2. Compile use_vector.c
gcc -Wall use_vector.c -o use_vector -L. -lvector -lm