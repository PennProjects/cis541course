#include <pthread.h> //Needed for thread functionality
//#include <time.h> // Implicetely already included from pthread.h
#include <iostream> // Needed for cout/endl
#include <iomanip> //Needed for formatting cout

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::setw;
using std::setfill;

//Use global variables for brevity
pthread_mutex_t* mutex_ds;
pthread_mutex_t* mutex_s;
pthread_mutex_t* mutex_m;
pthread_cond_t* sec;
pthread_cond_t* min;

//global variable to save timer values
int ds =0,s=0,m=0;
char c;


//thread to print stopwatch
[[noreturn]] void* print_timer(void* args){

    while(true){

    //delay print by 10 ms
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 10000000;

    //print MM:SS.d <cmd>
    cout << "\r" << setw(2) << setfill('0') << m << ":"
                 << setw(2) << setfill('0') << s << "." 
                 << setw(1) << setfill(' ') << ds
                //  <<" " << c;
                 <<" " << c <<flush;
    
    //delay print by 10 ms             
    nanosleep(&sleep_time, NULL);   

    }
}

//thread to read keyboard input
void* kb_read(void* args){

    while(true){
        
        //read keyboard input
        char prev_c =c;
        cin >>c;
        
        //Ignore a Reset after Start
        if((prev_c =='s' || prev_c =='S') && (c =='r' || c =='R')){
            c = 's';
        }
    }
}


//thread to calculate deciseconds
void* ds_timer(void* args) {

    while(true){

        // Lock mutex during execution
        pthread_mutex_lock(mutex_ds);

        //Create a struct for storing time
        timespec sleep_time;
        sleep_time.tv_sec = 0;
        sleep_time.tv_nsec = 100000000;

        //Sleep this thread for 1 decisec 
        nanosleep(&sleep_time, NULL);
        
        //To handle start, pause and reset operations
        if (c == 'S' ||  c== 's'){
            ds = ds+1;
        }
        if (c== 'P' || c== 'p'){
            ds = ds;
        }
        if (c== 'R' or c == 'r'){
            ds = 0;
            s = 0;
            m = 0;
        }

        //to signal that 10ds are completed and s can be incremented
        if (ds > 9) {
            pthread_cond_broadcast(sec);
            ds = 0;
        
        }

        // unlock mutex
        pthread_mutex_unlock(mutex_ds);
    }

    
}


//thread to calculate seconds
void* s_timer(void* args) {

    while(true){
        //Lock mutex till cond is met
        pthread_mutex_lock(mutex_s);

        //wait till ds thread signals 10ds completed
        pthread_cond_wait(sec,mutex_s);
        
        s = s+1; //increment s after 10ds wait

        //to signal that 60s are completed and m can be incremented
        if (s >59){
            pthread_cond_broadcast(min);
            s = 0;
        }

        //unlock mutex
        pthread_mutex_unlock(mutex_s);
    }

}

//thread to calculate minutes
void* m_timer(void* args) {

    while(true){
        //Lock mutex till cond is met
        pthread_mutex_lock(mutex_m);

        //wait till s thread signals 59s completed
        pthread_cond_wait(min,mutex_m);
        
        m = m+1;//increment m after 60s wait
        
        //reset min after 59m
        if (m >59){
            m = 0;
        }
        
        //unlock mutex
        pthread_mutex_unlock(mutex_m);
    }

}


int main(int argc, char** argv){

    //create a new mutex for ds, s and m threads and condition for sec and mic condition
    mutex_ds = new pthread_mutex_t();
    mutex_s = new pthread_mutex_t();
    mutex_m = new pthread_mutex_t();
    sec = new pthread_cond_t();
    min = new pthread_cond_t();
    
    //Initilaize mutex and condition
    pthread_mutex_init(mutex_ds,NULL);
    pthread_mutex_init(mutex_s,NULL);
    pthread_mutex_init(mutex_m,NULL);
    pthread_cond_init(sec, NULL);
    pthread_cond_init(min, NULL);

    //Create a thread object on the heap
    pthread_t* ds_thread = new pthread_t();
    pthread_t* s_thread = new pthread_t();
    pthread_t* m_thread = new pthread_t();
    pthread_t* print_thread = new pthread_t();
    pthread_t* kb_thread = new pthread_t();

    // Initialise the thread with the default attributes,
    pthread_create(print_thread, NULL, print_timer, NULL);
    pthread_create(kb_thread, NULL , kb_read, NULL);
    pthread_create(ds_thread, NULL , ds_timer, NULL);
    pthread_create(s_thread, NULL , s_timer, NULL);
    pthread_create(m_thread, NULL, m_timer, NULL);

    // wait for the new thread to join, ignoring any return values
    pthread_join(*ds_thread, NULL);
    pthread_join(*s_thread, NULL);
    pthread_join(*m_thread, NULL);
    pthread_join(*print_thread, NULL);
    pthread_join(*kb_thread, NULL);
    
    return 0;
}

