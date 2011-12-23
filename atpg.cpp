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
#include "preprocess.h"
#include "atpg.h"
#include "fileRead.h"
#include "fileWrite.h"
using namespace std;

int atpg(){
    
    char filteredFaultFile[] = "filtered_fault_list.txt";
    map<int, vector<int> >::iterator i;
    int j,k;
    vector<int> temp;
    
    iread(filteredFaultFile);
    for(i=filteredFault.begin();i!=filteredFault.end();i++){
        temp = i->second;
        cout<<i->first<<": ";
        for(j=0;j<temp.size();j++){
            cout<<temp[j]<<" ";
        }
        cout<<endl;
    }

    for(i=filteredFault.begin();i!=filteredFault.end();i++){
        temp = i->second;
        for(j=0;j<temp.size();j++){
            cout<<"CLB "<<i->first<<" with fault "<<temp[j]<<" is run for ATPG."<<endl;
            initialATPG();
            startupATPG(i->first,temp[j]);
            storeTestPattern();
        }
    }

    for(j=0;j<testPattern.size();j++){
        for(k=0;k<testPattern[j].size();k++){
            cout<<translate(testPattern[j][k])<<" ";
        }
        cout<<endl;
    }

    printTestPattern();
    return 1;
    
}

int initialATPG(){
    int i;
    for(i=0;i<NumberNodes;i++){
        NodePointer[i].nodeClear();
    }
    return 1;
}

int storeTestPattern(){
    int i;
    vector<e_logic> temp;

    for(i=0;i<NumberPPI;i++){
        temp.push_back(PPIList[i]->getLogicValue_E());
    }

    for(i=0;i<NumberPI;i++){
        temp.push_back(PrimaryInput[i]->getLogicValue_E());
    }

    testPattern.push_back(temp);
    
    return 1;
}

int startupATPG(int number, int fault){
    Node* np;
    vector< vector<e_logic> > temp;
    int i,j;
    
    np=findNode(number);
    np->fCubeProcess(np->findFaultNum(fault));

    temp = np->getDetectD();
    if(temp.size()!=0){
        for(i=0;i<temp.size();i++){
            for(j=0;j<temp[i].size();j++){
                np->getUpperNodes()[j]->setLogicValue_E(temp[i][j]);
            }
            np->setLogicValue_E(D);
            if(D_alg(np)){
                //store test pattern
                return 1;
            }
        }
    }

    temp = np->getDectectDbar();
    if(temp.size()!=0){
        for(i=0;i<temp.size();i++){
            for(j=0;j<temp[i].size();j++){
                np->getUpperNodes()[j]->setLogicValue_E(temp[i][j]);
            }
            np->setLogicValue_E(DBAR);
            if(D_alg(np)){
                //store test pattern
                return 1;
            }
        }
    }

    return 0;
}

vector<Node*> findJFrontier(){
    int i,j;
    Node* np;
    e_ntype type;
    vector<e_logic> temp;
    vector<Node*> jFrontier;

    for(i=0;i<NumberNodes;i++){
        np = &NodePointer[i];
        type = (e_ntype)np->getType();
        if(type==GATE || type==PO || type==PPO){
            
            for(j=0;j<np->getFanin();j++){
                temp.push_back(np->getUpperNodes()[j]->getLogicValue_E());
            }
            if(np->getLogicValue_E()==ONE && !(np->searchOutputOne(temp))){
                jFrontier.push_back(np);
            }
            if(np->getLogicValue_E()==ZERO && !(np->searchOutputZero(temp))){
                jFrontier.push_back(np);
            }
        }
    }

    cout<<"J-frontier: ";
    for(i=0;i<jFrontier.size();i++){
        cout<<jFrontier[i]->getNumber()<<" ";
    }
    cout<<endl;
    return jFrontier;
}

//find node of D-frontier in the netlist
vector<Node*> findDFrontier(){
    int i;
    e_ntype type;
    vector<Node*> dFrontier;

    for(i=0;i<NumberNodes;i++){
        type = (e_ntype)NodePointer[i].getType();
        if(type==GATE || type==PO || type==PPO){
            if(findD_Dbar(&NodePointer[i]) && NodePointer[i].getLogicValue_E()==X){
                dFrontier.push_back(&NodePointer[i]);
            }
        }
    }

    cout<<"D-frontier: ";
    for(i=0;i<dFrontier.size();i++){
        cout<<dFrontier[i]->getNumber()<<" ";
    }
    cout<<endl;
    return dFrontier;
}

//find D or Dbar on the fanin of the node and return index+1
int findD_Dbar(Node* np){
    int i;
    e_logic logic;

    for(i=0;i<np->getFanin();i++){
        logic = np->getUpperNodes()[i]->getLogicValue_E();
        if(logic==D || logic==DBAR){
            return i+1; //trick code
        }
    }
    return 0;
}

