#include <bits/stdc++.h>
using namespace std;

#define repp(i,a) for(int i=0;i<(a);i++)
#define mem(v,i) memset(v,i,sizeof(v))

#define N_MAX 1000

void first_come_first_serve(int n, int arr[][3]){
    int tt[n], wt[n], rt[n];
    int cpu[n][4]; // enter the queue, ready, start, end
    mem(tt,0); mem(wt,0); mem(rt,0);

    queue<int> q;

    int time = 0;
    int computing = -1;
    int ienter = 0, end = 0;
    while(time<1000){

        while(arr[ienter][0] == time){
            cpu[ienter][0] = time;
            q.push(ienter);
            ienter++;
        }

        if(computing == -1){
            if(q.empty()){
                if(time>10000)break;
                time++;
                continue;
            }
            computing = q.front();
            cpu[q.front()][2] = time;
            cpu[q.front()][1] = time;
            q.pop();

            if(!q.empty())
                cpu[q.front()][1] = time;
        }

        if(arr[computing][1] + end == time){
            cerr<<time<<" "<<computing<<" "<<q.front()<<endl;
            cpu[computing][3] = time;
            end = time;

            if(q.empty()){
                computing = -1; 
            } else {
                computing = q.front();
                cpu[computing][2] = time;
                q.pop();

                cerr<<time<<" "<<computing<<" "<<q.front()<<endl;

                if(!q.empty())
                    cpu[q.front()][1] = time;
            }
        }
        time++;
    }

    repp(i,n)cerr<<cpu[i][0]<<" "<<cpu[i][1]<<" "<<cpu[i][2]<<" "<<cpu[i][3]<<endl;
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

    int arr[n][3];

    repp(i, n){
        cin >> arr[i][0]>>arr[i][1]>>arr[i][2];
    }

    first_come_first_serve(n, arr);

    // non_preemptive_shortest_job_first(n, arr);

    // preemptive_shortest_job_first(n, arr);

    // round_robin(n, arr, 2);

    // priority_based(n, arr);

    return 0;
}