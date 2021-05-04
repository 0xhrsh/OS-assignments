#include <bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

#define mem(v,i) memset(v,i,sizeof(v)) 

const int INITIAL_VALUE = 1;

#define nl cout<<endl
#define el cerr<<endl

#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define repp(i,n) for(int i=(0);i<(n);i++)

#define k 2
#define m 20
#define f 18
#define s 15
#define pageRange m // should be same as m

int pgFlts = 0;

int allocated_memory[k];
int mi_s[k];
int t_mi = 0;
int pageTable[k][m][2]; // [process id, page num] -> valid, frame

vector<vector<int>> tlb; // process id, pg, frame
vector<int> freeFrames; // list of frames that are free
vector<vector<int>> occupiedFrames; // process id, pg, frame

sem_t getPg, semPrint, sempgFlts;

int frameProportions[k];
int allocatedFrames[k];

void initFrames(){
    mem(frameProportions, 0);
    mem(allocatedFrames, 0);

    repp(i, f) freeFrames.push_back(i);


    int t_frames = 0;
    repp(i, k){
        int frames_allocated = (mi_s[i]*f)/t_mi;
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
    repp(i, k){
        int mi = 1 + rand()%m;
        mi_s[i] = mi;
        t_mi += mi;
    }
}

void updateFrameLRU(int frame_id){
    repp(lruF, occupiedFrames.size()){
        if(occupiedFrames[lruF][2] == frame_id){
            int proc_id = occupiedFrames[lruF][0];
            int pg = occupiedFrames[lruF][1];
            occupiedFrames.erase(occupiedFrames.begin() + lruF);
            occupiedFrames.push_back({proc_id, pg, frame_id});
            break;
        }
    }
}

void* run_process(void* ptr){
    srand(420);
    int id = *(int *)(&ptr);
    
    
    cerr<<"Process: P"<<id<<" started!\n";
    
    
    int req_len = (rand()%(8*mi_s[id])) + 2*mi_s[id];    

    repp(i, req_len){
        ostringstream oss;
        int pg = rand()%pageRange;
        

        oss<<"P"<<id<<": page "<<pg;
        sem_wait(&getPg);
        bool foundInTLB = false;
        repp(j, s){
            
            if(tlb[j][0] == id && tlb[j][1] == pg){
                foundInTLB = true;
                oss<<", TLB hit with frame no. "<<tlb[j][2]<<endl;

                updateFrameLRU(tlb[j][2]);

                vector<int> row = tlb[j];
                tlb.erase(tlb.begin() + j); // update TLB LRU 
                tlb.push_back(row);                

                break;
            }
        }

        if(!foundInTLB){
            if(pageTable[id][pg][0]==1){
                int allocFrame = pageTable[id][pg][1];
                oss<<", TLB miss → page table valid → with frame no. "<<allocFrame<<endl;

                updateFrameLRU(allocFrame);

                tlb.erase(tlb.begin()); // Get LRU of TBL
                tlb.push_back({id, pg, allocFrame}); 
                

            } else {
                sem_wait(&sempgFlts);
                pgFlts++;
                sem_post(&sempgFlts);

                if(freeFrames.size()>0 && allocatedFrames[id]<frameProportions[id]){ // allocating frames in proportion                    

                    int allocFrame = freeFrames[0];
                    oss<<", TLB miss → page fault → free frame: "<<allocFrame<<" allocated to it."<<endl;

                    freeFrames.erase(freeFrames.begin());

                    pageTable[id][pg][0] = 1;
                    pageTable[id][pg][1] = allocFrame;

                    occupiedFrames.push_back({id, pg, allocFrame});
                    allocatedFrames[id]++;
                    
                    tlb.erase(tlb.begin());
                    tlb.push_back({id, pg, allocFrame});


                } else {
                    int lruFrameForProcess = -1; // we only find the LRU frame for a process, so that frame proportion is maintained.
                    repp(lruF, occupiedFrames.size()){
                        if(occupiedFrames[lruF][0] == id){
                            lruFrameForProcess = lruF;
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
                    oss<<", TLB miss → page fault → frame: "<<allocFrame<<" re-allocated to it. (previously alloc to pg: "<<old_pg<<')'<<endl;

                    
                    occupiedFrames.erase(occupiedFrames.begin() + lruFrameForProcess);

                    // allocFrame = pageTable[old_process][old_pg][1];
                    pageTable[old_process][old_pg][0] = 0;

                    pageTable[id][pg][0] = 1;
                    pageTable[id][pg][1] = allocFrame;
                    
                    occupiedFrames.push_back({id, pg, allocFrame});
                    
                    bool present = false;;

                    repp(iTlb, s){
                        if(tlb[iTlb][2] == allocFrame){
                            tlb[iTlb][0] = id;
                            tlb[iTlb][1] = pg;
                            tlb[iTlb][2] = allocFrame;
                            present = true;
                        }
                    }

                    if(!present){
                        tlb.erase(tlb.begin());
                        tlb.push_back({id, pg, allocFrame});
                    }
                }
            }
        }

        sem_post(&getPg);
        sem_wait(&semPrint);
        cout<<oss.str();
        sem_post(&semPrint);
    }
}


int main(){
    srand(69);
    int num_exec = 0;

    sem_init(&getPg, 0, INITIAL_VALUE);
    sem_init(&semPrint, 0, INITIAL_VALUE);
    sem_init(&sempgFlts, 0, INITIAL_VALUE);

    initMis();
    initTLB();
    initFrames();

    bool exec[k];
    mem(exec, 0);
    mem(pageTable, 0);

    
    pthread_t proc[k];

    while(num_exec < k){
        int proc_id = rand()%k;
        if(!exec[proc_id]){
            proc[proc_id] = pthread_t();
            pthread_create(&proc[proc_id], NULL, run_process,(void*) proc_id);
            num_exec++;
            exec[proc_id] = true;
        }
    }

    repp(i, k) pthread_join(proc[i], NULL);

    cout<<"\nAll processes are completed!\nNumber of page faults: "<<pgFlts<<endl;
    
    return 0;
}