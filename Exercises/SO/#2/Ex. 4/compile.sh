# 1. Create libraries libvector.a and libvector.so
gcc -Wall -c -fPIC list_*.c node_*.c
gcc -shared -o liblist.so list_*.o node_*.o
ar -rc liblist.a list_*.o node_*.o

# Print the contents of the library
ar -t liblist.a
nm liblist.a

# 2. Compile use_vector.c
gcc -g -Wall use_list.c -o use_list -L. -llist -lm