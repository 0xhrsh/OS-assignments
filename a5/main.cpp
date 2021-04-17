#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) //Use mem(v,60) to initialize with 10^9

#define nl cout<<endl
#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)
#define rep(i,a,n) for(int i=(a);i<(n);i++)

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int b=-1, c=-1, w=-1;

    rep(i, 1, argc){
        if(argv[i][0]!='-')continue;
        switch (argv[i][1]){
        case 'b':
            b = atoi(argv[i]+3);
            break;
        case 'c':
            c = atoi(argv[i]+3);
            break;
        case 'w':
            w = atoi(argv[i]+3);
            break;
        default:
            cerr<<"Incorrect flag: "<<argv[i];
            break;
        }
    }

    cerr<<b<<" "<<c<<" "<<w;el;

    
    
    return 0;
}