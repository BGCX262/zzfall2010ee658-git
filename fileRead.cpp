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

#include <string.h>
#include <iostream>
#include <vector>
#include <map>

#include "Node.h"
#include "Command.h"
#include "global.h"
#include "type.h"
#include "fileRead.h"
#include "preprocess.h"
using namespace std;

int cread(char *charPointer){

    char buffer[MAXLINE];
    FILE * fileID;

    unsigned int type, number, gate, fanout, fanin;
    unsigned int i,j;
    unsigned int num;

    vector<GEVENT> events;
    vector<int> result;
    
    sscanf(charPointer, "%s", buffer); //scan name of netlist file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"netlist file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }
    
    //first pass to get number of nodes, PI, FB, and PO
    while(fgets(buffer, MAXLINE, fileID) != NULL){ //read one line from netlist file
        if(sscanf(buffer, "%d %d %d", &type, &number, &gate) == 3){ //scan the first three numbers
            NumberNodes++;
            switch (type){
                case GATE:
                    if(gate == FLIPFLOP){
                        NumberPPI++;
                        NumberPPO++;
                    }
                    break;
                case PI: NumberPI++; break;
                case FB: NumberFB++; break;
                case PO: NumberPO++; break;
                default: cout<<"unknow node type."<<endl;
            }
        }
    }
    cout<<"first pass is complete."<<endl;

    //number of PI, FB, PO are determined
    if(allocate()){//allocate dynamic memory for nodes
    cout<<"memory allocation is complete."<<endl;
    }

    fseek(fileID, 0L, 0);//reset file pointer
    i=0;
    //second pass to allocate dynamic memory for fanin and fanout pointers
    while(fscanf(fileID, "%d %d %d", &type, &number, &gate) == 3){

            NodePointer[i].setIndex(i);
            NodePointer[i].setType(type);
            NodePointer[i].setNumber(number);
            NodePointer[i].setGate(gate);
            displayBuffer.insert(make_pair(number,events));
            compareBuffer.insert(make_pair(number,result));

            switch (type){
                case GATE:
                    if(fscanf(fileID, "%d %d", &fanout, &fanin) == 2){
                        NodePointer[i].setDownNodes(new Node* [fanout]);
                        NodePointer[i].setUpperNodes(new Node* [fanin]);
                        NodePointer[i].setFanout(fanout);
                        NodePointer[i].setFanin(fanin);
                        for(j=0;j<fanin;j++){
                            fscanf(fileID, "%d", &num);
                        }
                    }break;
                case PI:
                    if(fscanf(fileID, "%d", &fanout) == 1){
                        NodePointer[i].setDownNodes(new Node* [fanout]);
                        NodePointer[i].setFanout(fanout);
                        fscanf(fileID, "%d", &num);
                    }break;
                case FB:
                    NodePointer[i].setUpperNodes(new Node*);
                    NodePointer[i].setDownNodes(new Node*);
                    NodePointer[i].setFanout(1);
                    NodePointer[i].setFanin(1);
                    fscanf(fileID, "%d", &num);
                    break;
                case PO:
                    if(fscanf(fileID, "%d %d", &fanout, &fanin) == 2){
                        NodePointer[i].setUpperNodes(new Node* [fanin]);
                        NodePointer[i].setFanin(fanin);
                        for(j=0;j<fanin;j++){
                            fscanf(fileID, "%d", &num);
                        }
                    }break;
                default: cout<<"unknow node type."<<endl;
            }

            i++;

    }
    cout<<"second pass is complete."<<endl;

    fseek(fileID, 0L, 0); //reset file pointer
    i=0;
    //third pass to configure fanin and fanout pointers
    while(fscanf(fileID, "%d %d %d", &type, &number, &gate) == 3){
            switch (type){
                case GATE:
                    if(fscanf(fileID, "%d %d", &fanout, &fanin) == 2){
                        for(j=0; j<fanin; j++){
                            if(fscanf(fileID, "%d", &num) == 1){
                                connectPointer(&(NodePointer[i]), num);
                            }
                        }
                    }break;
                case PI:
                    if(fscanf(fileID, "%d", &fanout) == 1){
                        fscanf(fileID, "%d", &fanin);
                    }break;
                case FB:
                    if(fscanf(fileID, "%d", &num) == 1){
                        connectPointer(&NodePointer[i], num);
                    }break;
                case PO:
                    if(fscanf(fileID, "%d %d", &fanout, &fanin) == 2){
                        for(int j=0; j<fanin; j++){
                            if(fscanf(fileID, "%d", &num) == 1){
                                connectPointer(&NodePointer[i], num);
                            }
                        }
                    }break;
                default: cout<<"unknow node type."<<endl;
            }

            i++;
    }

    cout<<"third pass is complete."<<endl;

    for(i=0,j=0;i<NumberNodes;i++){
        if(NodePointer[i].getType()==PI){
            PrimaryInput[j]=&NodePointer[i];
            j++;
        }
    }

    for(i=0,j=0;i<NumberNodes;i++){
        if(NodePointer[i].getType()==FB){
            FanoutBranch[j]=&NodePointer[i];
            j++;
        }
    }

    for(i=0,j=0;i<NumberNodes;i++){
        if(NodePointer[i].getType()==PO){
            PrimaryOutput[j]=&NodePointer[i];
            j++;
        }
    }

    for(i=0,j=0;i<NumberNodes;i++){
        if(NodePointer[i].getGate()==FLIPFLOP){
            PPIList[j]=&NodePointer[i];
            PPOList[j]=NodePointer[i].getUpperNodes()[0];
            j++;
        }
    }

    fclose(fileID);
    cout<<"circuit read is complete."<<endl;
    return 1;
}

