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
 * File:   fileRead.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 2:55 PM
 */
using namespace std;

#ifndef FILEREAD_H
#define	FILEREAD_H

int cread(char *charPointer); //read in circuit netlist
int fread(char *charPointer); //read in fault list
int dread(char *charPointer); //read in delay info
int seqread(char *charPointer); //read in input sequence
int tread(char *charPointer); //read in ATPG test logic
int vread(char* charPointer); //read in input vector for comparison
int iread(char *charPointer);
int findFilteredNode(int number);

#endif	/* FILEREAD_H */

