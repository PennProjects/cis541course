#include <pthread.h> //Needed for thread functionality


//Use global variables for brevity
pthread_mutex_t* mutex;
pthread_cond_t* cond;
int sharedCounter;

void* incrementer_routine(void* args){

    while(true){
        //Lock the mutex, blocking if it is unavailable
        pthread_mutex_lock(mutex);

        //Increment the shared counter
        sharedCounter += 1;

        if(sharedCounter > 0){
            //Let all waiting threads know the conddition has been met
            pthread_cond_broadcast(cond);
        }

        //Unlock the mutex
        pthread_mutex_unlock(mutex);

    }
}



void* decrementer_routine(void* args){

    while(true){
        //Lock the mutex, blocking if it is unavailable
        pthread_mutex_lock(mutex);

        //check the waiting condition, to guard it against spurous wakeups
        while(sharedCounter <= 0 ){
            //wait for the condtion to be met 
            // This releases the lock on the mutex
            pthread_cond_wait(cond, mutex);

            //After awaking from cond_wait, we hold the mutex lock again
        }

        //Decrement the shared counter
        sharedCounter -= 1;
        

        //Unlock the mutex
        pthread_mutex_unlock(mutex);

    }
}

int main(int argc, char** argv){

    //Create a new mutex and condition objects
    mutex = new pthread_mutex_t();
    cond = new pthread_cond_t();


    //Initialize mutex and condition objects with default settings
    pthread_mutex_init(mutex, NULL);
    pthread_cond_init(cond, NULL);


    //Create two thread objects on the heap for demonstration
    pthread_t* thread1 = new pthread_t();
    pthread_t* thread2 = new pthread_t();

    // Initialise the thread with the default attributes,
    // the "thread_routine" fuction, qnd no argumrnts
    pthread_create(thread1, NULL , incrementer_routine, mutex);
    pthread_create(thread2, NULL , decrementer_routine, mutex);

    // wait for the new thread to join, ignoring any return value
    pthread_join(*thread1, NULL);
    pthread_join(*thread2, NULL);
}