
#include <pthread.h> //Needed for thread functionality
#include <time.h> // Implicetely already included from pthread.h
#include <iostream> // Needed for cout/endl
#include <iomanip> //Needed for formatting cout

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::setw;
using std::setfill;

//Use global variables for brevity
pthread_mutex_t* mutex;
pthread_cond_t* sec;
pthread_cond_t* min;

//global variable to save timer values
int ds =0,s=0,m=0;
char c;

//thread to print timer
void* print_timer(void* args){

    while(true){
    
    // Lock mutex till cond is met
    pthread_mutex_lock(mutex);

    //print MM:SS.d
    cout << "\r" << setw(2) << setfill('0') << m << ":"
                 << setw(2) << setfill('0') << s << "." 
                 << setw(1) << setfill(' ') << ds
                 <<" " << c<< flush;

    // unlock mutex
    pthread_mutex_unlock(mutex);

    }

    
}

//thread to read from keyboard
void* kb_read(void* args){

    while(true){
        std::cin >> c;
    }
}

//thread to calculate deciseconds
void* ds_timer(void* args) {

    while(true){

    // Lock mutex till cond is met
    pthread_mutex_lock(mutex);

    //Create a struct for storing time
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 100000000;

    //Sleep this thread for 1 decisec 
    nanosleep(&sleep_time, NULL);
    ds = ds+1;

    //to signal that 10ds are completed and s can be incremented
    if (ds > 9) {
        pthread_cond_signal(sec);
        ds = 0;
        
    }

    // unlock mutex
    pthread_mutex_unlock(mutex);
    }

    
}


//thread to calculate seconds
void* s_timer(void* args) {

    while(true){
        //Lock mutex till cond is met
        pthread_mutex_lock(mutex);

        //wait till ds thread signals 10ds completed
        while(ds <= 9){ 
        pthread_cond_wait(sec,mutex);
        }
        s = s+1; //increment s after 10ds wait

        //to signal that 60s are completed and m can be incremented
        if (s >5){
            pthread_cond_signal(min);
            s = 0;
        }

        //unlock mutex
        pthread_mutex_unlock(mutex);
    }

}

//thread to calculate minutes
void* m_timer(void* args) {

    while(true){
        //Lock mutex till cond is met
        pthread_mutex_lock(mutex);

        //wait till s thread signals 59s completed
        while(s <= 5){ 
        pthread_cond_wait(min,mutex);
        }
        m = m+1;

        //reset min after 59m
        if (m >5){
            m = 0;
        }

        //unlock mutex
        pthread_mutex_unlock(mutex);
    }

}



int main(int argc, char** argv){

    //create a new mutex and condition for sec and mic
    mutex = new pthread_mutex_t();
    sec = new pthread_cond_t();
    min = new pthread_cond_t();
    

    //Create a thread object on the heap
    pthread_t* ds_thread = new pthread_t();
    pthread_t* s_thread = new pthread_t();
    pthread_t* m_thread = new pthread_t();
    pthread_t* print_thread = new pthread_t();
    pthread_t* kb_thread = new pthread_t();

    // Initialise the thread with the default attributes,
    pthread_create(print_thread, NULL, print_timer, mutex);
    // pthread_create(kb_thread, NULL , kb_read, mutex);
    pthread_create(ds_thread, NULL , ds_timer, mutex);
    pthread_create(s_thread, NULL , s_timer, mutex);
    pthread_create(m_thread, NULL, m_timer, mutex);

    // wait for the new thread to join, ignoring any return values
    pthread_join(*ds_thread, NULL);
    pthread_join(*s_thread, NULL);
    pthread_join(*m_thread, NULL);

    return 0;
}