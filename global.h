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
 * File:   global.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 3:16 PM
 */
#include "type.h"
#include "Node.h"
using namespace std;

#ifndef GLOBAL_H
#define	GLOBAL_H

extern Node * NodePointer;
extern Node ** PrimaryInput;
extern Node ** FanoutBranch;
extern Node ** PrimaryOutput;

extern Node ** PPIList;
extern Node ** PPOList;
extern Node ** ReadyList;

extern unsigned int NumberNodes;
extern unsigned int NumberPI;
extern unsigned int NumberFB;
extern unsigned int NumberPO;

extern unsigned int NumberPPI;
extern unsigned int NumberPPO;
extern unsigned int Done;

extern unsigned int NumberLevel;
extern unsigned int ClockPeriod;

extern vector<unsigned int> FFInitial;
extern vector< vector<unsigned int> > InputSequence;

extern vector<unsigned int> FFInitial_comp;
extern vector< vector<unsigned int> > InputSequence_comp;

extern TSTAMP* timeHead;
extern TSTAMP* timeTail;

extern ASSIGN* assignHead;
extern ASSIGN* assignTail;

extern map<int, vector<GEVENT> > displayBuffer;
extern map<int, vector<int> > compareBuffer;

extern map<int, vector<int> > filteredFault;
extern vector< vector<e_logic> > testPattern;

extern e_state GlobalState;

//extern vector<Node*> DFrontier;
//extern vector<Node*> JFrontier;

#endif	/* MAIN_H */

