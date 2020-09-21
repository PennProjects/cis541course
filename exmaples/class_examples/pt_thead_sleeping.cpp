#include <pthread.h> //Needed for thread functionality
#include <time.h> // Implicetely already included from pthread.h
#include <iostream> // Needed for cout/endl

//Function to be run on separate threads
// Must have single "void*" parameter and return "void*"
//Thread is stopped / detroyed when function exists
void* thread_routine(void* args) {

    //Create a struct for storing time
    timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 0;


    //Sleep this thread for 1 sec
    // If we are interrupted we do not care
    nanosleep(&sleep_time, NULL);


    std::cout << "HELLO AFTER SLEEPING!" << std::endl;
}

int main(int argc, char** argv){

    //Create a thread object on the heap for demonstration
    pthread_t* thread = new pthread_t();

    // Initialise the thread with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(thread, NULL , thread_routine, NULL);

    // wait for the new thread to join, ignoring any return value
    pthread_join(*thread, NULL);
}