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

/*
 * File:   node.cpp
 * Author: zhouzhao
 *
 * Created on October 14, 2010, 9:43 PM
 */

#include "Node.h"
#include "atpg.h"
#define MIN_BITS 3

Node::Node(){

    //data structure for preprocssing
    index = 0;

    type = 0;
    number = 0;
    gate = 0;
    faultList.clear();
    fanin = 0;
    fanout = 0;

    upperNodes = 0;
    upperCounter = 0;
    downNodes = 0;
    downCounter = 0;

    level = 0;
    nodeReady = 0;
    inputReady = 0;

    //data structure for fault domiance
    domRelation.clear();

    //data structure for logic simulation
    logicValue = 2; //variable is used for logic simulation
    lsv = 2; //last scheduled value
    inertialDelay = 0;
    riseDelay = 0;
    fallDelay = 0;

    //data structure for ATPG
    e_logicValue = X; //variable is used for ATPG
    logicReady = 0;
    for(int i=0;i<8;i++){
        goodCLB[i]=0;
        badCLB[i]=0;
        dCLB[i]=0;
        dbarCLB[i]=0;
    }

    table.clear(); //first table for logic minimization
    p_group.clear(); //second table
    final_group.clear(); //third table
    printed_numbers.clear(); //internal data structure
    stored_numbers.clear(); //internal data structure

    outputOne.clear(); //primitive one cube
    outputZero.clear(); //primitive zero cube

    detectD.clear(); //primitive cube to detect D
    detectDbar.clear(); //primitive cube to detect Dbar

    dcube.clear(); //internal data structure
    ddrive.clear(); //primitive cube to drive D
        
}

unsigned int Node::getIndex(){
        return this->index;
}

void Node::setIndex(unsigned int index){
        this->index = index;
}

unsigned int Node::getType(){
        return this->type;
}

void Node::setType(unsigned int type){
        this->type = type;
}

unsigned int Node::getNumber(){
        return this->number;
}

void Node::setNumber(unsigned int number){
        this->number = number;
}

unsigned int Node::getGate(){
    return this->gate;
}

void Node::setGate(unsigned int gate){
    this->gate = gate;
}

unsigned int Node::getFaultList(int index){
    return this->faultList[index];
}

unsigned int Node::getFaultListCount(){
    return this->faultList.size();
}

void Node::setFaultList(unsigned int fault){
    int i;
    for(i=0;i<faultList.size();i++){
        if(faultList[i]==fault){ //reduce fault redundancy
            return;
        }
    }
    faultList.push_back(fault);
}

unsigned int Node::getFanin(){
        return this->fanin;
}

void Node::setFanin(unsigned int fanin){
        this->fanin = fanin;
}

unsigned int Node::getFanout(){
        return this->fanout;
}

void Node::setFanout(unsigned int fanout){
        this->fanout = fanout;
}

Node ** Node::getUpperNodes(){
        return this->upperNodes;
}

void Node::setUpperNodes(Node ** upperNodes){
        this->upperNodes = upperNodes;
}

int Node::getUpperCounter(){
    return this->upperCounter;
}

void Node::setUpperCounter(int upperCounter){
    this->upperCounter = upperCounter;
}

Node ** Node::getDownNodes(){
        return this->downNodes;
}

void Node::setDownNodes(Node ** downNodes){
        this->downNodes = downNodes;
}

int Node::getDownCounter(){
    return this->downCounter;
}

void Node::setDownCounter(int downCounter){
    this->downCounter = downCounter;
}

unsigned int Node::getLevel(){
        return this->level;
}

void Node::setLevel(unsigned int level){
        this->level = level;
}

int Node::getNodeReady(){
        return this->nodeReady;
}

void Node::setNodeReady(int nodeReady){
        this->nodeReady = nodeReady;
}

int Node::getInputReady(){
        return this->inputReady;
}

void Node::setInputReady(int inputReady){
        this->inputReady = inputReady;
}

int Node::getLogicValue(){
    return this->logicValue;
}

int Node::getLogicOutput(int index){
    return this->goodCLB[index];
}

void Node::setLogicValue(int logicValue){
    this->logicValue = logicValue;
}

int Node::getLSV(){
    return this->lsv;
}

