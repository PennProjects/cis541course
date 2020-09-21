
#include <pthread.h> //Needed for thread functionality
#include <time.h> // Implicetely already included from pthread.h
#include <iostream> // Needed for cout/endl
#include <iomanip> //Needed for formatting cout
#include <termios.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h> 

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

int keyboardhit(void) 
{ 
	if(kbhit())

	ch = getchar(); 

	if (ch != EOF) { 
		ungetc(ch, stdin); 
		return 1; 
	} 

	return 0; 
}

//thread to print timer
void* print_timer(void* args){

    while(true){
    
    // Lock mutex till cond is met
    pthread_mutex_lock(mutex);

    //print MM:SS.d
    // cout << "\r" << setw(2) << setfill('0') << m << ":"
    //              << setw(2) << setfill('0') << s << "." 
    //              << setw(1) << setfill(' ') << ds
    //              <<" " << c<< flush;

    cout << "\r" <<"Hi" <<c << flush;
    if(keyboardhit()){
        c = getchar();
    }

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



int main(int argc, char** argv){

    //create a new mutex and condition for sec and mic
    mutex = new pthread_mutex_t(); 

    pthread_t* print_thread = new pthread_t();
    // pthread_t* kb_thread = new pthread_t();


    // Initialise the thread with the default attributes,
    pthread_create(print_thread, NULL, print_timer, mutex);
    // pthread_create(kb_thread, NULL , kb_read, mutex);

    // wait for the new thread to join, ignoring any return values
    pthread_join(*print_thread, NULL);


    return 0;
}