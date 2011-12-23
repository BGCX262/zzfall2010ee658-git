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
#include <vector>
#include <map>
#include "Node.h"
#include "Command.h"
#include "global.h"
using namespace std;

Node * NodePointer = NULL;
Node ** PrimaryInput = NULL;
Node ** FanoutBranch = NULL;
Node ** PrimaryOutput = NULL;

Node ** PPIList = NULL;
Node ** PPOList = NULL;
Node ** ReadyList = NULL;

unsigned int NumberNodes = 0;
unsigned int NumberPI = 0;
unsigned int NumberFB = 0;
unsigned int NumberPO = 0;

unsigned int NumberPPI = 0;
unsigned int NumberPPO = 0;
unsigned int Done = 0;

unsigned int NumberLevel = 0;
unsigned int ClockPeriod = 0;

vector<unsigned int> FFInitial;
vector< vector<unsigned int> > InputSequence;

vector<unsigned int> FFInitial_comp;
vector< vector<unsigned int> > InputSequence_comp;

TSTAMP* timeHead = NULL;
TSTAMP* timeTail = NULL;

ASSIGN* assignHead = NULL;
ASSIGN* assignTail = NULL;

map<int, vector<GEVENT> > displayBuffer;
map<int, vector<int> > compareBuffer;

map<int, vector<int> > filteredFault;
vector< vector<e_logic> > testPattern;

e_state GlobalState = EXEC;

//vector<Node*> DFrontier;
//vector<Node*> JFrontier;


