/**
 * Copyright 2010 Zhou Zhao
 *
 * This file is part of FPGA test system for EE658 at USC
 *
 FPGA test system is a free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 FPGA test system is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with the FPGA test system.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "Node.h"
#include "Command.h"
#include "global.h"
#include "type.h"
#include "fileWrite.h"
using namespace std;

void printDisplayBuffer(){

    map<int, vector<GEVENT> >::iterator i;
    int j;
    vector<GEVENT> temp;

    for(i=displayBuffer.begin();i!=displayBuffer.end();i++){
        cout<<"line "<<i->first<<": ";
        temp = i->second;
        for(j=0;j<temp.size();j++){
            cout<<"("<<temp[j].logic<<" @ "<<temp[j].time<<")";
        }
        cout<<endl;
    }
}

void printCompareBuffer(){

    map<int, vector<int> >::iterator i;
    int j;
    vector<int> temp;

    for(i=compareBuffer.begin();i!=compareBuffer.end();i++){
        cout<<"line "<<i->first<<": ";
        temp = i->second;
        for(j=0;j<temp.size();j++){
            cout<<temp[j]<<" ";
        }
        cout<<endl;
    }
}

int printScanNetlist(){

    int i,j;
    char fileName[] = "scan_version_netlist.txt";
    FILE* fileID;
    Node* np;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"scan file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberNodes;i++){
        np=&NodePointer[i];
        if(np->getType()==PI || np->getType()==PPI){
            fprintf(fileID, "%d %d %d %d %d\n", np->getType(), np->getNumber(),
                        np->getGate(), np->getFanout(), np->getFanin());
        }else{
            if(np->getType()==PO || np->getType()==GATE || np->getType()==PPO){
                fprintf(fileID, "%d %d %d %d %d ", np->getType(), np->getNumber(),
                        np->getGate(), np->getFanout(), np->getFanin());
                for(j=0;j<np->getFanin();j++){
                    fprintf(fileID, "%d ", np->getUpperNodes()[j]->getNumber());
                }
                fprintf(fileID, "\n");
            }else{
                if(np->getType()==FB){
                    fprintf(fileID, "%d %d %d %d\n", np->getType(), np->getNumber(),
                        np->getGate(), np->getUpperNodes()[0]->getNumber());
                }else{
                    cout<<"print unknown type node."<<endl;
                }
            }
        }
    }

    fclose(fileID);
    return 1;
}

int printLevel(){

    int i;
    char fileName[] = "logic_level.txt";
    FILE* fileID;
    Node* np;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"level file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberNodes;i++){
        np=&NodePointer[i];
        fprintf(fileID, "%d %d\n", np->getNumber(), np->getLevel());
    }

    fclose(fileID);
    return 1;
}

int printWaveform(){

    map<int, vector<GEVENT> >::iterator i;
    char fileName[] = "waveform.txt";
    FILE* fileID;
    int j;
    vector<GEVENT> temp;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"waveform file can not be opened."<<endl;
        return 0;
    }

    for(i=displayBuffer.begin();i!=displayBuffer.end();i++){
        fprintf(fileID, "%d ", i->first);
        temp = i->second;

        if(temp[0].time!=0){
            fprintf(fileID, "x(%d-%d), ", 0, temp[0].time);
        }

        for(j=0;j<temp.size()-1;j++){
            fprintf(fileID, "%d(%d-%d), ", temp[j].logic, temp[j].time, temp[j+1].time);
        }
        fprintf(fileID, "%d(%d-%d)\n", temp[j].logic, temp[j].time, (int)InputSequence.size()*ClockPeriod);
    }

    fclose(fileID);
    return 1;
}

int printPO_FF(){

    char fileName[] = "PO_FF_state.txt";
    FILE* fileID;
    int i,j,k;
    vector<GEVENT> temp;

    int cycle=InputSequence.size();
    int ppi[NumberPPI][cycle];
    int po[NumberPO][cycle];

    for(i=0;i<NumberPPI;i++){
        for(j=0;j<cycle;j++){
            ppi[i][j]=2;
        }
    }

    for(i=0;i<NumberPO;i++){
        for(j=0;j<cycle;j++){
            po[i][j]=2;
        }
    }

    //process ppi array
    for(i=0;i<NumberPPI;i++){
        temp=displayBuffer.find(PPIList[i]->getNumber())->second;
        for(j=0;j<temp.size();j++){
            ppi[i][temp[j].time/ClockPeriod]=temp[j].logic;
        }
        k=ppi[i][0];
        for(j=0;j<cycle;j++){
            if(ppi[i][j]==2){
                ppi[i][j]=k;
            }else{
                k=ppi[i][j];
            }
        }
    }

    //process po array
    for(i=0;i<NumberPO;i++){
        temp=displayBuffer.find(PrimaryOutput[i]->getNumber())->second;
        for(j=0;j<temp.size();j++){
            po[i][temp[j].time/ClockPeriod]=temp[j].logic;
        }
        k=po[i][0];
        for(j=0;j<cycle;j++){
            if(po[i][j]==2){
                po[i][j]=k;
            }else{
                k=po[i][j];
            }
        }
    }

    //data pre-process is ready
    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"PO and FF state file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberPPI;i++){
        fprintf(fileID,"%d ", PPIList[i]->getNumber());
        for(j=0;j<cycle;j++){
            fprintf(fileID,"%d ", ppi[i][j]);
        }
        fprintf(fileID, "\n");
    }

    for(i=0;i<NumberPO;i++){
        fprintf(fileID, "%d ", PrimaryOutput[i]->getNumber());
        for(j=0;j<cycle;j++){
            fprintf(fileID,"%d ", po[i][j]);
        }
        fprintf(fileID, "\n");
    }

    fclose(fileID);
    return 1;
}

int printGlitch(){

    char fileName[] = "report_on_glitches.txt";
    FILE* fileID;
    int i,j;
    vector<GEVENT> temp;
    int cycle = InputSequence.size();
    int ppo[NumberPPO][cycle];

    //pre-process ppo array
    for(i=0;i<NumberPPO;i++){
        for(j=0;j<cycle;j++){
            ppo[i][j]=2;
        }
    }

    for(i=0;i<NumberPPO;i++){
        temp=displayBuffer.find(PPOList[i]->getNumber())->second;
        for(j=0;j<temp.size();j++){
            if(temp[j].time%ClockPeriod==0 || (temp[j].time+1)%ClockPeriod==0){
                ppo[i][temp[j].time/ClockPeriod]=temp[j].logic;
            }
        }
    }

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"waveform file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberPPO;i++){
        fprintf(fileID, "%d ", PPOList[i]->getNumber());
        for(j=0;j<cycle;j++){
            switch (ppo[i][j]){
                case 0: fprintf(fileID, "F "); break;
                case 1: fprintf(fileID, "R "); break;
                case 2: fprintf(fileID, "- "); break;
            }
        }
        fprintf(fileID, "\n");
    }

    fclose(fileID);
    return 1;
}

int printDominance(){

    char fileName[] = "equivalence_and_dominance_relationship.txt";
    FILE* fileID;
    int i,j,k;
    F_number* temp;
    Node* np;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"dominance file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberNodes;i++){
        np = &NodePointer[i];
        if(np->getFaultListCount()!=0){
            for(j=0;j<np->getFaultListCount();j++){
                temp=np->getDomRelation(j);
                for(k=0;k<temp->domPointer.size();k++){
                    fprintf(fileID, "DOM CLB %d %d CLB %d %d\n", np->getNumber(), temp->fault,
                            np->getNumber(), temp->domPointer[k]->fault);
                }
            }
        }
    }

    fclose(fileID);
    return 1;
}

int printFilteredFault(){

    char fileName[] = "filtered_fault_list.txt";
    FILE* fileID;
    int i,j,k;
    F_number* temp;
    Node* np;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"filtered fault file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberNodes;i++){
        np=&NodePointer[i];
        if(np->getFaultListCount()!=0){
            for(j=0;j<np->getFaultListCount();j++){
                temp=np->getDomRelation(j);
                if(temp->domPointer.size()==0){
                    fprintf(fileID, "CLB %d %d\n", np->getNumber(), temp->fault);
                }
            }
        }
    }

    fclose(fileID);
    return 1;
}

int printTestPattern(){

    char fileName[] = "atpg_for_faults.txt";
    FILE* fileID;
    int i,j,k,counter=0;
    map<int, vector<int> >::iterator it;
    vector<int> temp;

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"test pattern file can not be opened."<<endl;
        return 0;
    }

    for(it=filteredFault.begin();it!=filteredFault.end();it++){
        temp = it->second;
        for(i=0;i<temp.size();i++){
            fprintf(fileID, "CLB %d %d\n", it->first, temp[i]);
            for(j=0;j<NumberPPI;j++){
                fprintf(fileID, "%d %s ", PPIList[j]->getNumber(), translate(testPattern[counter][j]).c_str());
            }
            fprintf(fileID, "\n");
            for(;j<testPattern[counter].size();j++){
                fprintf(fileID, "%s ", translate(testPattern[counter][j]).c_str());
            }
            fprintf(fileID, "\n");
            counter++;
        }
    }
    
    fclose(fileID);
    return 1;
}


