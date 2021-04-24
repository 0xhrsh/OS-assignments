#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v))
#define repp(i,n) for(int i=0;i<n;i++)  
#define nl cout<<endl

int main(){  
    int n, m;
    cout<<"Enter the number of processes: ";
    cin>>n;
    
    cout<<"Enter the number of resources: ";
    cin>>m;
    
    int alloc[n][m];
    int max[n][m];
    int avail[m];
    int need[n][m];

    cout<<"Enter the amount available for each of "<<m<<" resources (Space Seperated)\n";
    repp(i, m) cin>>avail[i];
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

    vector<int> seq;

    bool finished[n];
    mem(finished, 0);

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

    cout <<"safe sequence: ";
    for(auto& it : seq)
        cout <<it<<" ";
    nl;
  
    return (0);
}