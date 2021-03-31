#include <bits/stdc++.h>
using namespace std;

#define repp(i,a) for(int i=0;i<(a);i++)
#define mem(v,i) memset(v,i,sizeof(v))

#define N_MAX 1000
#define delta 2

int currTime;
int t_delta;
int arr[N_MAX][3];
int processingTime[N_MAX];

bool cpu(int task){
    // printf("Current Task in cpu is: %d at time %d\n", task, currTime -1 );
    processingTime[task]--;
    if(processingTime[task] == 0)
        return true;
    
    return false;
}


void driver(int n, queue<int> q, queue<int> (*arrange)(queue<int>, int)){
    int tt[n], wt[n], rt[n];
    mem(tt,0); mem(wt,0); mem(rt,0);

    float att = 0.0, awt = 0.0, art = 0.0;

    int ienter = 0;

    int metrics[n][4]; // enter the queue, first got cpu, time processed, end
    mem(metrics,0);
    repp(i,n){
        metrics[i][1] = -1;
        processingTime[i] = arr[i][1];
    }

    currTime = 0;
    int currProcs = -1;
    
    while(currTime<2500){

        while(arr[ienter][0] == currTime){
            metrics[ienter][0] = currTime;
            q.push(ienter);
            ienter++;
        }

        currTime++;

        if(!q.empty()){
            
            q = (*arrange)(q, currProcs);
            currProcs = q.front();


            bool isFinished = cpu(q.front());
            metrics[q.front()][2]++;

            if(metrics[q.front()][1] == -1){
                metrics[q.front()][1] = currTime -1;
            }

                
            
            if(isFinished){
                metrics[q.front()][3] = currTime;
                q.pop();
                currProcs = -1;
            }
        } else{
            currProcs = -1;
        }
    }
    
    repp(i,n){
        // cerr<<metrics[i][0]<<" "<<metrics[i][1]<<" "<<metrics[i][2]<<" "<<metrics[i][3]<<endl;

        tt[i] = metrics[i][3] - metrics[i][0];
        wt[i] = metrics[i][3] - metrics[i][0] - metrics[i][2];
        rt[i] = metrics[i][1] - metrics[i][0];

        att += tt[i];
        awt += wt[i];
        art += rt[i];

    }

    att = att/float(n);
    awt = awt/float(n);
    art = art/float(n);

    cout<<att<<"\t"<<awt<<"\t"<<art<<endl;
    // repp(i,n){
    //     cout<<endl<<tt[i]<<" "<<wt[i]<<" "<<rt[i];
    // }

}

queue<int> arrange_first_come_first_serve(queue<int> q, int curr){
    return q;
}

queue<int> arrange_non_preemptive_shortest_job_first(queue<int> q, int curr){
    queue<int> nq;
    int mini = -1, minCpuBurst = 10000;

    while(!q.empty()){
        if(processingTime[q.front()]<minCpuBurst){
            mini = q.front();
            minCpuBurst = processingTime[q.front()];
        }
        nq.push(q.front());
        q.pop();
    }

    q.push(mini);

    while(!nq.empty()){
        if(nq.front()!=mini){
            q.push(nq.front());
        }
        nq.pop();
    }
    return q;
}

queue<int> arrange_preemptive_shortest_job_first(queue<int> q, int curr){
    if(curr != -1) return q;

    return arrange_non_preemptive_shortest_job_first(q, curr);
}

queue<int> arrange_round_robin(queue<int> q, int curr){
    // cerr<<"Round robin: front, curr"<<q.front()<<" "<<curr<<endl;
    if(curr == -1){
        return q;
        t_delta--;
    }
    else if(q.front() == curr && t_delta > 1){
        t_delta--;
        return q;
    }
    t_delta = delta;

    int frt = q.front();
    q.pop();
    q.push(frt);

    return q;
}

queue<int> arrange_priority_based(queue<int> q, int curr){
    queue<int> nq;
    int mini, minPriority = 10000;

    while(!q.empty()){
        if(arr[q.front()][2]<minPriority){
            mini = q.front();
            minPriority = arr[q.front()][2];
        }
        nq.push(q.front());
        q.pop();
    }

    q.push(mini);

    while(!nq.empty()){
        if(nq.front()!=mini){
            q.push(nq.front());
        }
        nq.pop();
    }
    return q;
}

int main(){
    int n;    
    cin >> n;

    // int arr[n][3];

    repp(i, n){
        cin >> arr[i][0]>>arr[i][1]>>arr[i][2];
    }

    queue<int> q;
    cout<<"Algorithm\tATT\tAWT\tART\n";
    cout<<"=======================================\n";
    cout<<"FCFS\t\t";
    driver(n, q, arrange_first_come_first_serve);
    cout<<"NPSJF\t\t";
    driver(n, q, arrange_non_preemptive_shortest_job_first);
    cout<<"PSJF\t\t";
    driver(n, q, arrange_preemptive_shortest_job_first);
    cout<<"RR\t\t";
    t_delta = delta;
    driver(n, q, arrange_round_robin);
    cout<<"Priority\t";
    driver(n, q, arrange_priority_based);

    return 0;
}