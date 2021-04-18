#include <bits/stdc++.h>
#include "classes.cpp"
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define nl cout<<endl
#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)

#define MAX_BRBRS 100

Gatekeeper g;

int capacity;

int n_brbrs=-1, n_chrs=-1, n_wtRoom=-1, n_cstmrs=-1;

int brbrs_free;

int brbrStatus[MAX_BRBRS]; 
// 0 sleeping, 1 cutting hair, 2 cleaning, 3 collecting money
queue<int> outsideQ;
queue<int> waitingRoomQ;



void initCustomer(int id){
    Customer c(id);

    outsideQ.push(id);
    while(!(g.tokens_issued < capacity && outsideQ.front() == id))continue;
    outsideQ.pop();

    g.giveToken(id);
    c.enterShop();
    
    waitingRoomQ.push(id);
    while(!(couchQ.size() < n_chrs && waitingRoomQ.front() == id))continue;
    waitingRoomQ.pop();

    c.sitOnSofa();
    while(!(brbrs_free >0 && couchQ.front() == id))continue;
    couchQ.pop();

    repp(i,n_brbrs)
        if(brbrStatus[i] == 0){
            c.sitInBarberChair(i);
            c.waitforPayment();
            c.pay(i);
            break;
        }
    c.exitShop();
    return;
}

void initBarber(int id){
    Barber b(id);
    
    b.sleep();
    while(true){
        // wait for customer to show up
        int cstmr;
        b.cutHair(cstmr);
        b.cleanChair();
        // wait for customer to pay
        b.acceptPayment(cstmr);
        b.sleep();
    }

    return;
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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

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
    cerr<<n_brbrs<<" "<<n_chrs<<" "<<n_wtRoom;el;

    cout<<"Enter the number of customers: ";
    cin>>n_cstmrs;
    // input done

    repp(i,n_brbrs){
        initBarber(i);
    }
    repp(i,n_cstmrs){
        initCustomer(i);
    }
    initGatekeeper(&g);

    return 0;
}