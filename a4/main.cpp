#include <bits/stdc++.h>
#include <pthread.h>

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9
#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

using namespace std;

#define M 100
#define RAND_MAX 100
#define GENERATION_LIMIT 10
int buffer[M];
int bufferFill = 0;






void *producer(void *ptr){
    int tn = *(int *)(&ptr);

    int numsProduced = 0;
    cout<<"Producer Thread. Tnum: "<<tn<<endl; // print this to buffer
    
    while(true){
        if(numsProduced == GENERATION_LIMIT){
            break;
        } if(bufferFill == M){
            continue;
        } else{
            buffer[bufferFill++] = rand();
            numsProduced++;
        }
    }
        
    
    return NULL; 
}

void *consumer(void *ptr){
    int tn = *(int *)(&ptr);

    int numsConsumed = 0;
    cout<<"Consumer Thread. Tnum: "<<tn<<endl; // print this to buffer

    while(true){
        if(numsConsumed == GENERATION_LIMIT){
            break;
        } else if(bufferFill == 0){
            continue;
        } else{
            int num = buffer[--bufferFill];
            numsConsumed++;
        }
    }
    return NULL; 
}

int main(){
    int n;
    cout<<"Enter n: ";
    cin>>n;

    bool thread_type[n]; // 0 is producer, 1 is consumer
    pthread_t threads[n];
    int threads_ret[n];

    repp(i,n){
        thread_type[i] = rand()%2;  
        threads[i] = pthread_t();

        if(thread_type[i] == 0){    
            threads_ret[i] = pthread_create( &threads[i], NULL, producer,(void*) i);
        } else {
            threads_ret[i] = pthread_create( &threads[i], NULL, consumer,(void*) i);
        }
    }

    repp(i,n){
        pthread_join( threads[i], NULL); 
    }

    

    

    
    return 0;
}