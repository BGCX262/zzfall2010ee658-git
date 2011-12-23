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
 * File:   type.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 5:28 PM
 */

#ifndef MAIN_H
#define	MAIN_H

//macro definition
#define NUMFUNCS 5
#define MAXLINE 81
#define MAXNAME 31
#define FLIPFLOP 256
#define Upcase(x) ((isalpha(x) && islower(x))? toupper(x) : (x))
#define Lowcase(x) ((isalpha(x) && isupper(x))? tolower(x) : (x))

//type definition
enum e_com {READ, PC, HELP, QUIT, LEV};
enum e_ntype {GATE, PI, FB, PO, PPI, PPO};

//gate=0, primary input=1, fanout branch=2, primary output=3, PPI=4, PPO=5

typedef struct logicEvent{
    Node* line;
    int logic;
    struct logicEvent* nextEvent;
} LEVENT;

typedef struct graphEvent{
    int time;
    int logic;
} GEVENT;

typedef struct timeStamp{
    int time;
    struct logicEvent* eventHead;
    struct logicEvent* eventTail;
    struct timeStamp* nextStamp;
} TSTAMP;

typedef struct assignment{

    Node* line;
    e_logic logic;
    e_direction direct;
    struct assignment* next;

} ASSIGN;

#endif	/* MAIN_H */

