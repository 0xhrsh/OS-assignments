#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include "classes.cpp"
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define nl cout<<endl
#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)

#define MAX_BRBRS 100
#define MAX_CSTMRS 100

#define WAITING_OUTSIDE 0
#define WAIT_ROOM 1
#define SITTING_ON_COUCH 2
#define BRBR_CHAIR 3
#define CLEANED_CHAIR 4
#define READY_TO_PAY 5
#define PAYMENT_ACCEPTED 6
#define OUTSIDE 7

Gatekeeper g;

int capacity;

int n_brbrs=-1, n_chrs=-1, n_wtRoom=-1, n_cstmrs=-1;

int brbrs_free;

int cstmrStatus[MAX_CSTMRS]; // 0 in chair, 1 cutting done, 2 payment done
int cstmrBRBR[MAX_CSTMRS];
int brbrStatus[MAX_BRBRS]; 
// 0 sleeping, 1 cutting hair, 2 cleaning, 3 collecting money
queue<int> outsideQ;
queue<int> waitingRoomQ;
queue<int> couchQ;

const int INITIAL_VALUE = 1;
sem_t semCouch, semOutside, semWaitingRoom, semPayment, semCSTMR_BRBR, semClean, semCstmrStatus;

void* initCustomer(void* ptr){
    int id = *(int *)(&ptr);
    Customer c(id);

    // sem_wait(&semOutside);
    // outsideQ.push(id);
    // sem_post(&semOutside);

    // while(!(g.tokens_issued < capacity && outsideQ.front() == id))continue;

    // sem_wait(&semOutside);
    // outsideQ.pop();
    // sem_post(&semOutside);

    g.giveToken(id);
    c.enterShop();
    
    sem_wait(&semWaitingRoom);
    waitingRoomQ.push(id);
    sem_post(&semWaitingRoom);

    // while(!(couchQ.size() < n_chrs && waitingRoomQ.front() == id))continue;

    sem_wait(&semWaitingRoom);
    waitingRoomQ.pop();
    sem_post(&semWaitingRoom);

    sem_wait(&semCouch);
    c.sitOnSofa();
    sem_post(&semCouch);

    while(cstmrStatus[id] == SITTING_ON_COUCH) continue;

    sem_wait(&semCouch);
    couchQ.pop();
    sem_post(&semCouch);

    while(cstmrBRBR[id]==-1) continue;
    int myBRBR = cstmrBRBR[id];
    c.sitInBarberChair(myBRBR);

    c.waitforPayment();
    while(cstmrStatus[id]!=CLEANED_CHAIR)continue;

    sem_wait(&semCstmrStatus);
    cstmrStatus[id] = READY_TO_PAY;
    sem_post(&semCstmrStatus);

    c.pay(myBRBR);
    while(cstmrStatus[id]!=PAYMENT_ACCEPTED)continue;

    c.exitShop();
}

void* initBarber(void* ptr){
    int id = *(int *)(&ptr);
    Barber b(id);
    
    b.sleep();
    while(true){
        // wait for customer to show up
        sem_wait(&semCouch);
        if(couchQ.empty()){
            sem_post(&semCouch);
            continue;
        }
        int cstmr = couchQ.front();
        couchQ.pop();
        sem_post(&semCouch);

        sem_wait(&semCstmrStatus);
        cstmrStatus[cstmr]=BRBR_CHAIR;
        sem_post(&semCstmrStatus);

        sem_wait(&semCSTMR_BRBR);
        cstmrBRBR[cstmr] = id;
        sem_post(&semCSTMR_BRBR);

        b.cutHair(cstmr);

        sem_wait(&semClean);
        b.cleanChair();
        sem_post(&semClean);

        sem_wait(&semCstmrStatus);
        cstmrStatus[cstmr]=CLEANED_CHAIR;
        sem_post(&semCstmrStatus);

        while(cstmrStatus[cstmr]!=READY_TO_PAY)continue;

        sem_wait(&semPayment);
        b.acceptPayment(cstmr);
        sem_post(&semPayment);

        sem_wait(&semCstmrStatus);
        cstmrStatus[cstmr]=PAYMENT_ACCEPTED;
        sem_post(&semCstmrStatus);

        b.sleep();
    }
}

void initGatekeeper(Gatekeeper* g){
    while(true){
        // see if any customer paid
        int cstmr;
        g->takeToken(cstmr);
    }
    return;
}

int main(int argc, char *argv[]){

    sem_init(&semCouch, 0, INITIAL_VALUE);
    sem_init(&semOutside, 0, INITIAL_VALUE);
    sem_init(&semPayment, 0, INITIAL_VALUE);
    sem_init(&semWaitingRoom, 0, INITIAL_VALUE);

    rep(i, 1, argc){
        if(argv[i][0]!='-')continue;
        switch (argv[i][1]){
        case 'b':
            n_brbrs = atoi(argv[i]+3);
            break;
        case 'c':
            n_chrs = atoi(argv[i]+3);
            break;
        case 'w':
            n_wtRoom = atoi(argv[i]+3);
            break;
        default:
            cerr<<"Incorrect flag: "<<argv[i];
            break;
        }
    }
    cout<<n_brbrs<<" "<<n_chrs<<" "<<n_wtRoom;nl;

    cout<<"Enter the number of customers: ";
    cin>>n_cstmrs;
    cout<<n_cstmrs;nl;
    // input done
    pthread_t brbrs[n_brbrs];
    repp(i,n_brbrs){
        brbrs[i] = pthread_t();
        pthread_create( &brbrs[i], NULL, initBarber,(void*) i);
    }

    pthread_t cstmrs[n_cstmrs];
    repp(i,n_cstmrs){
        cstmrs[i] = pthread_t();
        pthread_create( &cstmrs[i], NULL, initCustomer,(void*) i);
    }

    // initGatekeeper(&g);

    repp(k, 10){
        sem_wait(&semCouch);
        couchQ.push(k+2);
        sem_post(&semCouch);
    }

    repp(i, n_brbrs)
        pthread_join(brbrs[i], NULL);
    repp(i, n_cstmrs)
        pthread_join(cstmrs[i], NULL);

    return 0;
}