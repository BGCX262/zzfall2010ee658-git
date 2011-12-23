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
#include "logicSim.h"
#include "fileRead.h"
#include "fileWrite.h"
#include "preprocess.h"
#include "atpg.h"
using namespace std;

int simulation(){

    char delayFile[] = "delay_values.txt";
    char sequenceFile[] = "logic_simulator_input_sequence.txt";

    dread((char*)delayFile); //read in delay file
    seqread((char*)sequenceFile); //read in input sequence

    fpgaSim_delay();
    printDisplayBuffer();
    printWaveform();
    printPO_FF();
    printGlitch();

    //local ATPG
    /*
    np=findNode(4);
    np->fCubeProcess(np->findFaultNum(13));
    for(i=0;i<np->getFanin();i++){
        np->getUpperNodes()[i]->setLogicValue_E(np->getDectectDbar()[0][i]);
    }
    np->setLogicValue_E(DBAR);
    implyProcess(4,DBAR);
    implyCheck();
     */

//    tread(testATPGFile);
//    findNode(16)->dCubeProcess(0,D);
    return 1;
}

int comparison(){
    int i,j;
    char vectorFile[] = "input_vectors.txt";
    char fileName[] = "PO_FF_state.txt";
    FILE* fileID;
    vector<int> temp;

    vread(vectorFile);
    fpgaSim_comp();

    fileID = fopen(fileName, "w");
    if(fileID==NULL){
        cout<<"PO and FF state file can not be opened."<<endl;
        return 0;
    }

    for(i=0;i<NumberPPI;i++){
        fprintf(fileID, "%d ", PPIList[i]->getNumber());
        temp = (compareBuffer.find(PPIList[i]->getNumber()))->second;
        for(j=0;j<temp.size();j++){
            fprintf(fileID, "%d ", temp[j]);
        }
        fprintf(fileID, "\n");
    }

    for(i=0;i<NumberPO;i++){
        fprintf(fileID, "%d ", PrimaryOutput[i]->getNumber());
        temp = (compareBuffer.find(PrimaryOutput[i]->getNumber()))->second;
        for(j=0;j<temp.size();j++){
            fprintf(fileID, "%d ", temp[j]);
        }
        fprintf(fileID, "\n");
    }
    fclose(fileID);
    return 1;
}

int fpgaSim_delay(){
    int i;
    //logic simulation with delay
    for(i=0;i<InputSequence.size();i++){
        cout<<"------------------"<<endl;
        cout<<"cycle: "<<i<<endl;
        cout<<"------------------"<<endl;
        initialSim(i);
        startupSim(i);
        logicSim_delay();
    }
    
    return 1;
}

int fpgaSim_comp(){

    int i;
    map<int, vector<int> >::iterator it;
    
    for(it=compareBuffer.begin();it!=compareBuffer.end();it++){
        (it->second).clear();
    }

    for(i=0;i<InputSequence_comp.size();i++){
        initialSim_comp(i);
        logicSim_comp();
    }
    printCompareBuffer();
    return 1;
}

int logicSim_comp(){

    int level,i;
    for(level=1;level<=NumberLevel;level++){
        for(i=0;i<NumberNodes;i++){
            if(NodePointer[i].getLevel() == level){
                NodePointer[i].setLogicValue(evaluate(&NodePointer[i]));
            }
        }
    }

    for(i=0;i<NumberNodes;i++){
        compareBuffer.find(NodePointer[i].getNumber())->second.push_back(NodePointer[i].getLogicValue());
    }
    return 1;
}

int evaluate(Node* np){

    int j;
    int input;
    int weight;
    int logic;

    if(np->getType()==PI || np->getType()==PPI){
        logic = np->getLogicValue();
    }else{
        if(np->getType()==FB){
            logic = np->getUpperNodes()[0]->getLogicValue();
        }else{
            for(j=2,input=0,weight=1;j>=0;j--){
                input += np->getUpperNodes()[j]->getLogicValue() * weight;
                weight *=2;
            }
            logic = np->getLogicOutput(input);
        }
    }

    return logic;
}

//event-driven logic simulation using two-pass strategy as shown in Fig. 3.31 in the textbook
int logicSim_delay(){
    int currentTime;
    TSTAMP* currentStamp;
    LEVENT* ep;
    Node* np;
    int i;
    int logic;
    vector<Node*> activatedLine;

    while(currentStamp = queuePop()){

        currentTime = currentStamp->time;
        cout<<"current time = "<<currentTime<<endl;

        activatedLine.clear();
        //first pass
        while(currentStamp->eventHead != NULL){
            ep = currentStamp->eventHead;
            np = ep->line;
            np->setLogicValue(ep->logic);
            displayBufferPush(np->getNumber(),currentTime,ep->logic);
            cout<<"node "<<np->getNumber()<<" is processed with logic of "<<ep->logic<<endl;

            for(i=0;i<np->getFanout();i++){
                activatedLine.push_back(np->getDownNodes()[i]); //todo not all fanout are activated
            }

            currentStamp->eventHead = ep->nextEvent;
            delete ep;
        }

        //second pass
        for(i=0;i<activatedLine.size();i++){
            logic = evaluate(activatedLine[i]);

            if(logic != activatedLine[i]->getLSV()){
                queuePush(currentTime+rfDelay(activatedLine[i],logic),activatedLine[i],logic);
                activatedLine[i]->setLSV(logic);

                cout<<"node "<<activatedLine[i]->getNumber()<<" is scheduled with logic of "<<logic<<endl;
            }
        }

        //release the memory
        delete currentStamp;
    }
}

