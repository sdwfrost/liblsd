#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <list>
#include <cstdlib>
#include <string.h>
#include "stdarg.h"
#include <math.h>
#ifndef maxIter
#define maxIter 1000
#endif
#ifndef toCollapse
#define toCollapse 0
#endif
#include "node.h"
#include "pr.h"
#include "pair.h"
#include "subtree.h"
#include "part.h"

using namespace std;
  
string readWord(FILE *f,string fn);

string readWord(string line,int& pos);

char readChar(FILE *f,string fn);

double readdouble(FILE *f,string fn);

double readdouble(FILE *f,char c);

double readdouble(FILE *f,int c);

int readInt(FILE *f,string msg);

int getLineNumber(string fn);

string readSupport(FILE *f,string fn);

void concat(list<int> & l1,list<int> l2);

void concatPos(list<int> l1,list<int> &l2);

void concat(stack<int> & l1,list<int> l2);

void concat(list<int> & l1,stack<int> l2);

int getPosition(Node**,string s,int n,int m);

list<int> intersect(list<int> l1,list<int> l2);

double myabs(double a);

bool isAncestor(Node**,int i,int j);

int lca(list<int> a, list<int> b);

int mrca(Node**,int i,int j);

int index(list<int> & L, int e);

int index(string s,string* & L,int n);

bool contain(int s,vector<int> l);

bool contain(int s,list<int> l);

bool contain(string s,list<string> l);

string readLabel(FILE *f,FILE *w);

string readLabel(char ch,FILE *f,int& a);


char readBracket(FILE *f,string fn);

char readCommaBracket(FILE *f,string fn,string& s);

char read2P(FILE *f,string fn);

void computeSuc(int* & Pre,int* & Suc1,int* & Suc2,int size,int n);

void computeVariance(Pr* pr,Node** nodes);

void computeNewVariance(Pr* pr,Node** nodes);

void myExit( string msg, ... );


void myErrorMsg( string msg, ... );

bool isReal( const char* str );

bool isInteger( const char* str );

void sort(int* & tab,int size);
void sort(double* & tab,int size);
int index(int* & tab,int value,int size);

int mrca(Node**,list<int> taxa);

int mrca(Node**,vector<int> taxa);

bool markLeaf(Node* no);

bool leaf(Node* no);

bool tc(Node* no);

bool limit(Node* no);

bool tc_limit(Node* no);

bool lower(Node* no);

bool upper(Node* no);

void activeMarkLeaf(Node* no);

void desactiveMarkLeaf(Node* no);

void activeTC(Node* no);

void activeUpper(Node* no);

void activeLower(Node* no);

void desactive(Node* no);

void desactiveTC(Node* no);

void desactiveLimit(Node* no);

list<int> down_leaf(int i,Pr* pr,Node** nodes);

stack<int>* computeFeuilles(list<int> ls,Pr* pr,Node** nodes);

list<int> suc(int i,int j,Pr* pr,Node** nodes,int* & Pre,list<int> &sucL,list<int> &sucI);

bool reroot_rootedtree(double& br,int r,int s10,int s20,Pr* pr,Node** nodes,Node** nodes_new);

int reroot_rootedtree(int r,Pr* pr,Node** nodes,Node** nodes_new);

bool reroot_rootedtree(double& br,int r,int s10,int s20,Pr* pr,Node** nodes,Node** nodes_new,int* & P_ref,int* & tab);

Node** unrooted2rooted(Pr* pr,Node** nodes,int s);

Node** unrooted2rootedS(Pr* pr,Node** nodes,int s);

void computeObjective(Pr* pr,Node** nodes);

void computeObjectiveMultiRates(Pr* pr,Node** nodes);

void computeObjectiveMultiRates(Pr* pr,Node** nodes,double* B, double* V);

void computeObjectiveEstimateRoot(int r,int p_r,double br,Pr* pr,Node** nodes);

string newick(int i,int n,int* & P,int* & Suc1,int* & Suc2,string* & Labels,double* & B);

string newick(int i,int terminate,Pr* pr,Node** nodes);

string newickDate(int i,Pr* pr,Node** nodes);

string nexus(int i,Pr* pr,Node** nodes);

string nexusIC(int i,Pr* pr,Node** nodes,double* T_min,double* T_max);

double* variance(int w,int m,double* B,int c,int s);

double variance(Pr* pr,double b);

double* newvariance(int m,double rho,int* P,double* T,int c,int s);

list<string> getOutgroup(string fn);

bool initConstraintReRooted(Pr* pr,Node** nodes);

bool initConstraint(Pr* pr,Node** nodes);

void initialize_status(Pr* &pr,Node** &nodes);

list<int> getActiveSet(Pr* pr,Node** nodes);

Node** cloneLeaves(Pr* pr,Node** nodes,int f);

void cloneInternalNodes(Pr* pr,Node** nodes,Node** nodes_new,int f);

void shiftInternalConstraints(Pr* pr);

void computeSuc_polytomy(Pr* pr,Node** nodes);

list<int> pos_polytomy(int i,Pr* pr,Node** nodes);

list<int> postorder_polytomy(Pr* pr,Node** nodes);

list<int> pre_polytomy(int i,Pr* pr,Node** nodes);

list<int> preorder_polytomy(Pr* pr,Node** nodes);

stack<int>* computeFeuilles_polytomy(list<int> ls,Pr* pr,Node** nodes);

list<int> down_polytomy(int,int,int* &,list<int>* &,list<int>* &,bool* &,int* &,double* &);

void reduceTree_polytomy(Pr* pr,Node** nodes,int* &Pre,list<int>* & Suc,list<int>* &internal);

list<int> suc_polytomy(int i,int j,Pr* pr,Node** nodes,int* & Pre,list<int> &suc);

void computeVarianceEstimateRoot(Pr* pr,Node** nodes,double br);

void computeNewVarianceEstimateRoot(Pr* pr,Node** nodes);

void checkRooted(Pr* opt);

int getInternalNodeId(Pr* pr,Node** nodes,string s);

int firstCharacter(string s,int p);

int lastCharacter(string s,int p);

int assignRecursive(int r,Node** nodes,int g);

int assignRateGroupToSubTree(Subtree* subtree,Pr* pr,Node** nodes,int g);

void assignRateGroupToTree(Pr* pr,Node** nodes);


double* sortTab(double* tab,int size);

