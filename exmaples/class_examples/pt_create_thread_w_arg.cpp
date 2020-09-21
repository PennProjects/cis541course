#include <pthread.h> //Needed for thread functionality
#include <iostream> // Needed for cout/endl

//Function to be run on separate threads
// Must have single "void*" parameter and return "void*"
//Thread is stopped / detroyed when function exists
void* thread_routine(void* args) {

    //cast the void* argument to the char* thst we know it is 
    char* world = (char*) args;

    std::cout <<"HELLO "<< world << std::endl;
}

int main(int argc, char** argv){

    //Create a thread object on the heap for demonstration
    pthread_t* thread = new pthread_t();

    //Collect the argument for our function
    char* arg = argv[1];

    // Initialise the thread with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(thread, NULL , thread_routine, arg);

    // wait for the new thread to join, ignoring any return value
    pthread_join(*thread, NULL);
}