#include <stdio.h>
#include <pthread.h>

void *thread_func(void *arg)
{
    printf("I am thread #%d\n", *(int *)arg);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int i = 1;
    int j = 2;

    /* Create 2 threads t1 and t2 with default attributes which will execute
    function "thread_func()" in their own contexts with specified arguments. */
    pthread_create(&t1, NULL, &thread_func, &i);
    pthread_create(&t2, NULL, &thread_func, &j);

    /* This makes the main thread wait on the death of t1 and t2. */
  
    pthread_join(t2, NULL);

    printf("In main thread\n");
 
    pthread_join(t1, NULL);
    return 0;
}