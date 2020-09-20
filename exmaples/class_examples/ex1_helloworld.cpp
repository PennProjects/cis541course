#include <iostream>
#include <pthread.h>

using namespace std;

void* helloworld( void *args){
    cout << "Hello World!" << endl;
    return NULL;
}

int main(){
    pthread_t threads[2];
    for(int i =0;i < 2 ; i++){
        pthread_create(&threads[i], NULL, helloworld, NULL);
    }
    for(int i = 0;i<2;i++){
        pthread_join(threads[i],NULL);
    }
}