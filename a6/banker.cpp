#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v))
#define repp(i,n) for(int i=0;i<n;i++)  
#define nl cout<<endl

#define N_MAX 1000
#define M_MAX 1000

int need[N_MAX][M_MAX];
int alloc[N_MAX][M_MAX];
int avail[M_MAX];

void isSafe(int n, int m){
    bool finished[n];
    mem(finished, 0);

    vector<int> seq;

    repp(k, n){
        repp(i, n){
            if (finished[i]) continue;      

            bool flag = true;
            repp(j, m){
                if (need[i][j] > avail[j]){
                    flag = false;
                    break;
                }
            }

            if(flag){
                seq.push_back(i);
                repp(t, m)
                    avail[t] += alloc[i][t];
                finished[i] = true;
            }       
        }
    }

    bool flag = true;
    repp(i, n) flag = finished[i] && flag;

    if(flag){
        cout<<"Input State is Safe.\nSafe Sequence: ";
        for(auto& it : seq)
            cout <<it<<" ";
        nl;
    } else{
        cout<<"Input State is not Safe.\n";
    }
    nl;
    return;
}


int main(){  
    int n, m;
    cout<<"Enter the number of processes: ";
    cin>>n;
    
    cout<<"Enter the number of resources: ";
    cin>>m;
    
    // int alloc[n][m];
    int max[n][m];
    int old_avail[m];
    // int need[n][m];

    cout<<"Enter the amount available for each of "<<m<<" resources (Space Seperated)\n";
    repp(i, m) cin>>avail[i];
    repp(i, m) old_avail[i] = avail[i];
    nl;

    repp(i, n){
        cout<<"Enter the amount of each resouce currently allocated to process "<<i<<" (space seperated)"<<endl;
        repp(j, m) cin>>alloc[i][j];
    }
    nl;
    repp(i, n){
        cout<<"Enter the max amount of each resouce process "<<i<<" needs for completion (space seperated)"<<endl;
        repp(j, m) cin>>max[i][j];
    }   
    nl;

    repp(i, n) repp(j, m) need[i][j] = max[i][j] - alloc[i][j];

    isSafe(n, m);

    repp(i, m) avail[i] = old_avail[i];

    int pi;
    cout<<"Enter the process id (0 indexed) for new process: ";
    cin>>pi;

    int request[m];
    cout<<"\nEnter the number of instances required for each resource type (space seperated): \n";
    repp(i, m) cin>>request[i];
    nl;
    repp(i, m)if(request[i]>need[pi][i]){cerr<<"Request more than need for resource index: "<<i;nl;exit(1);}
    repp(i, m)if(request[i]>avail[i]){cerr<<"Request more than available for resource index: "<<i;nl;exit(1);}

    repp(i,m){
        avail[i] -= request[i];
        need[pi][i] -= request[i];
        alloc[pi][i] +=request[i];
    }

    isSafe(n, m);

    return (0);
}