void Node::setLSV(int value){
    this->lsv = value;
}

unsigned int Node::getInertialDelay(){
    return this->inertialDelay;
}

void Node::setInertialDelay(unsigned int inertialDelay){
        this->inertialDelay = inertialDelay;
}

unsigned int Node::getRiseDelay(){
        return this->riseDelay;
}

void Node::setRiseDelay(unsigned int riseDelay){
        this->riseDelay = riseDelay;
}

unsigned int Node::getFallDelay(){
        return this->fallDelay;
}

void Node::setFallDelay(unsigned int fallDelay){
        this->fallDelay = fallDelay;
}

e_logic Node::getLogicValue_E(){
    return this->e_logicValue;
}

void Node::setLogicValue_E(e_logic value){
    this->e_logicValue = value;
}

//create D cube array
void Node::create_d_array(){
    int i;
    for(i=0;i<8;i++){
        dCLB[i]=0;
        dbarCLB[i]=0;
        if(goodCLB[i] != badCLB[i]){
            if(goodCLB[i]==1){
                dCLB[i]=1;
                dbarCLB[i]=0;
            }else{
                dCLB[i]=0;
                dbarCLB[i]=1;
            }
        }
    }
/*
    for(i=0;i<8;i++){
        cout<<dCLB[i]<<" ";
    }
    cout<<endl;

    for(i=0;i<8;i++){
        cout<<dbarCLB[i]<<" ";
    }
    cout<<endl;
 */
}

//gateway method to process primitive cube for each node
int Node::pCubeProcess(){
    cout<<"Node: "<<this->number<<endl;

    decimal2binary(gate,goodCLB, 8); //generate binary form of CLB number
    cout<<"One cube:"<<endl;
    process(goodCLB,1,&outputOne); //process primitive one cube
    cout<<"Zero cube:"<<endl;
    process(goodCLB,0,&outputZero); //process primitive zero cube
    
    return 0;
}

//gateway method to process primitve cube to detect a fault Note that
int Node::fCubeProcess(int index){

    cout<<"Node: "<<this->number<<endl;
    if(faultList.size()>0){
        decimal2binary(faultList[index], badCLB, 8);
        create_d_array();
        cout<<"D cube"<<endl;
        cout<<"----------------"<<endl;
        cout<<"1 2 3 | fault"<<endl;
        cout<<"----------------"<<endl;
        process(dCLB,1,&detectD); //process primitive cube to detect D at output
        cout<<"Dbar cube"<<endl;
        process(dbarCLB,1,&detectDbar); //process primitive cube to detect Dbar at output
    }

    return 0;
}

int Node::findFaultNum(unsigned int fault){
    int i;
    for(i=0;i<faultList.size();i++){
        if(faultList[i]==fault){
            return i;
        }
    }
    cout<<"fault "<<fault<<" can not be found."<<endl;
}

//main process
void Node::process(unsigned int* clb, unsigned int flag, vector<vector<e_logic> >* tab){

    table.clear();
    table.resize(1);

    p_group.clear();
    p_group.resize(1);

    final_group.clear();
    final_group.resize(1);

    printed_numbers.clear();
    stored_numbers.clear();

    create_table(clb,flag);
//    print_table();

    create_p_group();
//    print_p_group();

    create_final_group();
    print_final_group();

    create_p_table(tab);
    print_p_table(tab);
}

//count the number of 1 in a number
/* counts 1s by getting the LSB (%2) and then shifting until 0 */
unsigned Node::count_1s(unsigned number){
	short bit =0;
	int count = 0;
	while(number>0)	{
		bit = number%2;
		number>>=1;
		if(bit) {
			count++;
		}
	}
	return count;
}

//print a number in binary format
/*get LSB, arrange it in array, the print array in reverse order so MSB is on
the left */
void Node::print_binary(unsigned number){
	unsigned bits[MIN_BITS];
	int count = 0;

	while(number>0||count<MIN_BITS)	{
		bits[count] = number%2;
		number>>= 1;
		count++;
	}
	for(int i=count-1;i>=0;i--)
		cout<<bits[i];
}