void displayBufferPush(int number, int time, int logic){

    int count = displayBuffer.find(number)->second.size();
    if(count==0 || displayBuffer.find(number)->second[count-1].logic != logic){
        GEVENT* gp = new GEVENT;
        gp->logic = logic;
        gp->time = time;

        displayBuffer.find(number)->second.push_back(*gp);
    }
}

int rfDelay(Node* np, int logic){

    int delay;

    if(np->getLogicValue() == logic){
        delay = 0;
    }else{
        if(logic==0){
            delay = np->getFallDelay();
        }else{
            delay = np->getRiseDelay();
        }
    }

    return delay;
}

int initialSim(int cycle){

    int i,logic,temp;
    if(cycle == 0){
        for(i=0;i<NumberPPI;i++){
            temp = FFInitial[i];
            PPIList[i]->setLogicValue(temp);
            displayBufferPush(PPIList[i]->getNumber(),cycle*ClockPeriod,temp);
        }
    }else{
        for(i=0;i<NumberPPI;i++){
            temp = PPOList[i]->getLogicValue();
            PPIList[i]->setLogicValue(temp);
            displayBufferPush(PPIList[i]->getNumber(),cycle*ClockPeriod,temp);
        }
    }
    

    for(i=0;i<NumberPI;i++){
        logic = InputSequence[cycle][i];
        PrimaryInput[i]->setLogicValue(logic);
        displayBufferPush(PrimaryInput[i]->getNumber(),cycle*ClockPeriod,logic);
    }

    for(i=0;i<NumberNodes;i++){
        NodePointer[i].setLSV(2);
    }

    return 1;
}

int initialSim_comp(int cycle){
    int i,temp,logic;
    if(cycle == 0){
        for(i=0;i<NumberPPI;i++){
            temp = FFInitial_comp[i];
            PPIList[i]->setLogicValue(temp);
        }
    }else{
        for(i=0;i<NumberPPI;i++){
            temp = PPOList[i]->getLogicValue();
            PPIList[i]->setLogicValue(temp);
        }
    }
    
    for(i=0;i<NumberPI;i++){
        logic = InputSequence_comp[cycle][i];
        PrimaryInput[i]->setLogicValue(logic);
    }
    
    return 1;
}

int startupSim(int cycle){

    int i,j;
    int logic;
    int delay;

    for(i=0;i<NumberNodes;i++){
        if(NodePointer[i].getLevel()==1){
            logic = evaluate(&NodePointer[i]);
//            cout<<"logic value = "<<logic<<endl;
            delay = rfDelay(&NodePointer[i],logic);
            queuePush(cycle*ClockPeriod+delay,&NodePointer[i],logic);
        }
    }

    return 1;
}

void queuePush(int time, Node* node, int value){

    LEVENT* ep = new LEVENT; //ep:=event pointer
    TSTAMP* temp;

    ep->line = node;
    ep->logic = value;
    ep->nextEvent = NULL;

    if(timeHead==NULL && timeTail == NULL){
        cout<<"queue of time is empty in the initial push"<<endl;

        TSTAMP* tp = new TSTAMP; //tp:=time pointer
        tp->eventHead = ep;
        tp->eventTail = ep;
        tp->nextStamp = NULL;
        tp->time = time;

        timeHead = tp;
        timeTail = tp;

    }else{
        temp=queueFind(time);
        if(temp!=NULL){
            temp->eventTail->nextEvent = ep;
            temp->eventTail = ep;
        }else{
            temp=queueInsert(time);
            temp->eventHead = ep;
            temp->eventTail = ep;
        }
    }
}

TSTAMP* queuePop(){

    TSTAMP* temp;

    if(timeHead == NULL && timeTail == NULL){
        cout<<"queue of time is empty in the final pop"<<endl;
        return NULL;
    }

    if(timeHead == timeTail){
        temp = timeHead;
        timeHead = NULL;
        timeTail = NULL;
        return temp;
    }

    temp = timeHead;
    timeHead = timeHead->nextStamp;
    return temp;
}

TSTAMP* queueFind(int time){
    TSTAMP* tp = timeHead;

    while(tp!=NULL){
        if(tp->time == time){
            return tp;
        }else{
            tp=tp->nextStamp;
        }
    }

    return NULL;
}

TSTAMP* queueInsert(int time){
    TSTAMP* tp = new TSTAMP;
    tp->eventHead = NULL;
    tp->eventTail = NULL;
    tp->nextStamp = NULL;
    tp->time = time;

    TSTAMP* pre = timeHead;
    TSTAMP* pro = timeHead->nextStamp;

    if(time > timeTail->time){
        timeTail->nextStamp = tp;
        timeTail = tp;
    }else{
        if(time < timeHead->time){
            tp->nextStamp = timeHead;
            timeHead = tp;
        }else{
            while(pro!=NULL){
                if(pre->time < time && pro->time > time){
                    pre->nextStamp = tp;
                    tp->nextStamp = pro;
                    return tp;
                }else{
                    pre = pre->nextStamp;
                    pro = pro->nextStamp;
                }
            }
        }
    }

    return tp;
}


