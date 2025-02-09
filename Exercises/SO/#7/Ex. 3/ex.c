#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t lock;
pthread_cond_t not_zero;

void *inc(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        count++;
        if (count > 0) pthread_cond_signal(&not_zero); // Less signal calls
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *dec(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        while (count == 0) pthread_cond_wait(&not_zero, &lock);
        count--;
        if (count < 0) printf("."); // Never prints
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    printf("Start: %d\n", count);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_zero, NULL);
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, inc, NULL);
    pthread_create(&tid2, NULL, dec, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);
    printf("End: %d\n", count);
}
