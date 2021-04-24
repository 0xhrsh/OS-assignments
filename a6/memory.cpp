#include <bits/stdc++.h>

using namespace std;

#define mem(v,i) memset(v,i,sizeof(v))

#define nl cout<<endl
#define el cerr<<endl

#define repp(i,n) for(int i=(0);i<(n);i++)

int pgFaultsFIFO(vector<int> pages, int capacity){
    int n = pages.size();

    int pgFault  = 0;
    queue<int> fifo;
    unordered_set<int> present;

    repp(i, n){
        int page = pages[i];
        if(present.find(page) != present.end()) continue;

        if(fifo.size() >= capacity){
            int rem = fifo.front();
            fifo.pop();
            present.erase(rem);
            // cerr<<"Rem=>"<<rem<<endl;
        }

        fifo.push(page);
        present.insert(page);

        pgFault++;
        // cerr<<page<<endl;
    }
    cerr<<endl;
    return pgFault;
}

int main(){
    int n;
    cout<<"Enter n: ";
    cin>>n;
    repp(i, n){
        int n_frames;
        cout<<"Enter the number of frames: ";
        cin>>n_frames;

        vector<int> rString;

        cout<<"Enter the pages(positive integers) space seperated: ";

        while (true) {
            int temp;
            cin >> temp;
            if(temp == -1) break;

            rString.push_back(temp);
            if (cin.peek() == '\n') {
                break;
            }
        }

        cout<<"Page Faults in FIFO: "<<pgFaultsFIFO(rString, n_frames)<<endl;

    }
    
    return 0;
}