#include <bits/stdc++.h>
using namespace std;

#define repp(i,a) for(int i=0;i<(a);i++)
#define mem(v,i) memset(v,i,sizeof(v))

#define N_MAX 1000

int currTime;
bool isCpuBusy;
int currProc;
int procStarted;
int arr[10000][3];

void cpu(){
    if(currProc == -1){
        isCpuBusy = false;
    } else if(procStarted + arr[currProc][1] == currTime){
        isCpuBusy = false;
    } else{
        isCpuBusy = true;
    }    
    return;
}


void first_come_first_serve(int n){
    int tt[n], wt[n], rt[n];
    int ienter = 0;
    int metrics[n][4]; // enter the queue, ready, start, end
    mem(tt,0); mem(wt,0); mem(rt,0);

    queue<int> q;
    currTime = 0;
    isCpuBusy = false;
    currProc = -1;
    
    
    while(currTime<1500){

        while(arr[ienter][0] == currTime){
            metrics[ienter][0] = currTime;

            if(q.empty()){
                metrics[ienter][1] = currTime;
            }

            q.push(ienter);
            ienter++;
        }

        cpu();

        if(!isCpuBusy && currProc != -1){
            metrics[currProc][3] = currTime;


            if(!q.empty()){
                    metrics[q.front()][2] = currTime;
                    currProc = q.front();
                    procStarted = currTime;
                    q.pop();

                    if(!q.empty()){
                        metrics[q.front()][1] = currTime;
                    }
            }
        } else if(!isCpuBusy){
            if(!q.empty()){
                metrics[q.front()][2] = currTime;
                currProc = q.front();
                procStarted = currTime;
                metrics[q.front()][1] = currTime;
                q.pop();

                if(!q.empty()){
                    metrics[q.front()][1] = currTime;
                }
            }
        }   
        currTime++;
    }
    
    repp(i,n)cerr<<metrics[i][0]<<" "<<metrics[i][1]<<" "<<metrics[i][2]<<" "<<metrics[i][3]<<endl;

}

void non_preemptive_shortest_job_first(int n, int arr[][3]){
    float att = 0.0, awt = 0.0, art = 0.0;
}

void preemptive_shortest_job_first(int n, int arr[][3]){
    float att = 0.0, awt = 0.0, art = 0.0;
}

void round_robin(int n, int arr[][3], int delta){
    float att = 0.0, awt = 0.0, art = 0.0;
}

void priority_based(int n, int arr[][3]){
    float att = 0.0, awt = 0.0, art = 0.0;
}

int main(){
    int n;    
    cin >> n;

    // int arr[n][3];

    repp(i, n){
        cin >> arr[i][0]>>arr[i][1]>>arr[i][2];
    }

    first_come_first_serve(n);

    // non_preemptive_shortest_job_first(n, arr);

    // preemptive_shortest_job_first(n, arr);

    // round_robin(n, arr, 2);

    // priority_based(n, arr);

    return 0;
}