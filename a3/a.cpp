#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v))
#define nl cout<<endl
#define el cerr<<endl
#define ll long long

#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)
#define repr(i,n) for(int i=(n);i>=0;i--)
#define repl(i,a,b) for(ll i=(a);i<(b);i++)

#define exMean 10
default_random_engine generator;

exponential_distribution<float> exp_distribution(1.0/exMean);
uniform_int_distribution<int> uni_distribution_burst(1, 20);
uniform_int_distribution<int> uni_distribution_priority(1, 10);

int main(int argc, char* argv[]){
    srand ( time(NULL) );
    generator.seed(time(NULL));
    
    if(argc != 2){
        fprintf(stderr, "Enter only 1 integer as cmd line argument\n");
        exit(1);
    }
    int n = stoi(argv[1]);
    // cout<<"Enter the number of processes: ";
    // cin>>n;
    int processes[n][3]; // in this order -> arrival time, cpu burst, priority

    repp(i,n){
        if(i==0){
            processes[0][0] = 0; // arrival time is zero for first process
        } else{
            int num = int(exp_distribution(generator));
            while(num>10){
                num = int(exp_distribution(generator));
            }
            processes[i][0] = processes[i-1][0] + num;
        }

        processes[i][1] = uni_distribution_burst(generator);
        processes[i][2] = uni_distribution_priority(generator);
    }

    ofstream file("in.txt", ios::trunc);

    file<<n<<endl;
    
    repp(i,n){
        file<<processes[i][0]<<" "<<processes[i][1]<<" "<<processes[i][2]<<endl;
    }
    file.close();


    
    return 0;
}