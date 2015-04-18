#include <bits/stdc++.h>
using namespace std;

//Global variable
int frameSize = 1;
int mainMemory = 640;
int arrSize = mainMemory/frameSize;

struct pageTable
{
    vector<int> pageNumber;
    vector<int> frameNumber;
    vector<bool> validBit;
    vector<bool> referenceBit;
    vector<bool> modifyBit;
    vector<int> historyBits;
};


int main()
{
    vector< map<int, int> > processLimit;
    vector<int> temp;
    ifstream myReadFile;
    myReadFile.open("progAddrLimits.txt");
    int output;
    if (myReadFile.is_open()) 
    {
        while (!myReadFile.eof()) 
        {
            myReadFile >> output ;
            temp.push_back(output);
        }
    }
    myReadFile.close();
    int totalSpace=0;
    for(int i=0;i<(temp.size()-1);i+=2)
    {
        map<int, int> t;
        t[temp[i]] = temp[i+1];
        totalSpace+=temp[i+1];
        processLimit.push_back(t);
    }

    int processCount = processLimit.size();

    struct pageTable pt[processCount];

    for(int i=0;i<processCount;i++)
    {
        int t = processLimit[i][i];
        t = t/frameSize;
        for(int j=0;j<=t;j++)
        {
            
            pt[i].pageNumber.push_back(j);
            pt[i].frameNumber.push_back(-1);
            pt[i].validBit.push_back(0);
            pt[i].referenceBit.push_back(0);
            pt[i].modifyBit.push_back(0);
            pt[i].historyBits.push_back(0);
        }
    }

    int a,b;
    int allowedFrameCount[processLimit.size()];

    int t1=0;
    for(int i=0;i<(processLimit.size()-1);i++)
    {
        allowedFrameCount[i] = max(2,(arrSize*processLimit[i][i])/totalSpace);
        //cout<<allowedFrameCount[i]<<endl;
        //allowedFrameCount[i] = (arrSize/10);
        t1+=allowedFrameCount[i];
    }

    allowedFrameCount[processLimit.size()-1] = arrSize - t1;

    int frames[arrSize];
    int processValid[processCount];

    int pageFault[processCount];

    for(int i=0;i<processCount;i++)
        pageFault[i] = 0;

    for(int i=0;i<processCount;i++)
        processValid[i] = 0;

    queue<int> processQueue[processCount];
    //queue<pair<int, int> > processQueue;
    int queueSize = arrSize;
    //cout<<queueSize<<endl;
    int changeHistoryBits = 0;
    int flushClock = 0;

    myReadFile.open("progMemoryref.txt");
    if (myReadFile.is_open()) 
    {
        while (!myReadFile.eof()) 
        {
            myReadFile >> a >> b;
            
            
            //Local FIFO
            
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(allowedFrameCount[a]>0)
                        {
                            pt[a].validBit[b/frameSize] = 1;
                            processQueue[a].push(b/frameSize);
                            allowedFrameCount[a]--;   
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            pt[a].validBit[processQueue[a].front()] = 0;
                            processQueue[a].pop();
                            processQueue[a].push(b/frameSize);
                            pt[a].validBit[b/frameSize] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                } 
            }
            
            /*
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                {
                        int t = processLimit[a][a];
                        t = t/frameSize;
                        if(allowedFrameCount[a]>0)
                        {
                            
                            pt[a].validBit[b/frameSize] = 1;
                            processQueue[a].push(b/frameSize);
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            processQueue[a].push((b/frameSize+1)%(t+1));
                            allowedFrameCount[a]-=2;   
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;

                            pt[a].validBit[processQueue[a].front()] = 0;
                            processQueue[a].pop();
                            pt[a].validBit[processQueue[a].front()] = 0;
                            processQueue[a].pop();
                            processQueue[a].push(b/frameSize);
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            processQueue[a].push((b/frameSize+1)%(t+1));
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                } 
            }
            */
            /*
            // Global FIFO
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                     if(pt[a].validBit[b/frameSize] == 0)
                     {
                         if(queueSize>0)
                         {
                             pt[a].validBit[b/frameSize] = 1;
                             processQueue.push(make_pair(a,b/frameSize));
                             queueSize--;   
                         }
                         else
                         {
                             //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                             pt[processQueue.front().first].validBit[processQueue.front().second] = 0;
                             processQueue.pop();
                             processQueue.push(make_pair(a,b/frameSize));
                             pt[a].validBit[b/frameSize] = 1;
                         }
                         pageFault[a]++;
                     }
                     else
                     {
                         pt[a].referenceBit[b/frameSize] = 1;
                     }
                }
                else
                {
                     cout << a <<" "<<b<<endl;
                     processValid[a] = 1;
                }
            } 
            */
            /*
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    int t = processLimit[a][a];
                    t = t/frameSize;
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(queueSize>0)
                        {
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            processQueue.push(make_pair(a,b/frameSize));
                            processQueue.push(make_pair(a,(b/frameSize+1)%(t+1)));
                            queueSize-=2;   
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            pt[processQueue.front().first].validBit[processQueue.front().second] = 0;
                            processQueue.pop();
                            pt[processQueue.front().first].validBit[processQueue.front().second] = 0;
                            processQueue.pop();
                            processQueue.push(make_pair(a,b/frameSize));
                            processQueue.push(make_pair(a,(b/frameSize+1)%(t+1)));
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                     //cout << a <<" "<<b<<endl;
                     processValid[a] = 1;
                }
            }
            */
           
            //Local LRU
            /*
            if(processValid[a] == 0)
            {

                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(allowedFrameCount[a]>0)
                        {
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                            allowedFrameCount[a]--;
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            int mini = 0;
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            for(int j=0;j<=t;j++)
                            {
                                if(pt[a].validBit[j]==1)
                                {
                                    mini = j;
                                    break;
                                }
                            }
                            for(int j=0;j<=t;j++)
                            {
                                if(pt[a].validBit[j]==1 && pt[a].historyBits[j]<pt[a].historyBits[mini])
                                {
                                    mini = j;
                                }
                            }
                            pt[a].validBit[mini] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                        {
                            pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                        }
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
                if(changeHistoryBits<128)
                {
                    changeHistoryBits++;
                } 
                else
                {

                    for(int i=0;i<processCount;i++)
                    {
                        int t = processLimit[i][i];
                        t = t/frameSize;
                        for(int j=0;j<=t;j++)
                        {
                            if(pt[i].historyBits[j]>0)
                                pt[i].historyBits[j]/=2;
                        }
                    }
                    changeHistoryBits = 0;
                }
            }
            */
            /*
            // Local LRU PrePaging
            if(processValid[a] == 0)
            {

                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(allowedFrameCount[a]>0)
                        {
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                            if(pt[a].historyBits[(b/frameSize+1)%(t+1)]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[(b/frameSize+1)%(t+1)] += (INT_MAX/2) + 1;
                            }
                            allowedFrameCount[a]-=2;
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            int mini = 0;
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            for(int j=0;j<=t;j++)
                            {
                                if(pt[a].validBit[j]==1)
                                {
                                    mini = j;
                                    break;
                                }
                            }
                            for(int j=0;j<=t;j++)
                            {
                                if(pt[a].validBit[j]==1 && pt[a].historyBits[j]<pt[a].historyBits[mini])
                                {
                                    mini = j;
                                }
                            }
                            pt[a].validBit[mini] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                            int hh = mini+1;
                            int new_mini;
                            while(hh!=mini)
                            {
                                if(pt[a].validBit[hh]==1)
                                {
                                    new_mini = hh;
                                    break;
                                } 
                                hh = (hh+1)%(t+1);
                            }
                            
                            while(hh!=mini)
                            {
                                if(pt[a].validBit[hh]==1 && pt[a].historyBits[new_mini]>pt[a].historyBits[hh])
                                {
                                    new_mini = hh;
                                }
                                hh = (hh+1)%(t+1); 
                            }
                            pt[a].validBit[new_mini] = 0;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            if(pt[a].historyBits[(b/frameSize+1)%(t+1)]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[(b/frameSize+1)%(t+1)] += (INT_MAX/2) + 1;
                            }
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                        {
                            pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                        }
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
                if(changeHistoryBits<32)
                {
                    changeHistoryBits++;
                } 
                else
                {

                    for(int i=0;i<processCount;i++)
                    {
                        int t = processLimit[i][i];
                        t = t/frameSize;
                        for(int j=0;j<=t;j++)
                        {
                            if(pt[i].historyBits[j]>0)
                                pt[i].historyBits[j]/=2;
                        }
                    }
                    changeHistoryBits = 0;
                }
            }
            */
            /*
            //Global LRU
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(queueSize>0)
                        {
                            
                            queueSize--;
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            int miniProc = 0, minFrame=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                bool temp = false;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        miniProc = i;
                                        minFrame = j;
                                        temp = true;
                                        break;
                                    }
                                }
                                if(temp)
                                {
                                    break;
                                }
                            }
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[miniProc].historyBits[minFrame]> pt[i].historyBits[j])
                                        {
                                            miniProc = i;
                                            minFrame = j;
                                        }
                                    }
                                }
                            }
                            pt[miniProc].validBit[minFrame] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                        {
                            pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                        }
                    }
                }
                else
                {
                    cout << a <<" "<<b<<endl;
                    processValid[a] = 1;
                }
                if(changeHistoryBits<128)
                {
                    changeHistoryBits++;
                } 
                else
                {

                    for(int i=0;i<processCount;i++)
                    {
                        int t = processLimit[i][i];
                        t = t/frameSize;
                        for(int j=0;j<=t;j++)
                        {
                            if(pt[i].historyBits[j]>0)
                                pt[i].historyBits[j]/=2;
                        }
                    }
                    changeHistoryBits = 0;
                } 
            }
            */
            /*
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(queueSize>0)
                        {
                            
                            queueSize-=2;
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                            if(pt[a].historyBits[(b/frameSize+1)%(t+1)]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[(b/frameSize+1)%(t+1)] += (INT_MAX/2) + 1;
                            }
                        }
                        else
                        {
                            //cout<<"Page Fault By"<<" "<<a<<" "<<b<<endl;
                            int miniProc = 0, minFrame=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                bool temp = false;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        miniProc = i;
                                        minFrame = j;
                                        temp = true;
                                        break;
                                    }
                                }
                                if(temp)
                                {
                                    break;
                                }
                            }
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[miniProc].historyBits[minFrame]> pt[i].historyBits[j])
                                        {
                                            miniProc = i;
                                            minFrame = j;
                                        }
                                    }
                                }
                            }
                            pt[miniProc].validBit[minFrame] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                            }
                            miniProc = 0; minFrame=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                bool temp = false;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        miniProc = i;
                                        minFrame = j;
                                        temp = true;
                                        break;
                                    }
                                }
                                if(temp)
                                {
                                    break;
                                }
                            }
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[miniProc].historyBits[minFrame]> pt[i].historyBits[j])
                                        {
                                            miniProc = i;
                                            minFrame = j;
                                        }
                                    }
                                }
                            }
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[miniProc].validBit[minFrame] = 0;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            if(pt[a].historyBits[(b/frameSize+1)%(t+1)]<=((INT_MAX/2)))
                            {
                                pt[a].historyBits[(b/frameSize+1)%(t+1)] += (INT_MAX/2) + 1;
                            }
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        if(pt[a].historyBits[b/frameSize]<=((INT_MAX/2)))
                        {
                            pt[a].historyBits[b/frameSize] += (INT_MAX/2) + 1;
                        }
                    }
                }
                else
                {
                    cout << a <<" "<<b<<endl;
                    processValid[a] = 1;
                }
                if(changeHistoryBits<128)
                {
                    changeHistoryBits++;
                } 
                else
                {

                    for(int i=0;i<processCount;i++)
                    {
                        int t = processLimit[i][i];
                        t = t/frameSize;
                        for(int j=0;j<=t;j++)
                        {
                            if(pt[i].historyBits[j]>0)
                                pt[i].historyBits[j]/=2;
                        }
                    }
                    changeHistoryBits = 0;
                } 
            }
            */
            /*
            //Local Clock
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(allowedFrameCount[a]>0)
                        {
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                            allowedFrameCount[a]--;
                        }
                        else
                        {
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            int replace;
                            int j=0;
                            while(true)
                            {
                                if(pt[a].validBit[j]==1)
                                {
                                    if(pt[a].referenceBit[j]==0)
                                    {
                                        replace = j;
                                        break;
                                    }
                                    else
                                    {
                                        pt[a].referenceBit[j] = 0;
                                    }
                                }
                                j = (j+1)%(t+1);
                            }
                            pt[a].validBit[replace] = 0;
                            pt[a].referenceBit[replace] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
            }
            */
            /*
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(allowedFrameCount[a]>0)
                        {
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            pt[a].referenceBit[(b/frameSize+1)%(t+1)] = 1;
                            allowedFrameCount[a]-=2;
                        }
                        else
                        {
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            int replace;
                            int j=0;
                            while(true)
                            {
                                if(pt[a].validBit[j]==1)
                                {
                                    if(pt[a].referenceBit[j]==0)
                                    {
                                        replace = j;
                                        break;
                                    }
                                    else
                                    {
                                        pt[a].referenceBit[j] = 0;
                                    }
                                }
                                j = (j+1)%(t+1);
                            }
                            pt[a].validBit[replace] = 0;
                            pt[a].referenceBit[replace] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;

                            replace=0;
                            j=0;
                            while(true)
                            {
                                if(pt[a].validBit[j]==1)
                                {
                                    if(pt[a].referenceBit[j]==0)
                                    {
                                        replace = j;
                                        break;
                                    }
                                    else
                                    {
                                        pt[a].referenceBit[j] = 0;
                                    }
                                }
                                j = (j+1)%(t+1);
                            }
                            pt[a].validBit[replace] = 0;
                            pt[a].referenceBit[replace] = 0;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            pt[a].referenceBit[(b/frameSize+1)%(t+1)] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
            }
            */
            /*
            // Global Clock
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(queueSize>0)
                        {
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                            queueSize--;
                        }
                        else
                        {
                            int replacePage=999999;
                            int replaceProc=999999;
                            int j=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[i].referenceBit[j]==1)
                                        {
                                            pt[i].referenceBit[j] = 0;
                                        }
                                        else
                                        {
                                            replacePage = j;
                                            replaceProc = i;
                                            break;
                                        }
                                    }
                                }
                                if(replacePage!=999999)
                                {
                                    break;
                                }
                            }
                            if(replacePage==999999)
                            {
                                for(int i=0;i<processCount;i++)
                                {
                                    int t = processLimit[i][i];
                                    t = t/frameSize;
                                    for(int j=0;j<=t;j++)
                                    {
                                        if(pt[i].validBit[j]==1)
                                        {
                                            if(pt[i].referenceBit[j]==1)
                                            {
                                                pt[i].referenceBit[j] = 0;
                                            }
                                            else
                                            {
                                                replacePage = j;
                                                replaceProc = i;
                                                break;
                                            }
                                        }
                                    }
                                    if(replacePage!=999999)
                                    {
                                        break;
                                    }
                                } 
                            }
                            pt[replaceProc].validBit[replacePage] = 0;
                            pt[replaceProc].referenceBit[replacePage] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
            }
            */
            /*
            if(processValid[a] == 0)
            {
                if(b>=0 && b<=processLimit[a][a])
                {
                    if(pt[a].validBit[b/frameSize] == 0)
                    {
                        if(queueSize>0)
                        {
                            
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            pt[a].referenceBit[(b/frameSize+1)%(t+1)] = 1;
                            queueSize-=2;
                        }
                        else
                        {
                            int replacePage=999999;
                            int replaceProc=999999;
                            int j=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[i].referenceBit[j]==1)
                                        {
                                            pt[i].referenceBit[j] = 0;
                                        }
                                        else
                                        {
                                            replacePage = j;
                                            replaceProc = i;
                                            break;
                                        }
                                    }
                                }
                                if(replacePage!=999999)
                                {
                                    break;
                                }
                            }
                            if(replacePage==999999)
                            {
                                for(int i=0;i<processCount;i++)
                                {
                                    int t = processLimit[i][i];
                                    t = t/frameSize;
                                    for(int j=0;j<=t;j++)
                                    {
                                        if(pt[i].validBit[j]==1)
                                        {
                                            if(pt[i].referenceBit[j]==1)
                                            {
                                                pt[i].referenceBit[j] = 0;
                                            }
                                            else
                                            {
                                                replacePage = j;
                                                replaceProc = i;
                                                break;
                                            }
                                        }
                                    }
                                    if(replacePage!=999999)
                                    {
                                        break;
                                    }
                                } 
                            }
                            pt[replaceProc].validBit[replacePage] = 0;
                            pt[replaceProc].referenceBit[replacePage] = 0;
                            pt[a].validBit[b/frameSize] = 1;
                            pt[a].referenceBit[b/frameSize] = 1;

                            replacePage=999999;
                            replaceProc=999999;
                            j=0;
                            for(int i=0;i<processCount;i++)
                            {
                                int t = processLimit[i][i];
                                t = t/frameSize;
                                for(int j=0;j<=t;j++)
                                {
                                    if(pt[i].validBit[j]==1)
                                    {
                                        if(pt[i].referenceBit[j]==1)
                                        {
                                            pt[i].referenceBit[j] = 0;
                                        }
                                        else
                                        {
                                            replacePage = j;
                                            replaceProc = i;
                                            break;
                                        }
                                    }
                                }
                                if(replacePage!=999999)
                                {
                                    break;
                                }
                            }
                            if(replacePage==999999)
                            {
                                for(int i=0;i<processCount;i++)
                                {
                                    int t = processLimit[i][i];
                                    t = t/frameSize;
                                    for(int j=0;j<=t;j++)
                                    {
                                        if(pt[i].validBit[j]==1)
                                        {
                                            if(pt[i].referenceBit[j]==1)
                                            {
                                                pt[i].referenceBit[j] = 0;
                                            }
                                            else
                                            {
                                                replacePage = j;
                                                replaceProc = i;
                                                break;
                                            }
                                        }
                                    }
                                    if(replacePage!=999999)
                                    {
                                        break;
                                    }
                                } 
                            }
                            pt[replaceProc].validBit[replacePage] = 0;
                            pt[replaceProc].referenceBit[replacePage] = 0;
                            int t = processLimit[a][a];
                            t = t/frameSize;
                            pt[a].validBit[(b/frameSize+1)%(t+1)] = 1;
                            pt[a].referenceBit[(b/frameSize+1)%(t+1)] = 1;
                        }
                        pageFault[a]++;
                    }
                    else
                    {
                        pt[a].referenceBit[b/frameSize] = 1;
                    }
                }
                else
                {
                    processValid[a] = 1;
                }
            }
            */
        }
    }
    
    myReadFile.close();
    int sum = 0;
    for(int i=0;i<processCount;i++)
    {
        cout<<pageFault[i]<<endl;
        sum+=pageFault[i];
    }
    cout<<sum<<endl;
    
    return 0;

}