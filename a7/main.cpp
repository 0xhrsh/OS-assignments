#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) 

#define nl cout<<endl
#define el cerr<<endl

#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

#define k 2
#define m 10
#define f 5
#define s 2
#define pageRange 10 // should be same as m

int pgFlts = 0;

int allocated_memory[k];

int pageTable[k][m][2]; // [process id, page num] -> valid, frame

vector<vector<int>> tlb; // process id, pg, frame

vector<int> freeFrames; // list of frames that are free
vector<vector<int>> occupiedFrames; // frame id, process id, pg

void initTLB(){
    repp(i, s){
        // tlb.push_back({rand()%k, rand()%pageRange, rand()%f});
        tlb.push_back({-1, -1, -1});
        // cerr<<tlb[i][0]<<" "<<tlb[i][1]<<" "<<tlb[i][2]<<endl;
    }
}

void initFrames(){
    repp(i, f) freeFrames.push_back(i);
}

void run_process(int id){
    cout<<"Process: P"<<id<<" started!\n";
    
    int mi = 1 + rand()%m;
    int req_len = (rand()%(8*mi)) + 2*mi;    

    repp(i, req_len){
        int pg = rand()%pageRange;
        

        cout<<"P"<<id<<": page "<<pg;
        
        bool foundInTLB = false;
        repp(j, s){
            if(tlb[j][0] == id && tlb[j][1] == pg){
                foundInTLB = true;
                cout<<", TLB hit with frame no. "<<tlb[j][0]<<endl;

                vector<int> row = tlb[j];
                tlb.erase(tlb.begin() + j);
                tlb.push_back(row);

                break;
            }
        }

        if(!foundInTLB){
            if(pageTable[id][pg][0]==1){
                int allocFrame = pageTable[id][pg][1];
                cout<<", TLB miss → page table valid → with frame no. "<<allocFrame<<endl;

                tlb.erase(tlb.begin());
                tlb.push_back({id, pg, allocFrame});

            } else {
                if(freeFrames.size()>0){
                    pgFlts++;

                    int allocFrame = freeFrames[0];
                    cout<<", TLB miss → page fault → free frame: "<<allocFrame<<" allocated to it."<<endl;

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
                    cout<<", TLB miss → page fault → frame: "<<allocFrame<<" re-allocated to it. (previously alloc to pg: "<<old_pg<<')'<<endl;

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

    initTLB();
    initFrames();

    while(num_exec < k){
        run_process(num_exec);
        num_exec++;
    }

    cout<<"All processes are completed!\nNumber of page faults: "<<pgFlts<<endl;
    
    return 0;
}