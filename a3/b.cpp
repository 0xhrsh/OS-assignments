#include <bits/stdc++.h>
using namespace std;

#define repp(i,a) for(int i=0;i<(a);i++)

#define N_MAX 1000

void first_come_first_serve(int arr[][3]){

}

void non_preemptive_shortest_job_first(int arr[][3]){
    
}

void preemptive_shortest_job_first(int arr[][3]){
    
}

void round_robin(int arr[][3], int delta){
    
}

void priority_based(int arr[][3]){
    
}

int main(){
    int n;
    cin >> n;

    int arr[n][3];

    repp(i, n){
        cin >> arr[i][0]>>arr[i][1]>>arr[i][2];
    }

    first_come_first_serve(arr);

    non_preemptive_shortest_job_first(arr);

    preemptive_shortest_job_first(arr);

    round_robin(arr, 2);

    priority_based(arr);

    return 0;
}