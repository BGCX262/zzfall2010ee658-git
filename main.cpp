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
 * File:   main.cpp
 * Author: zhouzhao
 *
 * Created on October 14, 2010, 9:43 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "Node.h"
#include "Command.h"
#include "fileRead.h"
#include "global.h"
#include "type.h"
#include "logicSim.h"
#include "preprocess.h"
#include "fileWrite.h"
#include "atpg.h"
using namespace std;


int main(){
    
    preprocess();
    simulation();
//    comparison();
//    atpg();
//    pc();
    
    return (EXIT_SUCCESS);
}



