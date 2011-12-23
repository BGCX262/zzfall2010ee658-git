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
 * File:   logicSim.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 6:00 PM
 */

#ifndef LOGICSIM_H
#define	LOGICSIM_H

int simulation();
int comparison();

int logicSim_comp(); //gateway method of logic simulation without delay info
int logicSim_delay(); //gateway method of logic simulation with delay info
int evaluate(Node* np); //evaluate logic output of CLB
int rfDelay(Node* np, int logic); //compute rise or fall delay
int initialSim(int cycle); //initial PI and PPI for logic simulation
int initialSim_comp(int cycle);
int startupSim(int cycle); //timing wheel startup
void queuePush(int time, Node* node, int value); //push an event into the timing wheel
void displayBufferPush(int number, int time, int logic); //push event into waveform data structure
TSTAMP* queuePop(); //pop an event from the timing wheel
TSTAMP* queueFind(int time); //find event in the timing wheel
TSTAMP* queueInsert(int time); //insert event into timing wheel

int fpgaSim_delay();
int fpgaSim_comp();

#endif	/* LOGICSIM_H */

