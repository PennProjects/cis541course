#include <pthread.h> //Needed for thread functionality

//Function to be run on separate threads
// Must have single "void*" parameter and return "void*"
//Thread is stopped / detroyed when function exists
void* thread_routine(void* args) {
    //Cast void* argument back to mutex*
    pthread_mutex_t* mutex  = (pthread_mutex_t*) args;

    /// Lock the mutex, blocking if its unavailable
    // ptread_mutex_trylock(mutex) can be used  if we did not want to block
    pthread_mutex_lock(mutex);

    //Access some shared resource 

    //Unlock the mutex
    pthread_mutex_unlock(mutex);

}

int main(int argc, char** argv){

    //Create a neew mutex object and initiate it with default values
    pthread_mutex_t* mutex = new pthread_mutex_t();
    pthread_mutex_init(mutex, NULL);


    //Create teo thread objects on the heap for demonstration
    pthread_t* thread1 = new pthread_t();
    pthread_t* thread2 = new pthread_t();

    // Initialise the thread with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(thread1, NULL , thread_routine, mutex);
    pthread_create(thread2, NULL , thread_routine, mutex);

    // wait for the new thread to join, ignoring any return value
    pthread_join(*thread1, NULL);
    pthread_join(*thread2, NULL);
}