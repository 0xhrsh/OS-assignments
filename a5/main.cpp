#include <bits/stdc++.h>
#include "classes.cpp"
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define nl cout<<endl
#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)




void initCustomer(int id){
    Customer c(id);

    while(true){
        
    }
    return;
}

void initBarber(int id){
    return;
}

void initGatekeeper(int id){
    return;
}

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_brbrs=-1, n_chrs=-1, n_wtRoom=-1, n_cstmrs=-1;

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
    initGatekeeper(0);
    
    
    return 0;
}