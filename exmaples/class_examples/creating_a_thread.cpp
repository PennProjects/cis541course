#include <pthread.h> //Needed for thread functionality
#include <iostream> // Needed for cout/endl

//Function to be run on separate threads
// Must have single "void*" parameter and return "void*"
//Thread is stopped / detroyed when function exists
void* thread_routine(void* args) {
    std::cout << "HELLO" << std::endl;
}

int main(int argc, char** argv){

    //Create a thread object
    pthread_t thread;

    // InitiLISE THE THREAD with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(&thread, NULL , thread_routine, NULL);

    // wait for the new thread to join, ignoring any return value
    pthread_join(thread, NULL);
}