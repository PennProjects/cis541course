#include <pthread.h> //Needed for thread functionality
#include <semaphore.h> // Needed for semaphore functionality



//Function to be run on separate threads
// Must have single "void*" parameter and return "void*"
//Thread is stopped / detroyed when function exists
void* thread_routine(void* args) {
    //Cast void* argument back to semaphore*
    sem_t* semaphore  = (sem_t*) args;

    //Acquire the semaphore, blocking of the count is <= 0
    //sem_trywait(semaphore) can be used if we didn't want to block
    sem_wait(semaphore);

    //Use some shared resource

    //Release the semaphore
    sem_post(semaphore);
}

int main(int argc, char** argv){

    //Create a new semaphore object and initialize it 6 resources
    sem_t* semaphore = new sem_t();
    sem_init(semaphore, 0, 6);


    //Create two thread objects on the heap for demonstration
    pthread_t* thread1 = new pthread_t();
    pthread_t* thread2 = new pthread_t();

    // Initialise the thread with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(thread1, NULL , thread_routine, semaphore);
    pthread_create(thread2, NULL , thread_routine, semaphore);

    // wait for the new thread to join, ignoring any return value
    pthread_join(*thread1, NULL);
    pthread_join(*thread2, NULL);
}