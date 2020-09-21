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
    
    int* type = (int*) args;
    while(true){
    
 
    // Lock mutex till cond is met
    pthread_mutex_lock(mutex);
        

    //Create a struct for storing time
    
    if(*type == 0) { 
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
    }

    if(*type == 1) { 
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
    }
    if(*type == 2) { 
        timespec sleep_time;
        sleep_time.tv_sec = 60;
        sleep_time.tv_nsec =0;

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
    }
        
    

    // unlock mutex
    pthread_mutex_unlock(mutex);
    }

    
}



int main(int argc, char** argv){

    //create a new mutex and condition for sec and mic
    mutex = new pthread_mutex_t();
    sec = new pthread_cond_t();
    min = new pthread_cond_t();
    
    //Initialize mutex and Cond
    pthread_mutex_init(mutex,NULL);
    pthread_cond_init(sec, NULL);
    pthread_cond_init(min, NULL);

    //Create a thread object on the heap
    pthread_t timer_thread[3];
    pthread_t* ds_thread = new pthread_t();
    pthread_t* s_thread = new pthread_t();
    pthread_t* m_thread = new pthread_t();
    pthread_t* print_thread = new pthread_t();
    pthread_t* kb_thread = new pthread_t();

    // Initialise the thread with the default attributes,
    pthread_create(print_thread, NULL, print_timer, NULL);
    pthread_create(kb_thread, NULL , kb_read, NULL);

    int temp[3];
    for(int i = 0; i <3; i++){
        temp[i] = i;
        pthread_create(&timer_thread[i], NULL , ds_timer,&temp[i]);
    }
    // wait for the new thread to join, ignoring any return values
    pthread_join(*ds_thread, NULL);
    pthread_join(*s_thread, NULL);
    pthread_join(*m_thread, NULL);
    pthread_join(*print_thread, NULL);
    pthread_join(*kb_thread, NULL);
    pthread_join(*timer_thread, NULL);

    return 0;
}





