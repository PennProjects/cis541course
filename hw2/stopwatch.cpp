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
pthread_mutex_t* mutex_ds;
pthread_mutex_t* mutex_s;
pthread_mutex_t* mutex_m;

//global variable to save timer values
int ds =0,s=0,m=0;
char c;


//thread to print timer
void* print_timer(void* args){

    while(true){

    //delay print by 1 us
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000;

    //print MM:SS.d
    cout << "\r" << setw(2) << setfill('0') << m << ":"
                 << setw(2) << setfill('0') << s << "." 
                 << setw(1) << setfill(' ') << ds
                //  <<" " << c;
                 <<" " << c <<flush;
    
    //delay print by 1 us             
    nanosleep(&sleep_time, NULL);   

    }
}

//thread to read keyboard input
void* kb_read(void* args){

    while(true){
        
        //read keyboard input
        char prev_c =c;
        cin >>c;
        
        //ignore reset after start
        if(prev_c =='s' && c =='r'){
            c = 's';
        }

    }

}


//thread to calculate deciseconds
void* ds_timer(void* args) {

    while(true){

    // Lock mutex till cond is met
    pthread_mutex_lock(mutex_ds);

    //Create a struct for storing time
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 100000000;

    //Sleep this thread for 1 decisec 
    nanosleep(&sleep_time, NULL);
    if (c == 'S' ||  c== 's'){
        ds = ds+1;
    }
    if (c== 'P' || c== 'p'){
        ds = ds;
    }
    if (c== 'R' or c == 'r'){
        ds = 0;
    }
        
    if(ds >9) ds = 0;

    // unlock mutex
    pthread_mutex_unlock(mutex_ds);
    }

    
}


//thread to calculate seconds
void* s_timer(void* args) {

    while(true){

    // Lock mutex till cond is met
    pthread_mutex_lock(mutex_s);

    //Create a struct for storing time
    timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 0;

    //Sleep this thread for 1 decisec 
    nanosleep(&sleep_time, NULL);
    if (c == 'S' ||  c== 's'){
        s = s+1;
    }
    if (c== 'P' || c== 'p'){
        s = s;
    }
    if (c== 'R' or c == 'r'){
        s = 0;
    }
        
    if(s >59) s = 0;
        
    // unlock mutex
    pthread_mutex_unlock(mutex_s);
    }

}

//thread to calculate minutes
void* m_timer(void* args) {

    while(true){

    // Lock mutex till cond is met
    pthread_mutex_lock(mutex_m);

    //Create a struct for storing time
    timespec sleep_time;
    sleep_time.tv_sec = 60;
    sleep_time.tv_nsec = 0;

    //Sleep this thread for 1 decisec 
    nanosleep(&sleep_time, NULL);
    if (c == 'S' ||  c== 's'){
        m = m+1;
    }
    if (c== 'P' || c== 'p'){
        m = m;
    }
    if (c== 'R' or c == 'r'){
        m = 0;
    }
    
    if(m >59) m = 0;

    // unlock mutex
    pthread_mutex_unlock(mutex_m);
    }

}



int main(int argc, char** argv){

    //create a new mutex and condition for sec and mic
    mutex_ds = new pthread_mutex_t();
    mutex_s = new pthread_mutex_t();
    mutex_m = new pthread_mutex_t();
    
    
    //Initilaize mutex and condition
    pthread_mutex_init(mutex_ds,NULL);
    pthread_mutex_init(mutex_s,NULL);
    pthread_mutex_init(mutex_m,NULL);
    

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

    return 0;
}

        