int fread(char *charPointer){

    char buffer[MAXLINE];
    char str[5];
    FILE* fileID;
    int number, fault;

    sscanf(charPointer, "%s", buffer); //scan name of fault list file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"fault list file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }
 //   cout<<"fault file is opened successfully."<<endl;

    while(fgets(buffer, MAXLINE, fileID) != NULL){ //read one line from fault list file
        if(sscanf(buffer, "%s %d %d", str, &number, &fault) == 3){
            findNode(number)->setFaultList(fault);
        }
    }

    fclose(fileID);
    cout<<"fault list readin is complete."<<endl;
    return 1;
}

int iread(char *charPointer){

    char buffer[MAXLINE];
    char str[5];
    FILE* fileID;
    int number, fault;
    vector<int> temp;

    sscanf(charPointer, "%s", buffer); //scan name of filtered fault file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"filtered fault file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }

    while(fgets(buffer, MAXLINE, fileID) != NULL){ //read one line from filtered fault file
        if(sscanf(buffer, "%s %d %d", str, &number, &fault) == 3){
            if(findFilteredNode(number)){
                filteredFault.find(number)->second.push_back(fault);
            }else{
                temp.clear();
                temp.push_back(fault);
                filteredFault.insert(make_pair(number, temp));
            }
        }
    }

    fclose(fileID);
    cout<<"filtered fault readin is complete."<<endl;
    return 1;
}

int findFilteredNode(int number){

    map<int, vector<int> >::iterator i;
    for(i=filteredFault.begin();i!=filteredFault.end();i++){
        if(i->first == number){
            return 1;
        }
    }
    return 0;
}

int tread(char *charPointer){
    char buffer[MAXLINE];
    FILE* fileID;
    int number, logic;

    sscanf(charPointer, "%s", buffer); //scan name of ATPG test file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"ATPG test file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }

    while(fgets(buffer, MAXLINE, fileID) != NULL){ //read one line from ATPG test file
        if(sscanf(buffer, "%d %d", &number, &logic) == 2){
            findNode(number)->setLogicValue_E((e_logic)logic);
        }
    }

    fclose(fileID);
    cout<<"ATPG test file readin is complete."<<endl;
    return 1;
}

int dread(char *charPointer){

    char buffer[MAXLINE];
    FILE* fileID;
    int clock;
    int number, inertialDelay, riseDelay, fallDelay;
    Node* np;

    sscanf(charPointer, "%s", buffer); //scan name of delay file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"delay file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }
//    cout<<"delay file is opened successfully."<<endl;

    if(fgets(buffer, MAXLINE, fileID)!= NULL){
        if(sscanf(buffer, "%d", &clock) == 1){
            ClockPeriod = clock;
        }
    }

    while(fgets(buffer, MAXLINE, fileID) != NULL){ //read one line from delay file
        if(sscanf(buffer, "%d %d %d %d", &number, &inertialDelay, &riseDelay, &fallDelay) == 4){
            np = findNode(number);
            np->setInertialDelay(inertialDelay);
            np->setRiseDelay(riseDelay);
            np->setFallDelay(fallDelay);
        }
    }

    fclose(fileID);
    cout<<"delay file readin is complete."<<endl;
    return 1;
}

int seqread(char *charPointer){

    char buffer[MAXLINE];
    FILE* fileID;
    int number, logic;
    int i,j;
    int counter=0;
    vector<unsigned int> temp;

    sscanf(charPointer, "%s", buffer); //scan name of input sequence file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"input sequence file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }
//   cout<<"input sequence file is opened successfully."<<endl;


    while(fgets(buffer, MAXLINE, fileID) != NULL){
        counter++;
    }

    fseek(fileID, 0L, 0);
    for(i=0;i<NumberPPI;i++){
        if(fscanf(fileID, "%d %d", &number, &logic) == 2){
            FFInitial.push_back(logic);
        }
    }

    for(i=0;i<counter-1;i++){
        for(j=0;j<NumberPI;j++){
            if(fscanf(fileID, "%d", &logic) == 1){
                temp.push_back(logic);
            }
        }
        InputSequence.push_back(temp);
        temp.clear();
    }

    fclose(fileID);
    cout<<"input sequence file readin is complete."<<endl;
    return 1;
}

int vread(char* charPointer){

    char buffer[MAXLINE];
    FILE* fileID;
    int number, logic;
    int i,j;
    int counter=0;
    vector<unsigned int> temp;

    sscanf(charPointer, "%s", buffer); //scan name of input sequence file
    fileID = fopen(buffer, "r");
    if(fileID == NULL){
        cout<<"input vector file "<<buffer<<" does not exist!"<<endl;
        return 0;
    }

    while(fgets(buffer, MAXLINE, fileID) != NULL){
        counter++;
    }
    fseek(fileID, 0L, 0);

    FFInitial_comp.clear();
    for(i=0;i<NumberPPI;i++){
        if(fscanf(fileID, "%d %d", &number, &logic) == 2){
            FFInitial_comp.push_back(logic);
        }
    }

    InputSequence_comp.clear();
    for(i=0;i<counter-1;i++){
        for(j=0;j<NumberPI;j++){
            if(fscanf(fileID, "%d", &logic) == 1){
                temp.push_back(logic);
            }
        }
        InputSequence_comp.push_back(temp);
        temp.clear();
    }

    fclose(fileID);
    cout<<"input vector file readin is complete."<<endl;
    return 1;
}