//creat table based on the number of 1 in minterm
/*creating first table: append current number to the array located in
table[number of 1s f this number]*/
void Node::create_table(unsigned int* clb, unsigned int flag){
    int i;
    short tmp;
    B_number temp_num;

    for(i=0;i<8;i++) {
        if(clb[i] == flag){
            tmp = count_1s(i);

            if(tmp+1>table.size())
                table.resize(tmp+1);

            temp_num = init_B_number(i,0,false);
            table[tmp].push_back(temp_num);
	}
    }

//    cout<<"table is created successfully."<<endl;
}

//print the first table
void Node::print_table(){

	cout<<endl<<"COMPUTING:"<<endl;
	for(int i=0;i<table.size();i++)	{
		cout<<i;
		for(int j=0;j<table[i].size();j++) {
			cout<<"\tm"<<table[i][j].number<<"\t";
			print_binary(table[i][j].number);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}
}

//constructor of struct B_number
/* initialize a B_number variable - like a constructor */
B_number Node::init_B_number(unsigned n,int d, bool u){
	struct B_number num;
	num.number = n;
	num.dashes = d;
	num.used = u;
	return num;
}

//creat p_group in the middle process
/*like the original table, but the paring of numbers from the original table-
dashes are represented by a 1. for example original A=0011 B=1011, new number
is -011 which is represented as C.number=A&B=0011,C.dashes=A^B=1000*/
void Node::create_p_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<table.size()-1;i++) {
		for(int j=0;j<table[i].size();j++) {
			for(int k=0;k<table[i+1].size();k++) {
				temp_number = table[i][j].number & table[i+1][k].number;
				temp_dashes = table[i][j].number ^ table[i+1][k].number;

				if(count_1s(temp_dashes)==1) {
					table[i][j].used = true;
					table[i+1][k].used = true;


					tmp = count_1s(temp_number);

					if(tmp+1>p_group.size())
						p_group.resize(tmp+1);

					temp_num = init_B_number(temp_number, temp_dashes, false);
					p_group[tmp].push_back(temp_num);
				}
			}
		}
	}

//        cout<<"primitive group is created successfully."<<endl;
}

//print the table of p_group
void Node::print_p_group() {
	cout<<endl<<"MID PROCESS COMPUTATION:"<<endl;

	for(int i=0;i<p_group.size();i++) {
		cout<<i;
		for(int j=0;j<p_group[i].size();j++) {
			cout<<"\t\t";
			print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}

}
/*print a number such as -001; this allocates bits in an array dash=2 then
prints reverse array */
void Node::print_p_binary(unsigned n, unsigned d) {
	unsigned bits[MIN_BITS];
	int count = 0;

	while(n>0||count<MIN_BITS) {
		if(!(d%2))
			bits[count] = n%2;
		else
			bits[count] = 2;

		n >>= 1;
		d >>= 1;
		count++;
	}

	for(int i=count-1;i>=0;i--) {
		if(bits[i]!=2)
			cout<<bits[i];
		else
			cout<<"-";
	}
}

//create primitive table
void Node::create_p_table(vector< vector<e_logic> >* table){

    unsigned bits[MIN_BITS];
    int count;
    int i,j;

    unsigned n;
    unsigned d;

    table->resize(stored_numbers.size());

    for(i=0;i<stored_numbers.size();i++){

        n = stored_numbers[i].number;
        d = stored_numbers[i].dashes;
        count=0;

        while(n>0 || count<MIN_BITS){

            if(!(d%2)){
                bits[count] = n%2;
            }else{
                bits[count] = 2;
            }

            n >>= 1;
            d >>= 1;
            count++;
        }

        for(j=MIN_BITS-1;j>=0;j--){
            switch (bits[j]){
                case 0:
                    (*table)[i].push_back(ZERO);
                    break;
                case 1:
                    (*table)[i].push_back(ONE);
                    break;
                case 2:
                    (*table)[i].push_back(X);
                    break;
                default:
                    printf("");
            }
        }
    }

//    cout<<"table of primitive cube is created successfully."<<endl;
}

//print primitive table
void Node::print_p_table(vector< vector<e_logic> >* table){
    int i,j;

    for(i=0;i<table->size();i++){
        for(j=0;j<(*table)[i].size();j++){
            cout<<translate((*table)[i][j])<<" ";
        }
        cout<<endl;
    }
}

/*creates final table. works like p_group(). example; in p_group you have:
A=-001 B=-011 -> C= -0-1 which will be represented as
C.number=A&B=0001&0011=0001, and C.dashes=A^B^A.dashes=0001^0011^1000=1010.
Computation is done only when A.dashes = b.dashes*/
void Node::create_final_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<p_group.size()-1;i++) {
		for(int j=0;j<p_group[i].size();j++) {
			for(int k=0;k<p_group[i+1].size();k++) {
				if(p_group[i][j].dashes == p_group[i+1][k].dashes) {
					temp_number = p_group[i][j].number & p_group[i+1][k].number;
					temp_dashes = p_group[i][j].number ^ p_group[i+1][k].number;
					if(count_1s(temp_dashes)==1) {
						temp_dashes^= p_group[i][j].dashes;

						p_group[i][j].used = true;
						p_group[i+1][k].used = true;

						tmp = count_1s(temp_number);

						if(tmp+1>final_group.size())
							final_group.resize(tmp+1);

						temp_num = init_B_number(temp_number, temp_dashes, true);
						final_group[tmp].push_back(temp_num);
					}
				}
			}
		}
	}
