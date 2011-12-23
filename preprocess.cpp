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
#include "fileRead.h"
#include "fileWrite.h"
#include "preprocess.h"
using namespace std;

int preprocess(){

    int i;
    e_ntype temp;
    char netlistFile[] = "netlist.txt";
    char faultListFile[] = "original_fault_list.txt";

    cread((char*)netlistFile); //read in circuit netlist
    fread((char*)faultListFile); //read in fault list

    removeFF(); //strip off flip-flop
    lev(); //must levelize circuit after stripping off flip flop
    printScanNetlist(); //print out scan netlist file
    printLevel(); //print out netlist level

    for(i=0;i<NumberNodes;i++){
        temp = (e_ntype)NodePointer[i].getType();
        if(temp==GATE || temp==PO || temp==PPO){
            NodePointer[i].pCubeProcess(); //primitive cube process
            NodePointer[i].domProcess();
        }
    }
    printDominance();
    printFilteredFault();
    return 1;
}

int help(){

    cout<<"READ filename - ";
    cout<<"read in circuit file and creat all data structures\n";
    cout<<"PC - ";
    cout<<"print circuit information\n";
    cout<<"HELP - ";
    cout<<"print this help information\n";
    cout<<"QUIT - ";
    cout<<"stop and exit\n";
    cout<<"LEV - ";
    cout<<"levelize circuit\n";

    return 1;
}

int quit(){

    Done = 1;
    return 1;
}

int lev(){

    Node* npointer;
    int count,i,j,temp;
    ReadyList = new Node* [NumberNodes];

    initialPI_PPI();
    while(!checkPO_PPO()){
        count=0;
        for(i=0;i<NumberNodes;i++){
            npointer=&NodePointer[i];
            if(npointer->getNodeReady() == 0){
                temp = npointer->getInputReady();
                for(j=0;j<npointer->getFanin();j++){
                    if(npointer->getUpperNodes()[j]->getNodeReady()==1){
                        npointer->setInputReady(++temp);
                    }
                }
                if(npointer->getInputReady() == npointer->getFanin()){
                    ReadyList[count] = npointer;
                    count++;
                }else{
                    npointer->setInputReady(0);
                }
            }
        }

        for(i=0;i<count;i++){
            ReadyList[i]->setLevel(levelComp(ReadyList[i]));
            ReadyList[i]->setNodeReady(1);
            ReadyList[i]=NULL;
        }
    }

    cout<<"levelizing is complete.\n";
    return 1;
}

int levelComp(Node* np){

    int level,temp,i;
    if(np->getType() == FB){
        level = np->getUpperNodes()[0]->getLevel()+1;
    }else{
        if(np->getType() == GATE || np->getType() == PO || np->getType() == PPO){
            level = np->getUpperNodes()[0]->getLevel();
            for(i=0; i<np->getFanin(); i++){
                temp = np->getUpperNodes()[i]->getLevel();
                if(temp > level){
                    level = temp;
                }
            }
            level++;
        }else{
            cout<<"level calculation of unknown node type.\n";
        }
    }

    if(level > NumberLevel) NumberLevel = level;
    return level;
}

int removeFF(){

    int i;
    for(i=0;i<NumberPPI;i++){
        PPIList[i]->setUpperNodes(NULL);
        PPIList[i]->setType(PPI);
        PPIList[i]->setFanin(0);
        PPIList[i]->setGate(259);
    }

    for(i=0;i<NumberPPO;i++){
        PPOList[i]->setDownNodes(NULL);
        PPOList[i]->setType(PPO);
        PPOList[i]->setFanout(0);
    }

    return 1;
}

Node* findNode(int number){
    int i;
    for(i=0; i<NumberNodes; i++){
        if(NodePointer[i].getNumber() == number){
            return &NodePointer[i];
        }
    }

    return NULL;
}

int connectPointer(Node* nodePointer, int num){

//    cout<<"node="<<nodePointer->getNumber()<<" num="<<num<<"\n";
    int upperCounter = nodePointer->getUpperCounter();

    Node *temp = findNode(num);
    nodePointer->getUpperNodes()[upperCounter] = temp;
    nodePointer->setUpperCounter(upperCounter+1);

    int downCounter = temp->getDownCounter();
    temp->getDownNodes()[downCounter] = nodePointer;
    temp->setDownCounter(downCounter+1);

    return 1;
}

int allocate(){

    NodePointer = new Node [NumberNodes];
    if(NodePointer == NULL){
        cout<<"Node dynamic allocation fail."<<endl;
        return 0;
    }

    PrimaryInput = new Node* [NumberPI];
    if(PrimaryInput == NULL){
        cout<<"PI dynamic allocation fail."<<endl;
        return 0;
    }

    PrimaryOutput = new Node* [NumberPO];
    if(PrimaryOutput == NULL){
        cout<<"PO dynamic allocation fail."<<endl;
        return 0;
    }

    FanoutBranch = new Node* [NumberFB];
    if(FanoutBranch == NULL){
        cout<<"FB dynamic allocation fail."<<endl;
    }

    PPIList = new Node* [NumberPPI];
    if(PPIList == NULL){
        cout<<"PPI dynamic allocation fail."<<endl;
    }

    PPOList = new Node* [NumberPPO];
    if(PPOList == NULL){
        cout<<"PPO dynamic allocation fail."<<endl;
    }

    return 1;
}

