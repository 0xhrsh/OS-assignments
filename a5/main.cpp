#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include "classes.cpp"
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)

#define MAX_BRBRS 500
#define MAX_CSTMRS 500

#define CUST_SLEEP_TIME_LIMIT 10

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
int cstmrBRBR[MAX_BRBRS];


queue<int> outsideQ;
queue<int> waitingRoomQ;
queue<int> couchQ;
queue<int> leaveQ;

const int INITIAL_VALUE = 1;
sem_t semCouch, semOutside, semWaitingRoom, semPayment, semCSTMR_BRBR, semClean, semCstmrStatus, semLeave;

void* initCustomer(void* ptr){
    // std::this_thread::sleep_for (std::chrono::seconds(rand()%CUST_SLEEP_TIME_LIMIT));
    int id = *(int *)(&ptr);
    Customer c(id);

    sem_wait(&semOutside);
    outsideQ.push(id);
    sem_post(&semOutside);

    while(cstmrStatus[id]!=WAIT_ROOM) continue;
    
    sem_wait(&semWaitingRoom);
    c.enterShop();
    waitingRoomQ.push(id);
    sem_post(&semWaitingRoom);

    while(!(waitingRoomQ.front() == id)) continue;

    while(true){
        if(couchQ.size() < n_chrs){
            sem_wait(&semCouch);
            couchQ.push(id);
            c.sitOnSofa();
            sem_post(&semCouch);

            sem_wait(&semWaitingRoom);
            waitingRoomQ.pop();
            sem_post(&semWaitingRoom);

            break;
        }
    }

    while(couchQ.front()!=id) continue;

    int myBRBR = -1;
    while(myBRBR==-1){
        repp(i, n_brbrs){
            if(cstmrBRBR[i]==-1){
                sem_wait(&semCSTMR_BRBR);
                cstmrBRBR[i] = id;
                sem_post(&semCSTMR_BRBR);
                
                myBRBR = i;
                c.sitInBarberChair(myBRBR);
                break;
            }
        }
    }

    sem_wait(&semCouch);
    couchQ.pop();
    sem_post(&semCouch);

    sem_wait(&semCstmrStatus);
    cstmrStatus[id] = BRBR_CHAIR;
    sem_post(&semCstmrStatus);
    

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

    while(cstmrStatus[id]!=CAN_EXIT)continue;
    c.exitShop();
}

void* initBarber(void* ptr){
    int id = *(int *)(&ptr);
    Barber b(id);
    bool awake = true;

    while(gateKeeperPresent){
        if(cstmrBRBR[id] == -1){ // wait for customer to show up
            if(awake) b.sleep();
            awake = false;
            continue;
        }
        awake = true;

        int myCstmr = cstmrBRBR[id];

        while(cstmrStatus[myCstmr]!=BRBR_CHAIR) continue;
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

        sem_wait(&semCSTMR_BRBR);
        cstmrBRBR[id] = -1;
        sem_post(&semCSTMR_BRBR);
    }
}

void initGatekeeper(int n_cstmrs){
    Gatekeeper g;
    while(n_cstmrs > 0){
        if((!outsideQ.empty()) && g.tokens_issued < n_wtRoom + n_chrs + n_brbrs){
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
    repp(i, MAX_BRBRS) cstmrBRBR[i] = -1;

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

    repp(i, n_cstmrs)
        pthread_join(cstmrs[i], NULL);
    cerr<<"All customers done\n";
    repp(i, n_brbrs)
        pthread_join(brbrs[i], NULL);   

    return 0;
}