/* 
 * File:   atpg.h
 * Author: zhouzhao
 *
 * Created on November 25, 2010, 5:32 PM
 */

#ifndef ATPG_H
#define	ATPG_H

#include "type.h"
#include "Node.h"

int implyCheck(Node* np);
int implyProcess(int num, e_logic logic, e_direction direction);
int propagate(Node* node, e_logic value, e_direction direction);
e_logic atpgEvaluate(Node* node);
void atpgPush(Node* node, e_logic value, e_direction direction);
int compatible_V(vector<e_logic> va, vector<e_logic> vb);
int compatible(e_logic va, e_logic vb);
ASSIGN* atpgPop();
void atpgQueue(ASSIGN* ps);
void atpgElement(ASSIGN* ap);

vector<Node*> findDFrontier();
int findD_Dbar(Node* np);
//void addDFrontier(Node* np);
int D_alg(Node* np);
vector<Node*> findJFrontier();
//void addJFrontier(Node* np);
int startupATPG(int number, int fault);
int inputAllSpecified();
int initialATPG();
int storeTestPattern();
int atpg();

#endif	/* ATPG_H */