int clear(){

    int i;
    for(i=0; i<NumberNodes; i++){
        delete [] NodePointer[i].getUpperNodes();
        delete [] NodePointer[i].getDownNodes();
    }

    delete [] NodePointer;
    delete [] PrimaryInput;
    delete [] FanoutBranch;
    delete [] PrimaryOutput;

    delete [] PPIList;
    delete [] PPOList;
    delete [] ReadyList;

    NumberNodes = 0;
    NumberPI = 0;
    NumberFB = 0;
    NumberPO = 0;

    NumberPPI = 0;
    NumberPPO = 0;
    Done = 0;

    NumberLevel = 0;
    ClockPeriod = 0;

    FFInitial.clear();
    InputSequence.clear();

    FFInitial_comp.clear();
    InputSequence_comp.clear();

    delete timeHead;
    delete timeTail;

    delete assignHead;
    delete assignTail;

    displayBuffer.clear();
    compareBuffer.clear();

    GlobalState = EXEC;

    return 1;
}

int pc(){

    int i,j;

    for(i=0;i<NumberNodes;i++){
         cout<<"number: "<<NodePointer[i].getNumber();
         cout<<" level: "<<NodePointer[i].getLevel();
         //cout<<" fault: ";
         //for(j=0;j<NodePointer[i].getFaultListCount();j++){
         //    cout<<NodePointer[i].getFaultList(j)<<" ";
         //}
         //cout<<"inertial: "<<NodePointer[i].getInertialDelay();
         //cout<<" rise: "<<NodePointer[i].getRiseDelay();
         //cout<<" fall: "<<NodePointer[i].getFallDelay();
         //cout<<" logic: "<<NodePointer[i].getLogicValue();
         //cout<<" gate: "<<NodePointer[i].getGate();
         cout<<" logic: "<<translate(NodePointer[i].getLogicValue_E());
         cout<<" logicReady: "<<NodePointer[i].getLogicReady();
         cout<<endl;
    }

    cout<<"Primary inputs: ";
    for(i=0;i<NumberPI;i++){
        cout<<PrimaryInput[i]->getNumber()<<" ";
    }
    cout<<endl;

    cout<<"Fanout branches: ";
    for(i=0;i<NumberFB;i++){
        cout<<FanoutBranch[i]->getNumber()<<" ";
    }
    cout<<endl;

    cout<<"Primary outputs: ";
    for(i=0;i<NumberPO;i++){
        cout<<PrimaryOutput[i]->getNumber()<<" ";
    }
    cout<<endl;

    cout<<"PPI: ";
    for(i=0;i<NumberPPI;i++){
        cout<<PPIList[i]->getNumber()<<" ";
    }
    cout<<endl;

    cout<<"PPO: ";
    for(i=0;i<NumberPPO;i++){
        cout<<PPOList[i]->getNumber()<<" ";
    }
    cout<<endl;

    cout<<"Number of nodes = "<<NumberNodes<<endl;
    cout<<"Number of primary inputs = "<<NumberPI<<endl;
    cout<<"Number of fanout branches = "<<NumberFB<<endl;
    cout<<"Number of primary outputs = "<<NumberPO<<endl;
    cout<<"Number of PPI = "<<NumberPPI<<endl;
    cout<<"Number of PPO = "<<NumberPPO<<endl;
    cout<<"Max number of level = "<<NumberLevel<<endl;
    cout<<"Clock period = "<<ClockPeriod<<endl;
    cout<<"Flipflop initial value = ";
    for(i=0;i<FFInitial.size();i++){
        cout<<FFInitial[i]<<" ";
    }
    cout<<endl;

    for(i=0;i<InputSequence.size();i++){
        for(j=0;j<InputSequence[i].size();j++){
            cout<<InputSequence[i][j]<<" ";
        }
        cout<<endl;
    }
    return 1;
}

int initialPI_PPI(){

    int i;
    for(i=0;i<NumberPI;i++){
//        PrimaryInput[i]->setLevel(0);
        PrimaryInput[i]->setNodeReady(1);
    }

    for(i=0;i<NumberPPI;i++){
        PPIList[i]->setNodeReady(1);
    }

    return 1;
}

int checkPO_PPO(){

    int i;
    for(i=0;i<NumberPO;i++){
        if(PrimaryOutput[i]->getNodeReady() == 0){
            return 0;
        }
    }

    for(i=0;i<NumberPPO;i++){
        if(PPOList[i]->getNodeReady() == 0){
            return 0;
        }
    }
    return 1;
}

