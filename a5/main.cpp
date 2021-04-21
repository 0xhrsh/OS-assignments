#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include "classes.cpp"
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)

#define MAX_BRBRS 100
#define MAX_CSTMRS 100

#define WAITING_OUTSIDE 1
#define WAIT_ROOM 2
#define SITTING_ON_COUCH 3
#define BRBR_CHAIR 4
#define CLEANED_CHAIR 5
#define READY_TO_PAY 6
#define PAYMENT_ACCEPTED 7
#define CAN_EXIT 8
#define OUTSIDE 9

int n_brbrs=-1, n_chrs=-1, n_wtRoom=-1;
bool gateKeeperPresent = true;

int cstmrStatus[MAX_CSTMRS]; // 0 in chair, 1 cutting done, 2 payment done
int cstmrBRBR[MAX_CSTMRS];


queue<int> outsideQ;
queue<int> waitingRoomQ;
queue<int> couchQ;
queue<int> leaveQ;

const int INITIAL_VALUE = 1;
sem_t semCouch, semOutside, semWaitingRoom, semPayment, semCSTMR_BRBR, semClean, semCstmrStatus, semLeave;

void* initCustomer(void* ptr){
    int id = *(int *)(&ptr);
    Customer c(id);

    sem_wait(&semOutside);
    outsideQ.push(id);
    sem_post(&semOutside);

    while(cstmrStatus[id]==WAITING_OUTSIDE) continue;
    
    sem_wait(&semWaitingRoom);
    c.enterShop();
    waitingRoomQ.push(id);
    sem_post(&semWaitingRoom);

    while(!(waitingRoomQ.front() == id && couchQ.size() < n_chrs)) continue;

    sem_wait(&semWaitingRoom);
    waitingRoomQ.pop();
    sem_post(&semWaitingRoom);

    sem_wait(&semCouch);
    couchQ.push(id);
    c.sitOnSofa();
    
    sem_post(&semCouch);

    while(cstmrStatus[id] == SITTING_ON_COUCH) continue;

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

    sem_wait(&semLeave);
    c.leaveShop();
    leaveQ.push(id);
    sem_post(&semLeave);

    while(cstmrStatus[id] != CAN_EXIT)continue;
    c.exitShop();
}

void* initBarber(void* ptr){
    int id = *(int *)(&ptr);
    Barber b(id);
    bool awake = true;

    while(gateKeeperPresent){
        // wait for customer to show up
        if(couchQ.empty()){
            if(awake) b.sleep();
            continue;
        }
        awake = true;

        sem_wait(&semCouch);
        int myCstmr = couchQ.front();
        couchQ.pop();
        sem_post(&semCouch);

        sem_wait(&semCstmrStatus);
        cstmrStatus[myCstmr]=BRBR_CHAIR;
        sem_post(&semCstmrStatus);

        sem_wait(&semCSTMR_BRBR);
        cstmrBRBR[myCstmr] = id;
        // cerr<<endl<<cstmrBRBR[myCstmr]<<" with cut the hair of: "<<myCstmr<<endl<<endl;
        sem_post(&semCSTMR_BRBR);

        b.cutHair(myCstmr);

        sem_wait(&semClean);
        b.cleanChair();
        sem_post(&semClean);

        sem_wait(&semCstmrStatus);
        cstmrStatus[myCstmr]=CLEANED_CHAIR;
        sem_post(&semCstmrStatus);

        while(cstmrStatus[myCstmr]!=READY_TO_PAY)continue;

        sem_wait(&semPayment);
        b.acceptPayment(myCstmr);
        sem_post(&semPayment);

        sem_wait(&semCstmrStatus);
        cstmrStatus[myCstmr]=PAYMENT_ACCEPTED;
        sem_post(&semCstmrStatus);        
    }
}

void initGatekeeper(int n_cstmrs){
    Gatekeeper g;
    while(n_cstmrs > 0){
        if(!outsideQ.empty() && waitingRoomQ.size() < n_wtRoom){
            int nextCust;

            sem_wait(&semOutside);
            nextCust=outsideQ.front();
            outsideQ.pop();
            sem_post(&semOutside);

            g.giveToken(nextCust);

            sem_wait(&semCstmrStatus);
            cstmrStatus[nextCust] = WAIT_ROOM;
            sem_post(&semCstmrStatus);
        }

        if(!leaveQ.empty()){
            int leavingCust = leaveQ.front();
            if(cstmrStatus[leavingCust]==PAYMENT_ACCEPTED){
                g.verifyPayment(leavingCust);
            }
            g.takeToken(leavingCust);
            
            sem_wait(&semLeave);
            leaveQ.pop();
            sem_post(&semLeave);

            sem_wait(&semCstmrStatus);
            cstmrStatus[leavingCust] = CAN_EXIT;
            sem_post(&semCstmrStatus);
            
            n_cstmrs--;
        }
    }
    gateKeeperPresent = false;
    return;
}

int main(int argc, char *argv[]){

    sem_init(&semCouch, 0, INITIAL_VALUE);
    sem_init(&semOutside, 0, INITIAL_VALUE);
    sem_init(&semPayment, 0, INITIAL_VALUE);
    sem_init(&semWaitingRoom, 0, INITIAL_VALUE);
    sem_init(&semCSTMR_BRBR, 0, INITIAL_VALUE);
    sem_init(&semClean, 0, INITIAL_VALUE);
    sem_init(&semCstmrStatus, 0, INITIAL_VALUE);
    sem_init(&semLeave, 0, INITIAL_VALUE);
    sem_init(&semSTDOUT, 0, INITIAL_VALUE);

    mem(cstmrStatus, 0);
    repp(i, MAX_CSTMRS) cstmrBRBR[i] = -1;

    int n_cstmrs;

    cerr<<"Done with initialising\n";
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

    cerr<<"Enter the number of customers: ";
    cin>>n_cstmrs;

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

    initGatekeeper(n_cstmrs);

    repp(i, n_brbrs)
        pthread_join(brbrs[i], NULL);
    repp(i, n_cstmrs)
        pthread_join(cstmrs[i], NULL);

    return 0;
}