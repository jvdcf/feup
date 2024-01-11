#include <stdio.h>
#include <pthread.h>

int count = 0;

void *inc(void *arg)
{
    for (int i = 0; i < 1000000; i++)
        count++;
    return NULL;
}

int main()
{
    printf("Start: %d\n", count);
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, inc, NULL);
    pthread_create(&tid2, NULL, inc, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("End: %d\n", count);
}

/* Why the result can change?
 * Because the threads are not synchronized, so they can access the variable at the same time.  
 * Without locks, the result is not deterministic.
*/