//        cout<<"final group is created successfully."<<endl;
}

/*print all the values from the final table, except for duplicates.
  print all the unused numbers from original table and mid process table*/
void Node::print_final_group() {
//	cout<<endl<<"FINAL:\n-------------------------------------"<<endl;
	int i,j;
	for(i=0;i<final_group.size();i++) {
		for(j=0;j<final_group[i].size();j++) {
			if(!is_printed(final_group[i][j])) {
				print_p_binary(final_group[i][j].number,final_group[i][j].dashes);
				cout<<endl;
				printed_numbers.push_back(final_group[i][j]);
                                stored_numbers.push_back(final_group[i][j]);
			}
		}
	}
	for(i=0;i<p_group.size();i++) {
		for(j=0;j<p_group[i].size();j++) {
			if(!p_group[i][j].used) {
				print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
				cout<<endl;
                                stored_numbers.push_back(p_group[i][j]);
			}
		}
	}
	for(i=0;i<table.size();i++) {
		for(j=0;j<table[i].size();j++) {
			if(!table[i][j].used) {
				print_p_binary(table[i][j].number,table[i][j].dashes);
				cout<<endl;
                                stored_numbers.push_back(table[i][j]);
			}
		}
	}
//	cout<<"-------------------------------------"<<endl;
}
/*used to avoid printing duplicates that can exist in the final table*/
bool Node::is_printed(B_number n) {
	for(int i=0;i<printed_numbers.size();i++)
		if(n.number==printed_numbers[i].number && n.dashes == printed_numbers[i].dashes)
			return true;

	return false;
}

/*return min number of bits a number is represented by. used for best output*/
unsigned Node::count_bits(unsigned n) {
	short bit =0;
	int count = 0;
	while(n>0) {
		bit = n%2;
		n>>=1;
		count++;
	}
	return count;
}

//------------------------------------------------------------------------------
//create D cube table
void Node::create_d_table(){
    int i,j;
    unsigned int n;
    unsigned int d;
    unsigned int tempA, tempB;
    int count;
    unsigned int bits[MIN_BITS];

    ddrive.resize(dcube.size());
    for(i=0;i<dcube.size();i++){
        tempA = dcube[i].numberA;
        tempB = dcube[i].numberB;
        n = tempA & tempB;
        d = tempA ^ tempB;
        count = 0;

        while(n>0 || count<MIN_BITS){

            if(!(d%2)){
                bits[count] = n%2;
            }else{
                if(tempA%2){
                    bits[count] = 2;
                }else{
                    bits[count] = 3;
                }
            }

            n >>= 1;
            d >>= 1;
            tempA >>= 1;
            tempB >>= 1;
            count++;
        }

        for(j=MIN_BITS-1;j>=0;j--){
            switch (bits[j]){
                case 0:
                    ddrive[i].push_back(ZERO);
                    break;
                case 1:
                    ddrive[i].push_back(ONE);
                    break;
                case 2:
                    ddrive[i].push_back(D);
                    break;
                case 3:
                    ddrive[i].push_back(DBAR);
                default:
                    printf("");
            }
        }

        ddrive[i].push_back(dcube[i].output);
    }
}

