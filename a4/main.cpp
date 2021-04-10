#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>


#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9
#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

using namespace std;

#define M 100
#define GENERATION_LIMIT 100
#define N_MAX 100

#define SLEEP 0
#define ACTIVE 1
#define DEAD 2

int buffer[M];
int bufferFill = 0, producersActive = 0, consumersActive = 0;
int status[N_MAX]; // 0, 1, 2 -> sleep, active, dead
bool statusLocked = true;
int activeWorker = -1;
bool thread_type[N_MAX]; // 0 is producer, 1 is consumer

queue<int> readyQ;

void signal_handler(int sigNum){
	if(sigNum == SIGUSR1) // sleep if not dead
		status[activeWorker]=(status[activeWorker]==DEAD)?DEAD:SLEEP;
	if(sigNum == SIGUSR2) // wake up the thread if not dead
		status[activeWorker]=(status[activeWorker]==DEAD)?DEAD:ACTIVE;
}


void *producer(void *ptr){
    signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);

    producersActive++;
    int tn = *(int *)(&ptr);
    cout<<"Producer Thread. Tnum: "<<tn<<endl;
    readyQ.push(tn);
    
    int numsProduced = 0;
    while(true){
        if(numsProduced == GENERATION_LIMIT){
            while(statusLocked);
            statusLocked = true;
            status[tn] = DEAD;
            statusLocked = false;
            producersActive--;
            cout<<"==>Producer: "<<tn<<" Killed because it reached limit."<<bufferFill<<"<--"<<endl;
            break;
        } 
        if(status[tn]==SLEEP) continue;
        while(bufferFill == M);
        
        
        buffer[bufferFill] = rand()%100;
        // cerr<<"\tNumber"<<buffer[bufferFill]<<" Entered into buffer: "<<tn<<endl;
        bufferFill++;
        numsProduced++;
    }
    return NULL; 
}

void *consumer(void *ptr){
    signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);

    consumersActive++;
    int tn = *(int *)(&ptr);
    cout<<"Consumer Thread. Tnum: "<<tn<<endl;
    readyQ.push(tn);
    
    int numsConsumed = 0;    

    while(true){
        if(numsConsumed == GENERATION_LIMIT){
            while(statusLocked);
            statusLocked = true;
            status[tn] = DEAD;
            statusLocked = false;
            consumersActive--;
            cout<<"==>Consumer: "<<tn<<" Killed because it reached limit. "<<bufferFill<<"<--"<<endl;
            break;
        }   
        
        if(status[tn]==SLEEP)continue;
        while(bufferFill == 0);

        if(bufferFill==0)cerr<<"Fishy\n";
        int num = buffer[--bufferFill];
        // cerr<<"\tNumber"<<num<<" Read from buffer: "<<tn<<endl;
        numsConsumed++;   
    }
    return NULL; 
}

void *scheduler(void *ptr){

    pthread_t *workers = (pthread_t *)ptr;
    sleep(2);
    while(true){
        if((producersActive == 0 && consumersActive == 0) || readyQ.empty()){
            cout<<"All processes are over, killing scheduler"<<endl;
            break;
        }

        activeWorker =  readyQ.front();
        readyQ.pop();
        if(status[activeWorker] == DEAD) continue;
        readyQ.push(activeWorker);

        cout<<"\nContext Switching to : "<<activeWorker<<" ("<<(thread_type==0?"Producer":"Consumer")<<")"<<endl;
        cout<<"\tActive Producers: "<<producersActive<<" Active Consumers: "<<consumersActive<<endl;
        cout<<"before\tNumber of Elements in buffer: "<<bufferFill<<endl;
        
        pthread_kill(workers[activeWorker], SIGUSR2);
        sleep(1);
        pthread_kill(workers[activeWorker], SIGUSR1);
        // sleep(1);
        cout<<"after\tNumber of Elements in buffer: "<<bufferFill<<endl;

    }
    return NULL;
}

int main(){
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    int n;
    cerr<<"Enter n: ";
    cin>>n;

    
    pthread_t workers[n];
    int threads_ret[n];

    mem(status, SLEEP);
    statusLocked = false;

    repp(i,n){
        // thread_type[i] = rand()%2;
        thread_type[i] = i%2;
        
        workers[i] = pthread_t();

        if(thread_type[i] == 0){    
            threads_ret[i] = pthread_create( &workers[i], NULL, producer,(void*) i);
        } else {
            threads_ret[i] = pthread_create( &workers[i], NULL, consumer,(void*) i);
        }
    }

    pthread_t tscheduler;
    pthread_create( &tscheduler, NULL, scheduler, (void*)workers);

    repp(i,n){
        pthread_join(workers[i], NULL); 
    }

    pthread_join(tscheduler, NULL);

    return 0;
}
