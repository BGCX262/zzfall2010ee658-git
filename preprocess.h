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
 * File:   preprocess.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 6:00 PM
 */

#ifndef PREPROCESS_H
#define	PREPROCESS_H

#include "Node.h"

int preprocess();
int connectPointer(Node*, int); //build up fanin and fanout pointers between nodes
int allocate(); //allocate memory
int pc(); //print circuit
int clear(); //deallocate memory
Node* findNode(int); //find node in the node array
int initialPI_PPI(); //initial level of PI and PPI for levelization
int checkPO_PPO(); //check PO and PPO for levelization
int levelComp(Node*); //levelize netlist
int help(); //print help info
int quit(); //quit process
int lev(); //levelize netlist
int removeFF(); //strip off flip flop

#endif	/* PREPROCESS_H */