//print D cube table
void Node::print_d_table(){
    int i,j;

    cout<<"---------------"<<endl;
    cout<<"1 2 3 | output"<<endl;
    cout<<"---------------"<<endl;
    for(i=0;i<ddrive.size();i++){
        for(j=0;j<ddrive[i].size();j++){
            cout<<translate(ddrive[i][j])<<" ";
        }
        cout<<endl;
    }
    cout<<"---------------"<<endl;
}

//drive D or Dbar through CLB
//digit is input bit of CLB 0=LSB 2=MSB
void Node::d_drive(int digit, e_logic fault){

    unsigned int bitsA[MIN_BITS];
    unsigned int bitsB[MIN_BITS];
    unsigned int buffer[MIN_BITS-1];
    int i;

    unsigned int valueA, valueB;
    D_number temp;
    e_logic temp_logic;

    switch (fault){
        case D:
            bitsA[digit]=1;
            bitsB[digit]=0;
            break;
        case DBAR:
            bitsA[digit]=0;
            bitsB[digit]=1;
            break;
        default:
            cout<<""<<endl;
    }

    for(i=0;i<4;i++){
        decimal2binary(i,buffer,MIN_BITS);
        switch (digit){
            case 0:
                bitsA[1]=buffer[0];
                bitsB[1]=buffer[0];
                bitsA[2]=buffer[1];
                bitsB[2]=buffer[1];

                valueA = binary2decimal(bitsA,MIN_BITS);
                valueB = binary2decimal(bitsB,MIN_BITS);
                if(goodCLB[valueA] != goodCLB[valueB]){
                    if(goodCLB[valueA] == 1){
                        temp_logic = D;
                    }else{
                        temp_logic = DBAR;
                    }
                    temp = init_D_number(valueA,valueB,temp_logic);
                    dcube.push_back(temp);
                }
                break;
            case 1:
                bitsA[0]=buffer[0];
                bitsB[0]=buffer[0];
                bitsA[2]=buffer[1];
                bitsB[2]=buffer[1];

                valueA = binary2decimal(bitsA,MIN_BITS);
                valueB = binary2decimal(bitsB,MIN_BITS);
                if(goodCLB[valueA] != goodCLB[valueB]){
                    if(goodCLB[valueA] == 1){
                        temp_logic = D;
                    }else{
                        temp_logic = DBAR;
                    }
                    temp = init_D_number(valueA,valueB,temp_logic);
                    dcube.push_back(temp);
                }
                break;
            case 2:
                bitsA[0]=buffer[0];
                bitsB[0]=buffer[0];
                bitsA[1]=buffer[1];
                bitsB[1]=buffer[1];

                valueA = binary2decimal(bitsA,MIN_BITS);
                valueB = binary2decimal(bitsB,MIN_BITS);
                if(goodCLB[valueA] != goodCLB[valueB]){
                    if(goodCLB[valueA] == 1){
                        temp_logic = D;
                    }else{
                        temp_logic = DBAR;
                    }
                    temp = init_D_number(valueA,valueB,temp_logic);
                    dcube.push_back(temp);
                }
                break;
            default:
                cout<<""<<endl;
        }
    }
/*
    for(i=0;i<dcube.size();i++){
        cout<<dcube[i].numberA<<" "<<dcube[i].numberB<<" "<<dcube[i].output<<endl;
    }
 */
}

//constructor for D cube struct
D_number Node::init_D_number(unsigned int na, unsigned int nb, e_logic out){
    struct D_number num;
    num.numberA = na;
    num.numberB = nb;
    num.output = out;

    return num;
}

//convert binary index into decimal index
unsigned int Node::binary2decimal(unsigned int* bits, int digit){
    int i;
    unsigned int result = 0;
    unsigned int weight = 1;

    for(i=0;i<digit;i++){

        result += bits[i]*weight;
        weight *=2;
    }

    return result;
}

