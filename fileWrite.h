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
 * File:   fileWrite.h
 * Author: zhouzhao
 *
 * Created on November 23, 2010, 9:28 PM
 */

#ifndef FILEWRITE_H
#define	FILEWRITE_H

int printWaveform(); //print waveform file
int printPO_FF(); //print PO and FF status file during each clock
int printGlitch(); //print glitch file
void printDisplayBuffer(); //print waveform data structure in stdout
void printCompareBuffer(); //print compare data structure in stdout
int printDominance(); //print dominance relationship
int printScanNetlist(); //print scan netlist file
int printLevel(); //print level of netlist
int printFilteredFault(); //print fault list after collapsing
int printTestPattern();

#endif	/* FILEWRITE_H */

