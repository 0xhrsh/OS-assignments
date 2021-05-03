#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) 

#define nl cout<<endl
#define el cerr<<endl

#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

#define k 10
#define m 10
#define f 10
#define s 10
#define pageRange 10 // should be same as m

int allocated_memory[k];

int pageTable[k][m][2]; // [process, page] valid, frame

vector<vector<int>> tlb; // process id, pg, frame

vector<int> freeFrames;
vector<vector<int>> occupiedFrames; // frame id, process id, pg

void run_process(int id){
    int req_len = (rand()%(8*m)) + 2*m;

    int mi = 1 + rand()%m;

    repp(i, req_len){
        int pg = rand()%pageRange;

        bool foundInTLB = false;
        repp(j, s){
            if(tlb[j][0] == id && tlb[j][1] == pg){
                foundInTLB = true;
                cout<<"Process P"<<id<<": for page reference "<<pg<<", TLB hit with frame no. "<<tlb[j][0]<<endl;

                vector<int> row = tlb[j];
                tlb.erase(tlb.begin() + j);
                tlb.push_back(row);

                break;
            }
        }

        if(!foundInTLB){
            if(pageTable[id][pg][0]==1){
                int allocFrame = pageTable[id][pg][1];
                cout<<"Process P"<<id<<": for page reference "<<pg<<", TLB miss → page table valid→ with frame no. "<<pageTable[id][pg][1];

                tlb.erase(tlb.begin());
                tlb.push_back({id, pg, allocFrame});

            } else {
                if(freeFrames.size()>0){
                    int allocFrame = freeFrames[0];
                    freeFrames.erase(freeFrames.begin());

                    pageTable[id][pg][0] = 1;
                    pageTable[id][pg][1] = allocFrame;

                    occupiedFrames.push_back({id, pg, allocFrame});
                    
                    tlb.erase(tlb.begin());
                    tlb.push_back({id, pg, allocFrame});


                } else {
                    int old_process = occupiedFrames[0][0];
                    int old_pg = occupiedFrames[0][1];
                    int allocFrame = occupiedFrames[0][2];

                    occupiedFrames.erase(occupiedFrames.begin());

                    // int allocFrame = pageTable[old_process][old_pg][1];
                    pageTable[old_process][old_pg][0] = 0;

                    pageTable[id][pg][0] = 1;
                    pageTable[id][pg][1] = allocFrame;
                    
                    occupiedFrames.push_back({id, pg, allocFrame});
                    
                    tlb.erase(tlb.begin());
                    tlb.push_back({id, pg, allocFrame});
                }
            }
        }
    }




}


int main(){

    int num_exec = 0;

    while(num_exec < k){
        run_process(rand()%k);
        num_exec++;
    }
    
    return 0;
}