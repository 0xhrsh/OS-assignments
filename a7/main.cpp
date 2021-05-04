#include <bits/stdc++.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) 

#define nl cout<<endl
#define el cerr<<endl

#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

#define k 2
#define m 10
#define f 6
#define s 5
#define pageRange 10 // should be same as m

int pgFlts = 0;

int allocated_memory[k];
int mi_s[k];
int t_mi = 0;

int pageTable[k][m][2]; // [process id, page num] -> valid, frame

vector<vector<int>> tlb; // process id, pg, frame

vector<int> freeFrames; // list of frames that are free
vector<vector<int>> occupiedFrames; // frame id, process id, pg

int frameProportions[k];
int allocatedFrames[k];

void initFrames(){
    mem(frameProportions, 0);
    mem(allocatedFrames, 0);

    repp(i, f) freeFrames.push_back(i);


    int t_frames = 0;
    repp(i, k){
        int frames_allocated = (mi_s[i]*s)/t_mi;
        repp(j, frames_allocated){
            frameProportions[i]++;
            t_frames++;
        }
            
        
        if(frames_allocated == 0){ // if 0 frames are there in proportion
            frameProportions[i] = 1; // we give the process 1 frame   
            t_frames++;
        }
    }

    repp(i, k)cerr<<mi_s[i]<<" "<<frameProportions[i]<<" "<<t_mi<<endl;

    if(t_frames>f){
        cerr<<"Unsufficient frames to allow non-zero frames to everyone proportionally.\n";
        cerr<<"Either Run again or Increase f or decrease k\n";
        exit(1);
    }
}

void initTLB(){
    repp(i, s){
        tlb.push_back({-1, -1, -1});
    }
}


void initMis(){
    srand(0);

    repp(i, k){
        int mi = 1 + rand()%m;
        mi_s[i] = mi;
        t_mi += mi;
    }
}

void run_process(int id){
    srand(0);

    cout<<"Process: P"<<id<<" started!\n";
    
    int req_len = (rand()%(8*mi_s[id])) + 2*mi_s[id];    

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
                pgFlts++;
                if(freeFrames.size()>0 && allocatedFrames[id]<frameProportions[id]){ // allocating frames in proportion                    

                    int allocFrame = freeFrames[0];
                    cout<<", TLB miss → page fault → free frame: "<<allocFrame<<" allocated to it."<<endl;

                    freeFrames.erase(freeFrames.begin());

                    pageTable[id][pg][0] = 1;
                    pageTable[id][pg][1] = allocFrame;

                    occupiedFrames.push_back({id, pg, allocFrame});
                    allocatedFrames[id]++;
                    
                    tlb.erase(tlb.begin());
                    tlb.push_back({id, pg, allocFrame});


                } else {
                    int lruFrameForProcess = -1; // we only find the LRU frame for a process, so that frame proportion is maintained.
                    repp(i, occupiedFrames.size()){
                        if(occupiedFrames[i][0] == id){
                            lruFrameForProcess = i;
                            break;
                        }
                    }

                    if(lruFrameForProcess == -1){
                        cerr<<"Couldn't find any existing frame for the given process\n";
                        exit(1);
                    }

                    int old_process = occupiedFrames[lruFrameForProcess][0];
                    int old_pg = occupiedFrames[lruFrameForProcess][1];
                    int allocFrame = occupiedFrames[lruFrameForProcess][2];
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

    initMis();
    initTLB();
    initFrames();

    bool exec[k];
    mem(exec, 0);

    srand(0);

    while(num_exec < k){
        int proc_id = rand()%k;
        if(!exec[proc_id]){
            run_process(num_exec);
            num_exec++;
            exec[proc_id] = true;
        }
    }

    cout<<"\nAll processes are completed!\nNumber of page faults: "<<pgFlts<<endl;
    
    return 0;
}