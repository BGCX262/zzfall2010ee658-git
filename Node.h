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
 * File:   node.h
 * Author: zhouzhao
 *
 * Created on October 15, 2010, 11:30 AM
 */

#ifndef NODE_H
#define	NODE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum e_logic {ZERO=0,DBAR=1,D=2,ONE=3,X=4}; //enum logic for ATPG
enum e_direction{BACKWARD, FORWARD}; //propagate direction
string translate(unsigned int value);

//B:=boolean
struct B_number{
    unsigned number;
    unsigned dashes;
    bool used;
};

//D:=D cube
struct D_number{
    unsigned int numberA;
    unsigned int numberB;
    e_logic output;
};

//F:= fault
typedef struct F_number{
    unsigned int fault;
    vector< struct F_number* > domPointer;
} FSTRUCT;

class Node {
private:
    //data struture for preprocessing
    unsigned int index;

    unsigned int type;
    unsigned int number;
    unsigned int gate;
    vector<unsigned int> faultList;
    
    unsigned int fanout;
    unsigned int fanin;

    Node ** upperNodes;
    int upperCounter;

    Node ** downNodes;
    int downCounter;

    unsigned int level;
    int nodeReady;
    int inputReady;

    //data structure for fault dominance
    vector<F_number> domRelation;

    //data structure for logic simulation
    int logicValue;
    int lsv; //lsv:=last scheduled value
    unsigned int inertialDelay;
    unsigned int riseDelay;
    unsigned int fallDelay;

    //data structure for ATPG
    e_logic e_logicValue;
    int logicReady;
    unsigned int goodCLB[8];
    unsigned int badCLB[8];
    unsigned int dCLB[8];
    unsigned int dbarCLB[8];

    vector< vector<B_number> > table; //first table for logic minimization
    vector< vector<B_number> > p_group; //second table
    vector< vector<B_number> > final_group; //third table
    vector<B_number> printed_numbers; //internal data structure
    vector<B_number> stored_numbers; //internal data structure

    vector< vector<e_logic> > outputOne; //primitive one cube
    vector< vector<e_logic> > outputZero; //primitive zero cube

    vector< vector<e_logic> > detectD; //primitive cube to detect D
    vector< vector<e_logic> > detectDbar; //primitive cube to detect Dbar
 
    vector<D_number> dcube; //internal data structure
    vector< vector<e_logic> > ddrive; //primitive cube to drive D

    //private methods called by primitve cube process
    unsigned count_1s(unsigned number); //count the number of 1s in a number
    void print_binary(unsigned number); //print the number in binary

    void create_table(unsigned int*, unsigned int); //create original table sorted by the number of 1s
    void print_table(); //print the table

    B_number init_B_number(unsigned n,int d, bool u); //initialize a B_number

    void create_p_group(); //create mid process table
    void print_p_group(); //print it

    void print_p_binary(unsigned n, unsigned d); //print the mid table (with -'s)
    void create_final_group(); //create final table
    void print_final_group(); //print final table with -'s and unused terms

    bool is_printed(B_number n); //dont print terms that were already printed
    void getinput(); //get input from user
    unsigned count_bits(unsigned n); //min bits to represent a number

    void create_p_table(vector< vector<e_logic> >*); //create primitive cube table
    void print_p_table(vector< vector<e_logic> >*); //print primitive cube table

    void create_d_array(); //create D cube array
    void process(unsigned int* clb, unsigned int flag, vector< vector<e_logic> >* table); //internal process

    //private methods called by D cube process
    void decimal2binary(unsigned int number, unsigned int* bits, int digit); //convert decimal to binary
    unsigned int binary2decimal(unsigned int* bits, int digit); //convert binary to decimal
    D_number init_D_number(unsigned int na, unsigned int nb, e_logic out); //constructor for D cube
    void d_drive(int digit, e_logic fault); //drive D through CLB
    void print_d_table(); //print D cube table
    
    void create_d_table(); //create D cube table

    //private methods to find dominance relationship
    void create_dom_table(); //create tree structure of dominance
    F_number* findFault(unsigned int fault); //find fault in the tree
    int findDominance(int fa, int fb); //find dominance relationship between two faults

public:
    void nodeClear();
    
    Node();
    int pCubeProcess();
    int fCubeProcess(int faultNum);
    int findFaultNum(unsigned int fault);
    int dCubeProcess(int digit, e_logic fault);
    int domProcess(){
        create_dom_table();
        return 1;
    }
    void print_dom_table();
    F_number* getDomRelation(int index);

    unsigned int getIndex();
    void setIndex(unsigned int);

    unsigned int getType();
    void setType(unsigned int);

    unsigned int getNumber();
    void setNumber(unsigned int);

    unsigned int getGate();
    void setGate(unsigned int);

    unsigned int getFaultList(int index);
    unsigned int getFaultListCount();
    void setFaultList(unsigned int fault);

    unsigned int getFanin();
    void setFanin(unsigned int);

    unsigned int getFanout();
    void setFanout(unsigned int);

    Node ** getUpperNodes();
    void setUpperNodes(Node **);
    int getUpperCounter();
    void setUpperCounter(int);

    Node ** getDownNodes();
    void setDownNodes(Node **);
    int getDownCounter();
    void setDownCounter(int);

    unsigned int getLevel();
    void setLevel(unsigned int);

    int getNodeReady();
    void setNodeReady(int);

    int getInputReady();
    void setInputReady(int);

    int getLogicValue();
    int getLogicOutput(int index);
    void setLogicValue(int);

    int getLSV();
    void setLSV(int value);

    unsigned int getInertialDelay();
    void setInertialDelay(unsigned int);

    unsigned int getRiseDelay();
    void setRiseDelay(unsigned int);

    unsigned int getFallDelay();
    void setFallDelay(unsigned int);

    e_logic getLogicValue_E();
    void setLogicValue_E(e_logic value);

    int getLogicReady();
    void setLogicReady(int value);

    int searchOutputOne(vector<e_logic> pattern);
    int searchOutputZero(vector<e_logic> pattern);
    vector< vector<e_logic> > getOutputOne();
    vector< vector<e_logic> > getOutputZero();

    vector< vector<e_logic> > getDetectD();
    vector< vector<e_logic> > getDectectDbar();

    vector< vector<e_logic> > getDDrive();
};

#endif	/* NODE_H */

