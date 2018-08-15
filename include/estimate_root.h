#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <list>
#include "stdarg.h"
#include <math.h>
#include "utils.h"
#include "dating.h"


using namespace std;

list<double> without_constraint_lambda(double br,Pr* par,Node** nodes,list<int> active_set);

bool starting_point_without_constraint_lambda(double br,Pr* pr,Node** nodes,list<int> &active_set);

bool without_constraint_active_set_lambda(double br,Pr* pr,Node** nodes);

void without_constraint_active_set_lambda_secondTime(double br,Pr* pr,Node** nodes);

list<double> with_constraint_lambda(double br,Pr* pr,Node** nodes,list<int> active_set);


bool with_constraint_active_set_lambda(double br,Pr* pr,Node** nodes);

void with_constraint_active_set_lambda_secondTime(double br,Pr* pr,Node** nodes);

int estimate_root_without_constraint_local_rooted(Pr* pr,Node** nodes);

int estimate_root_without_constraint_rooted(Pr* pr,Node** &nodes);

int estimate_root_with_constraint_local_rooted(Pr* pr,Node** nodes);

int estimate_root_with_constraint_fast_rooted(Pr* pr,Node** nodes);

int estimate_root_with_constraint_rooted(Pr* pr,Node** nodes);

void calculateMultiplier_lambda(int r,int p_r,double br,Pr* pr,Node** nodes,bool* nan);

bool without_constraint_active_set_lambda_multirates(double br,Pr* pr,Node** nodes,bool reassign);

bool with_constraint_active_set_lambda_multirates(double br,Pr* pr,Node** nodes,bool reassign);