//convert decimal index into binary index
void Node::decimal2binary(unsigned int number, unsigned int* bits, int digit) {

	int count = 0;
	while(number>0||count<digit)	{
		bits[count] = number%2;
		number>>= 1;
		count++;
	}
}

//gateway methods for process D cube
int Node::dCubeProcess(int digit, e_logic fault){

    d_drive(digit, fault);
    create_d_table();
    print_d_table();
    return 0;
}

//private methods called by dominace process
void Node::create_dom_table(){
        int i,j;
        unsigned int fault;

        //build up data structure of dominance
        for(i=0;i<faultList.size();i++){
            fault=faultList[i];
            vector<F_number*> temp;

            FSTRUCT* fp = new FSTRUCT;
            fp->fault=fault;
            fp->domPointer = temp;
            domRelation.push_back(*fp);
        }

        for(i=0;i<faultList.size();i++){
            fault = faultList[i];
            for(j=0;j<faultList.size();j++){
                if(faultList[j]!=fault){
                    if(findDominance(fault,faultList[j])){
                        findFault(fault)->domPointer.push_back(findFault(faultList[j]));
                    }
                }
            }
        }
}

//find a fault in data structure of dominance tree
F_number* Node::findFault(unsigned int fault){
        int i;
        for(i=0;i<domRelation.size();i++){
            if(domRelation[i].fault == fault){
                return &domRelation[i];
            }
        }
        return NULL;
}

//find dominance relationship betweeen two faults
int Node::findDominance(int fa, int fb){
        unsigned int detecta[8];
        unsigned int detectb[8];
        int i;

        decimal2binary(gate^fa,detecta,8);
        decimal2binary(gate^fb,detectb,8);

        for(i=0;i<8;i++){
            if(detectb[i]==1 && detecta[i]!=1){
                return 0;
            }
        }
        return 1;
}

//print data structure of dominance relationship
void Node::print_dom_table(){
        int i,j;
        vector<F_number*> temp;
        for(i=0;i<domRelation.size();i++){
            if(domRelation[i].domPointer.size()!=0){
                cout<<"fault "<<domRelation[i].fault<<" dom ";
                temp = domRelation[i].domPointer;
                for(j=0;j<temp.size();j++){
                    cout<<temp[j]->fault<<" ";
                }
            cout<<endl;
            }
        }
}

F_number* Node::getDomRelation(int index){
    return &domRelation[index];
}

//atpg--------------------------------------------------------------------------

int Node::searchOutputOne(vector<e_logic> pattern){
    int i;
    for(i=0;i<outputOne.size();i++){
        if(outputOne[i] == pattern || compatible_V(outputOne[i], pattern)){
            return 1;
        }
    }
    return 0;
}

int Node::searchOutputZero(vector<e_logic> pattern){
    int i;
    for(i=0;i<outputZero.size();i++){
        if(outputZero[i] == pattern || compatible_V(outputZero[i], pattern)){
            return 1;
        }
    }
    return 0;
}

vector< vector<e_logic> > Node::getOutputOne(){
    return this->outputOne;
}

vector< vector<e_logic> > Node::getOutputZero(){
    return this->outputZero;
}

vector< vector<e_logic> > Node::getDetectD(){
    return this->detectD;
}

vector< vector<e_logic> > Node::getDectectDbar(){
    return this->detectDbar;
}

vector< vector<e_logic> > Node::getDDrive(){
    return this->ddrive;
}

//translate enum type into string
string translate(unsigned int value){
    switch (value){
        case 0: return ("0");
        case 1: return ("Dbar");
        case 2: return ("D");
        case 3: return ("1");
        case 4: return ("x");
        default:
            printf(" ");
    }
}

int Node::getLogicReady(){
    return this->logicReady;
}

void Node::setLogicReady(int value){
    this->logicReady = value;
}

void Node::nodeClear(){

    int i;
    this->e_logicValue = X;
    this->logicReady = 0;

        this->table.clear();
    this->p_group.clear();
    this->final_group.clear();
    this->printed_numbers.clear();
    this->stored_numbers.clear();

    this->outputOne.clear();
    this->outputZero.clear();
    this->detectD.clear();
    this->detectDbar.clear();

    this->dcube.clear();
    this->ddrive.clear();
    
}