int faultAtPO_PPO(){

    int i;
    e_logic temp;

    for(i=0;i<NumberPO;i++){
        temp = PrimaryOutput[i]->getLogicValue_E();
        if(temp==D || temp==DBAR){
            return 1;
        }
    }

    for(i=0;i<NumberPPO;i++){
        temp = PPOList[i]->getLogicValue_E();
        if(temp==D || temp==DBAR){
            return 1;
        }
    }

    return 0;
}

int inputAllSpecified(){
    int i;
    for(i=0;i<NumberPI;i++){
        if(PrimaryInput[i]->getLogicReady()==0){
            return 0;
        }
    }
    for(i=0;i<NumberPPI;i++){
        if(PPIList[i]->getLogicReady()==0){
            return 0;
        }
    }
    return 1;
}

//core code of D-algorithm for ATPG 1=success 0=failure as shown in Fig. 6.23 of textbook
int D_alg(Node* np){
    Node* dp;
    Node* jp;
    int i,j,index;
    vector<Node*> localDFrontier;
    vector<Node*> localJFrontier;
    vector< vector<e_logic> > temp;

    //if imply and check fail then retrun fail
    if(!implyCheck(np)){
        return 0;
    }

    localDFrontier=findDFrontier(); //update set of D-frontier

    //D-drive procedure
    if(!faultAtPO_PPO()){ //check if error is at PO
        if(localDFrontier.size()==0){ //if D-frontier is null then return fail
            return 0;
        }
        while(localDFrontier.size()!=0){
            //select one untried CLB from D-frontier
            dp=localDFrontier[localDFrontier.size()-1];
            cout<<"Node "<<dp->getNumber()<<" is selected for D drive."<<endl;
            localDFrontier.pop_back();
            //D cube process for the selected node
            index = findD_Dbar(dp)-1;
            dp->dCubeProcess(2-index, dp->getUpperNodes()[index]->getLogicValue_E()); //trick code 2-index
            temp = dp->getDDrive();
            //drive fault through the selected CLB
            
            if(temp.size()!=0){
                for(i=0;i<temp.size();i++){
                    for(j=0;j<temp[i].size()-1;j++){
                        dp->getUpperNodes()[j]->setLogicValue_E(temp[i][j]);
                    }
                    dp->setLogicValue_E(temp[i][3]);
                    if(D_alg(dp)){ //if recursive D_alg success then return success
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    //error propagate to one of POs

    //line justification procedure
    localJFrontier = findJFrontier();
    if(localJFrontier.size()==0){ //if J-frontier is null then return success
        return 1;
    }
    jp=localJFrontier[localJFrontier.size()-1];
    localJFrontier.pop_back();

    while(!inputAllSpecified()){
        if(jp->getLogicValue_E()==ONE){
            temp = jp->getOutputOne();
        }else{
            temp = jp->getOutputZero();
        }

        if(temp.size()!=0){
            for(i=0;i<temp.size();i++){
                for(j=0;j<temp[i].size();j++){
                    jp->getUpperNodes()[j]->setLogicValue_E(temp[i][j]);
                }
                if(D_alg(jp)){
                    return 1;
                }
            }
        }
        return 0;
    }

    return 0; //?
}

int implyCheck(Node* np){

    int i;
    for(i=0;i<np->getFanin();i++){
        if(!implyProcess(np->getUpperNodes()[i]->getNumber(), np->getUpperNodes()[i]->getLogicValue_E(), BACKWARD)){
            return 0;
        }
    }
    if(!implyProcess(np->getNumber(), np->getLogicValue_E(), FORWARD)){
        return 0;
    }
    return 1;
}

int implyProcess(int num, e_logic logic, e_direction direction){

    ASSIGN* temp;
    atpgPush(findNode(num), logic, direction);
    while(temp=atpgPop()){
        if(!propagate(temp->line, temp->logic, temp->direct)){
            delete temp;
            return 0;
        }
        delete temp;
    }
    return 1;
}

int propagate(Node* node, e_logic value, e_direction direction){
    int i;
    e_logic logic = node->getLogicValue_E();
    e_logic temp;

    //check consistancy
    if(logic!=X && logic!=value){
        cout<<"An inconsistancy is detected at line "<<node->getNumber()<<endl;
        return 0;
    }else{
        node->setLogicValue_E(value); //implied value is consistant
    }

    //propagate signal with direction
    if(node->getType()==PI || node->getType()==PPI){
        node->setLogicReady(1);
        if(direction == FORWARD){
            if(node->getFanout() > 1){
                for(i=0;i<node->getFanout();i++){
                    atpgPush(node->getDownNodes()[i], value, FORWARD);
                }
            }else{
                temp = atpgEvaluate(node->getDownNodes()[0]);
                if(temp != X){
                    atpgPush(node->getDownNodes()[0], temp, FORWARD);
                }
            }
        }else{
            cout<<"PI or PPI can not propagate backward."<<endl;
        }
    }else{
        if(node->getType()==FB){
            if(direction == FORWARD){
                temp = atpgEvaluate(node->getDownNodes()[0]);
                if(temp != X){
                    atpgPush(node->getDownNodes()[0], temp, FORWARD);
                }
            }else{
                atpgPush(node->getUpperNodes()[0], value, BACKWARD);
                for(i=0;i<node->getUpperNodes()[0]->getFanout();i++){
                    if(node->getUpperNodes()[0]->getDownNodes()[i]->getNumber() != node->getNumber()){
                        atpgPush(node->getUpperNodes()[0]->getDownNodes()[i], value, FORWARD);
                    }
                }
            }
        }else{
            if(direction == FORWARD){
                if(node->getFanout() > 1){
                    for(i=0;i<node->getFanout();i++){
                        atpgPush(node->getDownNodes()[i], value, FORWARD);
                    }
                }else{
                    if(node->getFanout()==1){
                        temp = atpgEvaluate(node->getDownNodes()[0]);
                        if(temp != X){
                            atpgPush(node->getDownNodes()[0], temp, FORWARD);
                        }
                    }
                }
            }else{
                if(value==ONE && node->getOutputOne().size()==1){
                    for(i=0;i<node->getFanin();i++){
                        atpgPush(node->getUpperNodes()[i], node->getOutputOne()[0][i], BACKWARD);
                    }
                }
                if(value==ZERO && node->getOutputZero().size()==1){
                    for(i=0;i<node->getFanin();i++){
                        atpgPush(node->getUpperNodes()[i], node->getOutputZero()[0][i], BACKWARD);
                    }
                }
                if(value==X){
                    for(i=0;i<node->getFanin();i++){
                        atpgPush(node->getUpperNodes()[i], X, BACKWARD);
                    }
                }
            }
        }
    }

    return 1;
}

e_logic atpgEvaluate(Node* node){

    vector<e_logic> temp;
    int i;
    e_logic logic = X;;

    if(node->getType()==PI || node->getType()==PPI){
        logic = node->getLogicValue_E();
    }else{
        if(node->getType()==FB){
            logic = node->getUpperNodes()[0]->getLogicValue_E();
        }else{
            for(i=0;i<node->getFanin();i++){
            temp.push_back(node->getUpperNodes()[i]->getLogicValue_E());
            }

            if(node->searchOutputOne(temp)){
            logic = ONE;
            }

            if(node->searchOutputZero(temp)){
            logic = ZERO;
            }
        }
    }

    return logic;
}

//va=primitive cube vb=pattern
int compatible_V(vector<e_logic> va, vector<e_logic> vb){
    int i;
    if(va.size()!=vb.size()){
        return 0;
    }

    for(i=0;i<va.size();i++){
        if(!compatible(va[i], vb[i])){
            return 0;
        }
    }

    return 1;
}

int compatible(e_logic va, e_logic vb){
    //
    if(va==X){
        return 1;
    }

    if(va==vb){
        return 1;
    }else{
        return 0;
    }
}

void atpgPush(Node* node, e_logic value, e_direction direction){

    ASSIGN* ap = new ASSIGN;
    ap->line = node;
    ap->logic = value;
    ap->direct = direction;
    ap->next = NULL;

    if(assignHead==NULL && assignTail==NULL){
        cout<<"assignment queue is empty for first push"<<endl;
        assignHead = ap;
        assignTail = ap;
    }else{
        assignTail->next = ap;
        assignTail = ap;
    }

    atpgQueue(assignHead);
}

ASSIGN* atpgPop(){
    ASSIGN* temp;
    if(assignHead==NULL && assignTail==NULL){
        cout<<"assignment queue is empty for final pop"<<endl;
        return NULL;
    }

    if(assignHead==assignTail){
        temp=assignHead;
        assignHead = NULL;
        assignTail = NULL;
        return temp;
    }

    temp = assignHead;
    assignHead = assignHead->next;
    return temp;
}

void atpgQueue(ASSIGN* ps){
    while(ps != NULL){
        atpgElement(ps);
        ps = ps->next;
    }
    cout<<"END"<<endl;
}

void atpgElement(ASSIGN* ap){
    if(ap){
        cout<<"Number="<<ap->line->getNumber()<<" Value="<<translate(ap->logic)<<endl;
    }else{
        cout<<"NULL assign pointer can not be printed"<<endl;
    }
}




