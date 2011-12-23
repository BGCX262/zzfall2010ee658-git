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
 * File:   command.cpp
 * Author: zhouzhao
 * 
 * Created on October 15, 2010, 11:21 AM
 */

#include "Command.h"

Command::Command() {
}

Command::Command(const Command& orig) {
}

Command::~Command() {
}


char * Command::getName(){

    return this->name;

}

e_state Command::getState(){

    return this->state;
}



