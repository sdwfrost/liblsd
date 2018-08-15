//    LSD - Least Square Dating for etimating substitution rate and divergence dates
//    Copyright (C) <2015> <Thu-Hien To>

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "utils.h"

string readWord(FILE *f,string fn){
    string s;
    char c=readChar(f,fn);
    int i=0;
    while (c<33 || c>126) c=readChar(f,fn);
    while (c>=33 && c<=126 && c!='(' && c!=')' && c!=','){
        s=s+c;
        i++;
        c=getc(f);
    }
    return s;
}

string readWord(string line,int& pos){
    string s;
    char c=line.at(pos);
    while (pos < line.length() && (c<33 || c>126)){ c=line.at(pos);pos++;}
    while (pos < line.length() && c>=33 && c<=126){
        c=line.at(pos);
        s=s+c;
        pos++;
    }
    return s;
}

int getLineNumber(string fn){
    ifstream myfile(fn.c_str());
    string line;
    int nb=0;
    while (getline(myfile, line)){
        nb++;
    }
    myfile.close();
    return nb;
}

char readChar(FILE *f,string fn){
    char r;
    if (fscanf(f,"%c",&r)==1) return r;
    else {
        cout<<"Error in the file "<<fn<<": file ended unexpected"<<endl;
        exit(EXIT_FAILURE);
    }
}

double readdouble(FILE *f,string fn){
    double r;
    if (fscanf(f,"%lf",&r)==1) return r;
    else {
        cout<<"Error in the file "<<fn<<" : real expected"<<endl;
        exit(EXIT_FAILURE);
    }
}

/*double readdouble(FILE *f,char c){
 double r;
 if (fscanf(f,"%lf",&r)==1){
 if (c=='-') {
 return -r;
 }
 else{
 return pow(10,log10(r)+1)*(c-'0')+r;
 }
 }
 else {
 if (c=='-') {
 return 0;
 }
 else return c-'0';
 }
 }
 double readdouble(FILE *f,int c){
 double r=c;
 char ch;
 fscanf(f,"%c",&ch);
 while (ch>='0' && ch<='9') {
 r=r*10+(ch-'0');
 fscanf(f,"%c",&ch);
 }
 return r;
 }*/

int readInt(FILE *f,string msg){
    int d;
    if (fscanf(f,"%d",&d)==1) return d;
    else {
        cout<<msg<<endl;
        exit(EXIT_FAILURE);
    }
}

string readSupport(FILE *f,string fn){
    string s="";
    char c=readChar(f,fn);
    while (c!=':' && c!=';') {
        s=s+c;
        c=readChar(f,fn);
    }
    return s;
}

void concat(list<int> & l1,list<int> l2){
    for (list<int>::iterator iter = l2.begin();iter != l2.end();iter++){
        l1.push_back(*iter);
    }
}

void concatPos(list<int> l1,list<int> &l2){
    for (list<int>::iterator iter = l1.begin();iter != l1.end();iter++){
        l2.push_back(*iter);
    }
}

void concat(stack<int> & l1,list<int> l2){
    for (list<int>::iterator iter = l2.begin();iter != l2.end();iter++){
        l1.push(*iter);
    }
}

void concat(list<int> & l1,stack<int> l2){
    while (!l2.empty()) {
        int i=l2.top();
        l2.pop();
        l1.push_back(i);
    }
}

/*
 list<int> arrayToList(int* & arr,list<int> li,int n){
 for (int i=0;i<n;i++){
 li.push_back(arr[i]);
 }
 return li;
 }
 void listToArray(list<int> li,int* & arr){
 int count=0;
 for (list<int>::iterator i=li.begin();i!=li.end();i++){
 arr[count]=(*i);
 count++;
 }
 }
 void listToArray(list<string> li,string* & arr){
 int count=0;
 for (list<string>::iterator i=li.begin();i!=li.end();i++){
 arr[count]=(*i);
 count++;
 }
 }
 void listToArray(list<double> li,double* & arr){
 int count=0;
 for (list<double>::iterator i=li.begin();i!=li.end();i++){
 arr[count]=(*i);
 count++;
 }
 }
 void listToArray(list<string> & inner,list<string> & leaves,string* & arr){
 int count=0;
 for (list<string>::iterator i=inner.begin();i!=inner.end();i++){
 arr[inner.size()-count-1]=(*i);
 count++;
 }
 for (list<string>::iterator i=leaves.begin();i!=leaves.end();i++){
 arr[count]=(*i);
 count++;
 }
 }*/
int getPosition(Node** nodes,string s,int n,int m){
    int i = n;
    int count=0;
    int k=0;
    while (i<m && count<2){
        if (nodes[i]->L.compare(s)==0){
            count++;
            k=i;
        }
        i++;
    }
    if (count==0) return -1;
    else if (count>1){
        cout<<"There are at least two leaves that have the same label"<<endl;
        exit(EXIT_FAILURE);
    }
    else return k;
}

bool comparecs(char* c,string s){
    for (int i=0;i<s.size();i++) {
        if (c[i]!=s.at(i)) return false;
    }
    return true;
}
bool comparec(char* c,char* s,int n){
    for (int i=0;i<n;i++) {
        if (c[i]!=s[i]) return false;
    }
    return true;
}

double minV(double* & B,int n){
    int i=1;
    while (B[i]==0) i++;
    double M=B[i];
    while (i<n){
        if (B[i]!=0 && B[i]<M) M=B[i];
        i++;
    }
    return M;
}

double Min(double a,double b){
    if (a<b) return a;
    else return b;
}

double Min(double a,double b,double c){
    if (a<b && a<c) return a;
    else if (b<a && b<c) return b;
    else return c;
}

double Max(double a,double b){
    if (a<b) return b;
    else return a;
}

double Maxi(double* & B,int n){
	   double max=0;
	   for (int i=0;i<n;i++){
           if (max<B[i]) max=B[i];
       }
	   return max;
}

double myabs(double a){
    if (a>=0) return a;
    else return -a;
}

bool isAncestor(Node** nodes,int i,int j){
    int x=j;
    while (x!=-1){
        if (x==i) return true;
        else x=nodes[x]->P;
    }
    return false;
}

int lca(list<int> a, list<int> b){
    for (list<int>::iterator ia=a.begin();ia!=a.end();ia++){
        for (list<int>::iterator ib=b.begin();(ib!=b.end() && *ib>=*ia);ib++){
            if (*ia==*ib){return *ia;}
        }
    }
    return 0;
}

int mrca(Node** nodes,int i,int j){
    int c = i;
    while (!isAncestor(nodes,c,j)){
        c=nodes[c]->P;
    }
    return c;
}

bool decrease(int a,int b){
    if (a>=b) return true;
    else return false;
}

int index(list<int> & L, int e){
    int ind=0;
    for (list<int>::iterator i=L.begin();i!=L.end();i++){
        if (*i==e) return ind;
        ind++;
    }
    return -1;
}

int index(string s,string* & L,int n){
    for (int i=0;i<n;i++){
        if (s.compare(L[i])==0){
            return i;
        }
    }
    return -1;
}
bool contain(int s,list<int> l){
    for (list<int>::iterator iter = l.begin();iter!=l.end();iter++){
        if (s==*iter) return true;
    }
    return false;
}

bool contain(int s,vector<int> l){
    for (int i=0;i<l.size();i++){
        if (s==l[i]) return true;
    }
    return false;
}

bool contain(string s,list<string> l){
    for (list<string>::iterator iter = l.begin();iter!=l.end();iter++){
        if (s.compare(*iter)==0) return true;
    }
    return false;
}

list<int> intersect(list<int> l1,list<int> l2){
    list<int>::iterator it1=l1.begin();
    list<int>::iterator it2=l2.begin();
    list<int> common;
    while (it1!=l1.end() && it2!=l2.end() && *it1==*it2) {
        common.push_back(*it1);
        it1++;
        it2++;
    }
    return common;
}

string readLabel(FILE *f,FILE *w){
    string s="";
    char c=readChar(f,"input tree");
    fprintf(w,"%c",c);
    while (c!=':' && c!=')'){
        s=s+c;
        fscanf(f,"%c",&c);
        fprintf(w,"%c",c);
    }
    return s.c_str();
}

string readLabel(char ch,FILE *f,int& a){
    string s="";
    s=s+ch;
    char c=readChar(f,"input tree");
    while (c!=':' && c!=';' && c!=')'){
        s=s+c;
        fscanf(f,"%c",&c);
    }
    if (c==')') {
        while (c==')') {
            fscanf(f,"%c",&c);
            a--;
        }
    }
    return s.c_str();
}


char readBracket(FILE *f,string fn){
    char c=readChar(f,fn);
    while (c!='('){
        c=readChar(f,fn);
    }
    return c;
}

char readCommaBracket(FILE *f,string fn,string& s){
    char c=readChar(f,fn);
    s="";
    while (c==' ' || c=='	'){ c=readChar(f,fn);}
    while (c!=',' && c!=')'){
        s=s+c;
        c=readChar(f,fn);
    }
    return c;
}
char read2P(FILE *f,string fn){
    char c= readChar(f,fn);
    if (c==';') return c;
    while (c!=':' && c!=';') c=readChar(f,fn);
    return c;
}


bool finish(bool* & flag,int size){
    for (int i=0;i<size;i++){
        if (flag[i]==false) {return false;}
    }
    return true;
}
int choose(bool* & flag,int* & Suc1,int* & Suc2,int size){
    for (int i=0;i<size;i++){
        if (!flag[i] && flag[Suc1[i]] && flag[Suc2[i]]) return i;
    }
    return -1;
}



Node** unrooted2rooted(Pr* pr,Node** nodes,int s){
    Node** nodes_new = new Node*[pr->nbBranches+1];
    for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
        nodes_new[i]=new Node();
        nodes_new[i]->P=nodes[i]->P;
        nodes_new[i]->B=nodes[i]->B;
        nodes_new[i]->L=nodes[i]->L;
        nodes_new[i]->D=nodes[i]->D;
        nodes_new[i]->type=nodes[i]->type;
        nodes_new[i]->lower=nodes[i]->lower;
        nodes_new[i]->upper=nodes[i]->upper;
        nodes_new[i]->status=nodes[i]->status;
    }
    for (int i=1; i<pr->nbINodes; i++) {
        nodes_new[i] = new Node();
        nodes_new[i]->P=nodes[i]->P;
        nodes_new[i]->B=nodes[i]->B;
        nodes_new[i]->L=nodes[i]->L;
    }
    nodes_new[0]=new Node();
    nodes_new[0]->P=-1;
    double br=nodes[s]->B;
    nodes_new[s]->B=br/2;
    nodes_new[1]->B=br/2;
    nodes_new[s]->V=variance(pr,br/2);
    nodes_new[1]->V=variance(pr,br/2);
    nodes_new[s]->P=0;
    nodes_new[1]->P=0;
    shiftInternalConstraints(pr);
    computeSuc_polytomy(pr,nodes_new);
    return nodes_new;
}

Node** unrooted2rootedS(Pr* pr,Node** nodes,int s){//simplier version, use only for remove outgroup
    Node** nodes_new = new Node*[pr->nbBranches+2];
    for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
        nodes_new[i+1]=new Node();
        nodes_new[i+1]->P=nodes[i]->P+1;
        nodes_new[i+1]->B=nodes[i]->B;
        nodes_new[i+1]->L=nodes[i]->L;
    }
    for (int i=0; i<pr->nbINodes; i++) {
        nodes_new[i+1] = new Node();
        nodes_new[i+1]->P=nodes[i]->P+1;
        nodes_new[i+1]->B=nodes[i]->B;
    }
    nodes_new[0]=new Node();
    nodes_new[0]->P=-1;
    double br=nodes[s]->B;
    nodes_new[s+1]->B=br/2;
    nodes_new[1]->B=br/2;
    nodes_new[s+1]->P=0;
    nodes_new[1]->P=0;
    pr->nbINodes+=1;
    pr->nbBranches+=1;
    computeSuc_polytomy(pr,nodes_new);
    return nodes_new;
}

void computeVariance(Pr* pr,Node** nodes){
    if (pr->variance==1 || pr->variance==2){
        for (int i=1;i<=pr->nbBranches;i++){
            nodes[i]->V=(nodes[i]->B+(double)(pr->c)/pr->seqLength)/pr->seqLength;
        }
    }
    else{
        for (int i=1;i<=pr->nbBranches;i++){
            nodes[i]->V=1./(double)(pr->seqLength);
        }
    }
}

void computeVarianceEstimateRoot(Pr* pr,Node** nodes,double br){
    if (pr->variance==1 || pr->variance==2){
        for (int i=1;i<=pr->nbBranches;i++){
            if (nodes[i]->P!=0) {
                nodes[i]->V=(nodes[i]->B+(double)(pr->c)/pr->seqLength)/pr->seqLength;
            }
            else nodes[i]->V=variance(pr,br);
        }
    }
    else{
        for (int i=1;i<=pr->nbBranches;i++){
            nodes[i]->V=1./(double)(pr->seqLength);
        }
    }
}

double variance(Pr* pr,double b){
    if (pr->variance==1 || pr->variance==2) return (b+(double)(pr->c)/pr->seqLength)/pr->seqLength;
    else return 1./(double)(pr->seqLength);
}

void computeNewVariance(Pr* pr,Node** nodes){
    for (int i=1;i<=pr->nbBranches;i++){
        if (nodes[i]->D>=nodes[nodes[i]->P]->D){
            nodes[i]->V=(pr->rho*nodes[i]->D-pr->rho*nodes[nodes[i]->P]->D+(double)(pr->c)/pr->seqLength)/pr->seqLength;
        }
        else{
            nodes[i]->V=((double)(pr->c)/pr->seqLength)/pr->seqLength;
        }
    }
}

void computeNewVarianceEstimateRoot(Pr* pr,Node** nodes){
    double br=0;
    for (vector<int>::iterator iter=nodes[0]->suc.begin(); iter!=nodes[0]->suc.end(); iter++) {
        br+=(nodes[*iter]->D-nodes[0]->D)*pr->rho;
    }
    for (int i=1;i<=pr->nbBranches;i++){
        if (nodes[i]->P==0) {
            if (br>=0){
                nodes[i]->V=variance(pr,br);
            }
            else{
                nodes[i]->V=((double)(pr->c)/pr->seqLength)/pr->seqLength;
            }
        }
        else{
            if (nodes[i]->D>=nodes[nodes[i]->P]->D){
                nodes[i]->V=(pr->rho*nodes[i]->D-pr->rho*nodes[nodes[i]->P]->D+(double)(pr->c)/pr->seqLength)/pr->seqLength;
            }
            else{
                nodes[i]->V=((double)(pr->c)/pr->seqLength)/pr->seqLength;
            }
        }
    }
}

void myExit( string msg, ... )
{
    va_list ptr;
    fprintf( stderr, "Error: " );
    va_start( ptr, msg );
    vfprintf( stderr, msg.c_str(), ptr );
    va_end( ptr );
    //fflush( NULL );
    exit( EXIT_FAILURE );
}


void myErrorMsg( string msg, ... )
{
    va_list ptr;
    fprintf( stderr, "Error: " );
    va_start( ptr, msg );
    vfprintf( stderr, msg.c_str(), ptr );
    va_end( ptr );
    
}

bool isReal( const char* str )
{
    while( *str!='\0' )
    {
        if( !( ('0'<=*str && *str<='9') || *str=='e' || *str=='E' || *str=='.' || *str=='-' ) )
            return false;
        str++;
    }
    return true;
}

bool isInteger( const char* str )
{
    if( *str=='-' )
        str++;
    while( *str!='\0' )
    {
        if( *str<'0' || '9'<*str )
            return false;
        str++;
    }
    return true;
}


void sort(int* & tab,int size){
    for (int i=0;i<size;i++){
        for (int j=i;j<size;j++){
            if (tab[i]>tab[j]){
                int temp = tab[i];
                tab[i]=tab[j];
                tab[j]=temp;
            }
        }
    }
}
void sort(double* & tab,int size){
    for (int i=0;i<size;i++){
        for (int j=i;j<size;j++){
            if (tab[i]>tab[j]){
                double temp = tab[i];
                tab[i]=tab[j];
                tab[j]=temp;
            }
        }
    }
}

double* sortTab(double* tab,int size){
    double* sortedTab = new double[size];
    for (int i=0;i<size;i++){
        sortedTab[i] = tab[i];
    }
    for (int i=0;i<size;i++){
        for (int j=i;j<size;j++){
            if (sortedTab[i]>sortedTab[j]){
                double temp = sortedTab[i];
                sortedTab[i]=sortedTab[j];
                sortedTab[j]=temp;
            }
        }
    }
    return sortedTab;
}

int index(int* & tab,int value,int size){
    for (int i=0;i<size;i++){
        if (value==tab[i]) return i;
    }
    return -1;
}

int mrca(Node** nodes,list<int> taxa){
    
    int t=taxa.front();
    
    taxa.pop_front();
    
    bool flag = false;
    
    while (!flag && nodes[t]->P!=-1){
        
        t = nodes[t]->P;
        
        flag=true;
        
        for (list<int>::iterator ia=taxa.begin();ia!=taxa.end();ia++){
            
            int j=*ia;
            
            if (!isAncestor(nodes,t,j)) {flag=false;break;}
            
        }
        
    }
    
    return t;
    
}
int mrca(Node** nodes,vector<int> taxa){
    int first=taxa.front();
    int t=first;
    bool flag = false;
    while (!flag && nodes[t]->P!=-1){
        t = nodes[t]->P;
        flag=true;
        for (vector<int>::iterator ia=taxa.begin();ia!=taxa.end();ia++){
            int j=*ia;
            if (j!=first && !isAncestor(nodes,t,j)) {flag=false;break;}
        }
    }
    return t;
}
/*
 int mrca(Node** nodes,list<int> taxa){
 int t=taxa.front();
 taxa.pop_front();
 bool flag = false;
 while (!flag && nodes[t]->P!=-1){
 t = nodes[t]->P;
 flag=true;
 for (list<int>::iterator ia=taxa.begin();ia!=taxa.end();ia++){
 int j=*ia;
 if (!isAncestor(nodes,t,j)) {flag=false;break;}
 }
 }
 return t;
 }
 */
void computeSuc(int* & Pre,int* & Suc1,int* & Suc2,int size,int n){
    for (int i=0;i<n;i++){
        Suc1[i]=-1;
    }
    for (int i=0;i<size;i++){
        if (Pre[i]!=-1){
            if (Suc1[Pre[i]]==-1) Suc1[Pre[i]]=i;
            else if (Suc1[Pre[i]]>i){
                Suc2[Pre[i]]=Suc1[Pre[i]];
                Suc1[Pre[i]]=i;
            }
            else Suc2[Pre[i]]=i;
        }
    }
}

bool markLeaf(Node* no){
    return no->status>=16;
}

bool limit(Node* no){
    return (no->status % 4 !=0);
}

void activeTC(Node* no){
    no->status+=4;
}

bool lower(Node* no){
    return (no->status %2 == 1);
}

bool upper(Node* no){
    return (no->status /2) %2 ==1 ;
}

bool tc(Node* no){
    return (no->status/4)%2==1;
}
bool leaf(Node* no){
    return ((no->status/8) % 2 == 1);
}

void activeMarkLeaf(Node* no){
    if (!markLeaf(no)) no->status+=16;
}

void desactiveMarkLeaf(Node* no){
    if (markLeaf(no)) no->status-=16;
}

void activeUpper(Node* no){
    no->status+=2+8;
}
void activeLower(Node* no){
    no->status+=1+8;
}
void desactive(Node* no){
    if (no->type=='p') {
        no->status=8;
    }
    else no->status=0;
}
void desactiveTC(Node* no){
    if (tc(no)) no->status-=4;
}
void desactiveLimit(Node* no){
    if (lower(no)) no->status-=1+8;
    if (upper(no)) no->status-=2+8;
}

bool integrateConstrainL(Date* d1,Node* n2){//n1 is an ancestor of n2
    if (n2->type=='n') {
        if (d1->type=='l' || d1->type=='b') {
            n2->type='l';
            n2->lower=d1->lower;
        }
        if (d1->type=='p') {
            n2->type='l';
            n2->lower=d1->date;
        }
    }
    if (n2->type=='l' || n2->type=='b'){
        if (d1->type=='l' || d1->type=='b') {
            n2->lower=max(d1->lower,n2->lower);
        }
        if (d1->type=='p') {
            n2->lower=max(n2->lower,d1->date);
        }
    }
    if (n2->type=='u' || n2->type=='b'){
        if (d1->type=='l' || d1->type=='b') {
            if (d1->lower>n2->upper) {
                return false;
            }
            n2->type='b';
            n2->lower=d1->lower;
        }
        if (d1->type=='u' || d1->type=='b') {
            n2->upper=min(d1->upper,n2->upper);
        }
        if (d1->type=='p') {
            if (d1->date>n2->upper) {
                return false;
            }
            n2->lower=d1->date;
            n2->type='b';
        }
    }
    return true;
}

bool integrateConstrainU(Date* d1,Node* n2){//n1 is a child of n2
    if (n2->type=='n') {
        if (d1->type=='u' || d1->type=='b') {
            n2->type='u';
            n2->upper=d1->upper;
        }
        if (d1->type=='p') {
            n2->type='u';
            n2->upper=d1->date;
        }
    }
    if (n2->type=='u' || n2->type=='b'){
        if (d1->type=='u' || d1->type=='b') {
            n2->upper=min(d1->upper,n2->upper);
        }
        if (d1->type=='p') {
            n2->upper=max(n2->upper,d1->date);
        }
    }
    if (n2->type=='l' || n2->type=='b'){
        if (d1->type=='u' || d1->type=='b') {
            if (d1->upper<n2->lower) {
                return false;
            }
            else if (d1->upper==n2->lower){
                n2->type='p';
                n2->D=n2->lower;
            }
            else{
                n2->type='b';
                n2->upper=d1->upper;
            }
        }
        if (d1->type=='p') {
            if (d1->date<n2->lower) {
                return false;
            }
            else if (d1->date==n2->lower){
                n2->type='p';
                n2->D=n2->lower;
                n2->status=8;
            }
            else{
                n2->upper=d1->date;
                n2->type='b';
            }
            
        }
    }
    return true;
}

bool initConstraintReRooted(Pr* pr,Node** nodes,int r,int p_r){
    bool constraintConsistent=true;
    for (vector<Date*>::iterator iter=pr->internalConstraints.begin();iter!=pr->internalConstraints.end();iter++){
        Date* no = (*iter);
        int k=-1;
        if (no->mrca.size()==0){
            k=no->id;
            if (k==0){
                if (!integrateConstrainL(no,nodes[r])) return false;//or swap r and p_r
                if (!integrateConstrainU(no,nodes[p_r])) return false;
            }
        }
        else{
            k=mrca(nodes,(*iter)->mrca);
        }
        bool bl = (nodes[k]->addConstraint(*iter));
        constraintConsistent = constraintConsistent && bl;
    }
    if (constraintConsistent){
        for (int i=0; i<=pr->nbBranches; i++) {
            if (nodes[i]->type=='l' || nodes[i]->type=='b') {
                nodes[i]->D = nodes[i]->lower;
                activeLower(nodes[i]);
            }
            else if (nodes[i]->type=='u'){
                nodes[i]->D = nodes[i]->upper;
                activeUpper(nodes[i]);
            }
        }
    }
    return constraintConsistent;
}

bool initConstraint(Pr* pr,Node** nodes){//internalConstraints of the old tree, to assign for new rerooted tree
    bool constraintConsistent=true;
    for (vector<Date*>::iterator iter=pr->internalConstraints.begin();iter!=pr->internalConstraints.end();iter++){
        Date* no = (*iter);
        int k=-1;
        if (no->mrca.size()==0){
            k=no->id;
        }
        else{
            k=mrca(nodes,no->mrca);
        }
        bool bl = (nodes[k]->addConstraint(*iter));
        constraintConsistent = constraintConsistent && bl;
    }
    if (constraintConsistent){
        for (int i=0; i<=pr->nbBranches; i++) {
            if (nodes[i]->type=='l' || nodes[i]->type=='b') {
                nodes[i]->D = nodes[i]->lower;
                activeLower(nodes[i]);
            }
            else if (nodes[i]->type=='u'){
                nodes[i]->D = nodes[i]->upper;
                activeUpper(nodes[i]);
            }
        }
    }
    return constraintConsistent;
}

Node** cloneLeaves(Pr* pr,Node** nodes,int f){
    Node** nodes_new =  new Node*[pr->nbBranches+1+f];
    for (int i=0; i<pr->nbINodes; i++) {
        nodes_new[i+f] = new Node();
    }
    for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
        nodes_new[i+f]=new Node();
        nodes_new[i+f]->P=nodes[i]->P+f;
        nodes_new[i+f]->B=nodes[i]->B;
        nodes_new[i+f]->L=nodes[i]->L;
        nodes_new[i+f]->V=nodes[i]->V;
        nodes_new[i+f]->type=nodes[i]->type;
        nodes_new[i+f]->lower=nodes[i]->lower;
        nodes_new[i+f]->upper=nodes[i]->upper;
        nodes_new[i+f]->D=nodes[i]->D;
        nodes_new[i+f]->status=nodes[i]->status;
    }
    return nodes_new;
}

void cloneInternalNodes(Pr* pr,Node** nodes,Node** nodes_new,int f){
    for (int i=0; i<pr->nbINodes; i++) {
        nodes_new[i+f]->P=nodes[i]->P+f;
        nodes_new[i+f]->B=nodes[i]->B;
        nodes_new[i+f]->L=nodes[i]->L;
        nodes_new[i+f]->suc.clear();
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            nodes_new[i+f]->suc.push_back((*iter)+f);
        }
    }
    for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
        nodes_new[i+f]->P=nodes[i]->P+f;
        nodes_new[i+f]->B=nodes[i]->B;
    }
}

void shiftInternalConstraints(Pr* pr){
    vector<Date*> newIC;
    for (vector<Date*>::iterator iter=pr->internalConstraints.begin();iter!=pr->internalConstraints.end();iter++){
        Date* d = (*iter);
        if (d->mrca.size()>0){
            vector<int> new_mrca;
            for (vector<int>::iterator iter=d->mrca.begin();iter!=d->mrca.end();iter++){
                new_mrca.push_back(*iter+1);
            }
            d->mrca=new_mrca;
        }
        else{
            d->id++;
        }
        newIC.push_back(d);
    }
    pr->internalConstraints=newIC;
}

bool reroot_rootedtree(double& br,int r,int s10,int s20,Pr* pr,Node** nodes,Node** nodes_new){
    cloneInternalNodes(pr,nodes,nodes_new,0);
    if (r==s10 || r==s20){
        br = nodes[s10]->B+nodes[s20]->B;
        nodes_new[s10]->B=br;
        nodes_new[s20]->B=br;
        computeVarianceEstimateRoot(pr,nodes_new,br);
        return initConstraint(pr,nodes_new);
    }
    else {
        nodes_new[0]->L="";
        nodes_new[0]->P=-1;
        nodes_new[r]->P=0;
        nodes_new[nodes[r]->P]->P=0;
        nodes_new[0]->suc.clear();
        nodes_new[0]->suc.push_back(r);
        nodes_new[0]->suc.push_back(nodes[r]->P);
        int ii=r;
        int i=nodes[r]->P;
        int j=nodes[i]->P;
        while (j!=0){
            nodes_new[i]->suc.clear();
            nodes_new[i]->suc.push_back(j);
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                if (*iter!=ii) {
                    nodes_new[i]->suc.push_back(*iter);
                }
            }
            nodes_new[j]->P=i;
            nodes_new[j]->B=nodes[i]->B;
            ii=i;
            i=j;
            j=nodes[i]->P;
        }
        int k=s10;
        if (k==i) k=s20;
        nodes_new[k]->P=i;
        nodes_new[i]->suc.clear();
        nodes_new[i]->suc.push_back(k);
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            if (*iter!=ii) {
                nodes_new[i]->suc.push_back(*iter);
            }
        }
        br=nodes[r]->B;
        nodes_new[k]->B=nodes[i]->B+nodes[k]->B;
        nodes_new[r]->B=br;
        nodes_new[nodes[r]->P]->B=br;
        computeVarianceEstimateRoot(pr,nodes_new,br);
        return initConstraintReRooted(pr, nodes_new,k,i);
    }
}


bool reroot_rootedtree(double& br,int r,int s10,int s20,Pr* pr,Node** nodes,Node** nodes_new,int* & P_ref,int* & tab){
    cloneInternalNodes(pr,nodes,nodes_new,0);
    for (int i=0; i<=pr->nbBranches; i++) {
        tab[i]=i;
        P_ref[i]=nodes[i]->P;
    }
    if (r==s10 || r==s20){
        for (int i=0;i<=pr->nbBranches;i++){
            nodes_new[i]->P=nodes[i]->P;
            P_ref[i]=nodes[i]->P;
            nodes_new[i]->B=nodes[i]->B;
        }
        br = nodes[s10]->B+nodes[s20]->B;
        nodes_new[s10]->B=br;
        nodes_new[s20]->B=br;
        computeVarianceEstimateRoot(pr,nodes_new,br);
        return initConstraint(pr, nodes_new);
    }
    else {
        nodes_new[0]->P=-1;
        P_ref[0]=-1;
        nodes_new[r]->P=0;
        P_ref[r]=0;
        nodes_new[nodes[r]->P]->P=0;
        P_ref[nodes[r]->P]=0;
        nodes_new[0]->suc.clear();
        nodes_new[0]->suc.push_back(r);
        nodes_new[0]->suc.push_back(nodes[r]->P);
        int ii=r;
        int i=nodes[r]->P;
        int j=nodes[i]->P;
        tab[i]=r;
        while (j!=0){
            nodes_new[i]->suc.clear();
            nodes_new[i]->suc.push_back(j);
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                if (*iter!=ii) {
                    nodes_new[i]->suc.push_back(*iter);
                }
            }
            tab[j]=i;
            nodes_new[j]->P=i;
            P_ref[j]=i;
            nodes_new[j]->B=nodes[i]->B;
            ii=i;
            i=j;
            j=nodes[i]->P;
        }
        int k=s10;
        if (k==i) k=s20;
        nodes_new[k]->P=i;
        P_ref[k]=i;
        nodes_new[i]->suc.clear();
        nodes_new[i]->suc.push_back(k);
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            if (*iter!=ii) {
                nodes_new[i]->suc.push_back(*iter);
            }
        }
        br=nodes[r]->B;
        nodes_new[k]->B=nodes[i]->B+nodes[k]->B;
        nodes_new[r]->B=br;
        nodes_new[nodes[r]->P]->B=br;
        computeVarianceEstimateRoot(pr,nodes_new,br);
        return initConstraintReRooted(pr, nodes_new,k,i);
    }
}

int reroot_rootedtree(int r,Pr* pr,Node** nodes,Node** nodes_new){//used in extrait outgroups
    for (int i=0; i<pr->nbINodes; i++) {
        nodes_new[i] = new Node();
        nodes_new[i]->P=nodes[i]->P;
        nodes_new[i]->B=nodes[i]->B;
        nodes_new[i]->L=nodes[i]->L;
    }
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s10=*iter;
    iter++;
    int s20=*iter;
    if (r==s10 || r==s20){
        for (int i=0;i<=pr->nbBranches;i++){
            nodes_new[i]->P=nodes[i]->P;
            nodes_new[i]->B=nodes[i]->B;
        }
        double br = nodes[s10]->B+nodes[s20]->B;
        nodes_new[s10]->B=br/2.;
        nodes_new[s20]->B=br/2.;
        if (r==s10) {
            return s20;
        }
        else return s10;
    }
    else {
        nodes_new[0]->L="";
        nodes_new[0]->P=-1;
        nodes_new[r]->P=0;
        nodes_new[nodes[r]->P]->P=0;
        int i=nodes[r]->P;
        int j=nodes[i]->P;
        while (j!=0){
            nodes_new[j]->P=i;
            nodes_new[j]->B=nodes[i]->B;
            i=j;
            j=nodes[i]->P;
        }
        int k=s10;
        if (k==i) k=s20;
        nodes_new[k]->P=i;
        double br=nodes[r]->B;
        nodes_new[k]->B=nodes[i]->B+nodes[k]->B;
        nodes_new[r]->B=br/2.;
        nodes_new[nodes[r]->P]->B=br/2.;
        return nodes[r]->P;
    }
}

void computeObjective(Pr* pr,Node** nodes){
    pr->objective = 0;
    for (int i=1;i<=pr->nbBranches;i++){
        //p+=(B[i]-rho*T[i]+rho*T[P[i]])*(B[i]-rho*T[i]+rho*T[P[i]])/(2*V[i]) +log(2*M_PI*V[i])/2;
        pr->objective+=(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[nodes[i]->P]->D)*(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[nodes[i]->P]->D)/(nodes[i]->V);
    }//p = -(log likelihood)
}

void computeObjectiveMultiRates(Pr* pr,Node** nodes){
    pr->objective = 0;
    for (int i=1;i<=pr->nbBranches;i++){
        double rate = pr->rho*pr->multiplierRate[nodes[i]->rateGroup];
        pr->objective+=(nodes[i]->B-rate*nodes[i]->D+rate*nodes[nodes[i]->P]->D)*(nodes[i]->B-rate*nodes[i]->D+rate*nodes[nodes[i]->P]->D)/(nodes[i]->V);
    }//p = -(log likelihood)
}

void computeObjectiveMultiRates(Pr* pr,Node** nodes,double* B,double* V){
    pr->objective = 0;
    double test = 0;
    for (int i=1;i<=pr->nbBranches;i++){
        double rate = pr->rho*pr->multiplierRate[nodes[i]->rateGroup];
        pr->objective+=(B[i]-rate*nodes[i]->D+rate*nodes[nodes[i]->P]->D)*(B[i]-rate*nodes[i]->D+rate*nodes[nodes[i]->P]->D)/(V[i]);
    }//p = -(log likelihood)
}

void computeObjectiveEstimateRoot(int r,int p_r,double br,Pr* pr,Node** nodes){
    pr->objective = (br-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D+2*pr->rho*nodes[0]->D)*(br-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D+2*pr->rho*nodes[0]->D)/nodes[r]->V;
    for (int i=1;i<=pr->nbBranches;i++){
        //p+=(B[i]-rho*T[i]+rho*T[P[i]])*(B[i]-rho*T[i]+rho*T[P[i]])/(2*V[i]) +log(2*M_PI*V[i])/2;
        if (i!=r && i!=p_r) {
            pr->objective+=(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[nodes[i]->P]->D)*(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[nodes[i]->P]->D)/(nodes[i]->V);
        }
    }//p = -(log likelihood)
}
/*
 double phi_lambda(int n,int m,int* &P,double* &B,double* &V,double* &T,double rho){
 double p=0;
 for (int i=1;i<=m;i++){
 if (P[i]!=0){
 p+=(B[i]-rho*T[i]+rho*T[P[i]])*(B[i]-rho*T[i]+rho*T[P[i]])/(V[i]);
 }
 }
 return p;
 }
 double phi(int n,int m,double &rho,int* & P,double* & B,double* & V,double* & T){
 double a=0;
 double b=0;
 double c=0;
 for (int i=1;i<=m;i++){
 a+=(T[P[i]]-T[i])*(T[P[i]]-T[i])/V[i];
 b+=2*(T[P[i]]-T[i])*B[i]/V[i];
 c+=B[i]*B[i]/V[i];
 }
 rho=-b/a/2;
 return a*rho*rho+b*rho+c;
 }*/

string newick(int i,int n,int* & P,int* & Suc1,int* & Suc2,string* & Labels,double* & B){
    ostringstream b;
    b<<B[i];
    if (i>=n) return Labels[i]+":"+b.str();
    else{
        int s1=Suc1[i];
        int s2=Suc2[i];
        string l1=newick(s1,n,P,Suc1,Suc2,Labels,B);
        string l2=newick(s2,n,P,Suc1,Suc2,Labels,B);
        if (P[i]!=-1){
            return "("+l1+","+l2+")"+Labels[i]+":"+b.str();
        }
        else{
            return "("+l1+","+l2+")"+Labels[i]+";\n";
        }
    }
}

string newick(int i,int terminate,Pr* pr,Node** nodes){;
    ostringstream b;
    if (i>0){
        b<<nodes[i]->B;
    }
    if (i>=pr->nbINodes) return nodes[i]->L+":"+b.str();
    else{
        string newLabel="(";
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            int s = *iter;
            string l=newick(s,terminate,pr,nodes);
            if (iter==nodes[i]->suc.begin()) newLabel+=l;
            else newLabel+=","+l;
        }
        if (i!=terminate) {
            return newLabel+")"+nodes[i]->L+":"+b.str();
        }
        else{
            return newLabel+")"+nodes[i]->L+";\n";
        }
    }
}

string newickDate(int i,Pr* pr,Node** nodes){
    ostringstream b;
    if (i>0){
        b<<(nodes[i]->D-nodes[nodes[i]->P]->D);
    }
    if (i>=pr->nbINodes) return nodes[i]->L+":"+b.str();
    else{
        string newLabel="(";
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            int s = *iter;
            string l=newickDate(s,pr,nodes);
            if (iter==nodes[i]->suc.begin()) newLabel+=l;
            else newLabel+=","+l;
        }
        if (i>0) {
            return newLabel+")"+nodes[i]->L+":"+b.str();
        }
        else{
            return newLabel+")"+nodes[i]->L+";\n";
        }
    }
}

string nexus(int i,Pr* pr,Node** nodes){
    ostringstream b,date;
    if (i>0){
        b<<nodes[i]->B;
    }
    date<<nodes[i]->D;
    if (i>=pr->nbINodes) return nodes[i]->L+"[&date="+date.str()+"]:"+b.str();
    else{
        string newLabel="(";
        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
            int s = *iter;
            string l=nexus(s,pr,nodes);
            if (iter==nodes[i]->suc.begin()) newLabel+=l;
            else newLabel+=","+l;
        }
        if (i>0) {
            if (myabs(nodes[i]->B)>toCollapse) return newLabel+")"+nodes[i]->L+"[&date="+date.str()+"]:"+b.str();
            else return newLabel+")"+nodes[i]->L+":"+b.str();
        }
        else{
            return newLabel+")"+nodes[i]->L+"[&date="+date.str()+"];\n";
        }
    }
}


string nexusIC(int i,Pr* pr,Node** nodes,double* T_min,double* T_max){
    ostringstream b,date;
    if (i>0){
        b<<nodes[i]->B;
    }
    date<<nodes[i]->D;
    if (i>=pr->nbINodes && nodes[i]->type=='p') return nodes[i]->L+"[&date="+date.str()+"]:"+b.str();
    else{
        ostringstream tmin,tmax;
        tmin<< T_min[i];
        tmax<< T_max[i];
        if (i>=pr->nbINodes) {
            return nodes[i]->L+"[&date="+date.str()+"][&CI=\""+date.str()+"("+tmin.str()+","+tmax.str()+")\"]:"+b.str();
        }
        else{
            string newLabel="(";
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                int s = *iter;
                string l=nexusIC(s,pr,nodes,T_min,T_max);
                if (iter==nodes[i]->suc.begin()) newLabel+=l;
                else newLabel+=","+l;
            }
            if (i>0) {
                if (myabs(nodes[i]->B)>toCollapse) return newLabel+")"+nodes[i]->L+"[&date="+date.str()+"][&CI=\""+date.str()+"("+tmin.str()+","+tmax.str()+")\"]:"+b.str();
                else return newLabel+")"+nodes[i]->L+":"+b.str();
            }
            else{
                return newLabel+")"+nodes[i]->L+"[&date="+date.str()+"][&CI=\""+date.str()+"("+tmin.str()+","+tmax.str()+")\"];\n";
            }
            
        }
    }
}

double* variance(int w,int m,double* B,int c,int s){
    double* V = new double[m+1];
    if (w==1 || w==2){
        for (int i=1;i<=m;i++){
            //V[i]=(B[i]+c/(c+s))/(c+s);
            V[i]=(B[i]+(double)c/s)/s;
        }
    }
    else{
        for (int i=1;i<=m;i++){
            V[i]=1./(double)(s);
        }
    }
    return V;
}

double* newvariance(int m,double rho,int* P,double* T,int c,int s){
    double* V = new double[m+1];
    for (int i=1;i<=m;i++){
        if (T[i]>=T[P[i]]) V[i]=(rho*T[i]-rho*T[P[i]]+(double)c/s)/s;
        else V[i]=((double)c/s)/s;
    }
    return V;
}

list<string> getOutgroup(string fn){
    list<string> result;
    int lineNb=getLineNumber(fn);
    FILE * o = fopen(fn.c_str(),"rt");
    if (o==NULL) cout<<"Impossible to open outgroup file"<<endl;
    else{
        int ino=readInt(o,"Error in the outgroup file, the file should begin with an integer (the number of outgroups)");
        if (lineNb-1<ino) {
            cout<<"The number of given outgroups is small than the number of outgroups to read. Please change the number of outgroups to read at the first line of the outgroup file."<<endl;
            exit(EXIT_FAILURE);
        }
        for (int i=0;i<ino;i++) result.push_back(readWord(o,fn));
    }
    return result;
}

void initialize_status(Pr* &pr,Node** &nodes){
    for (int i=0; i<=pr->nbBranches; i++) {
        if (nodes[i]->type=='p') nodes[i]->status=8;
        else nodes[i]->status=0;
    }
}

list<int> getActiveSet(Pr* pr,Node** nodes){
    list<int> active_set;
    for (int i=0;i<=pr->nbBranches;i++){
        if (tc(nodes[i])) active_set.push_back(i);
        if (limit(nodes[i])) active_set.push_back(-i);
    }
    return active_set;
}

void computeSuc_polytomy(Pr* pr,Node** nodes){
    for (int i=0;i<pr->nbINodes;i++){
        nodes[i]->suc.clear();
    }
    for (int i=1;i<=pr->nbBranches;i++){
        nodes[nodes[i]->P]->suc.push_back(i);
    }
}

list<int> pos_polytomy(int i,Pr* pr,Node** nodes){
    list<int> l;
    if (i>=pr->nbINodes) return l;
    else{
        for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
            if (*iter<pr->nbINodes) {
                list<int> l1 = pos_polytomy(*iter,pr,nodes);
                for (list<int>::iterator iter1=l1.begin();iter1!=l1.end();iter1++){
                    l.push_back(*iter1);
                }
            }
        }
        l.push_back(i);
        return l;
    }
}

list<int> postorder_polytomy(Pr* pr,Node** nodes){
    int root=0;
    for (root=0;root<pr->nbINodes;root++){
        if (nodes[root]->P==-1) break;
    }
    return pos_polytomy(root,pr,nodes);
}

list<int> pre_polytomy(int i,Pr* pr,Node** nodes){
    list<int> l;
    if (i>=pr->nbINodes) return l;
    else{
        l.push_back(i);
        for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
            if (*iter<pr->nbINodes) {
                list<int> l1 = pre_polytomy(*iter,pr,nodes);
                for (list<int>::iterator iter1=l1.begin();iter1!=l1.end();iter1++){
                    l.push_back(*iter1);
                }
            }
        }
        return l;
    }
}

list<int> preorder_polytomy(Pr* pr,Node** nodes){
    int root=0;
    for (root=0;root<pr->nbINodes;root++){
        if (nodes[root]->P==-1) break;
    }
    return pre_polytomy(root,pr,nodes);
}

list<int> down_polytomy(int i,Pr* pr,Node** nodes){
    list<int> result;
    result.push_back(i);
    activeMarkLeaf(nodes[i]);
    nodes[i]->D=nodes[nodes[i]->P]->D;
    if (i<pr->nbINodes){
        for (vector<int>::iterator iter=nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
            if (tc(nodes[*iter])){
                list<int> l1=down_polytomy(*iter,pr,nodes);
                concat(result,l1);
            }
        }
    }
    return result;
}

stack<int>* computeFeuilles_polytomy(list<int> ls,Pr* pr,Node** nodes){
    stack<int>* feuilles = new stack<int>[ls.size()];
    int count=0;
    for (list<int>::iterator iter=ls.begin();iter!=ls.end();iter++){
        int i=*iter;
        bool alone=true;
        list<int> ai;
        for (vector<int>::iterator iter=nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
            if (tc(nodes[*iter])){
                alone=false;
                ai.push_back(*iter);
            }
        }
        if (!tc(nodes[i]) && (i>=pr->nbINodes || alone)){//nodes[i] is alone
            feuilles[count].push(i);
        }
        else {
            if (!tc(nodes[i])){
                feuilles[count].push(i);
            }
            else{
                int j=i;
                while (j!=-1 && tc(nodes[j])){
                    feuilles[count].push(j);
                    int k=j;
                    j=nodes[j]->P;
                    if (j!=-1 && tc(nodes[k])){
                        activeMarkLeaf(nodes[j]);//leaf
                        nodes[j]->D=nodes[i]->D;
                        for (vector<int>::iterator iter=nodes[j]->suc.begin(); iter!=nodes[j]->suc.end(); iter++) {
                            if ((*iter)!=k && tc(nodes[*iter])) {
                                ai.push_back(*iter);
                            }
                        }
                    }
                }
            }
            for (list<int>::iterator it=ai.begin();it!=ai.end();it++){
                concat(feuilles[count],down_polytomy(*it,pr,nodes));
            }
        }
        count++;
        
    }
    return feuilles;
}


list<int> suc_polytomy(int i,int j,Pr* pr,Node** nodes,int* & Pre,list<int> &suc){
    list<int> result;
    if (leaf(nodes[i])) {
        nodes[j]->D=nodes[i]->D;
        activeMarkLeaf(nodes[j]);
    }
    if (j>=pr->nbINodes) {
        result.push_back(j);
        Pre[j]=i;
        if (markLeaf(nodes[j]) || j<pr->nbINodes) suc.push_back(j);
    }
    else{
        for (vector<int>::iterator iter=nodes[j]->suc.begin(); iter!=nodes[j]->suc.end(); iter++) {
            int s=*iter;
            if (!tc(nodes[s])) {
                Pre[s]=i;
                if (markLeaf(nodes[s]) || s<pr->nbINodes) suc.push_back(s);
            }
            else{
                list<int> l1=suc_polytomy(i,s,pr,nodes,Pre,suc);
                concatPos(l1,result);
            }
        }
        if (j!=i) result.push_back(j);
    }
    return result;
}

void reduceTree_polytomy(Pr* pr,Node** nodes,int* &Pre,list<int>* & Suc,list<int>* &internal){
    int count=0;
    Pre[0]=-1;
    for (int i=0;i<pr->nbINodes;i++){
        if (!tc(nodes[i]) && (!markLeaf(nodes[i]) || nodes[i]->type=='p')){
            bool bl=false;
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                if (tc(nodes[*iter])) {
                    bl=true;
                    break;
                }
            }
            if (bl){
                internal[count]=suc_polytomy(i,i,pr,nodes,Pre,Suc[i]);
                count++;
            }
            else {
                for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                    Pre[*iter]=i;
                    if (markLeaf(nodes[*iter]) || *iter<pr->nbINodes) Suc[i].push_back(*iter);
                }
            }
        }
        else {
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                Pre[*iter]=i;
                if (markLeaf(nodes[*iter]) || *iter<pr->nbINodes) Suc[i].push_back(*iter);
            }
        }
    }
}

void checkRooted(Pr* opt){
    FILE * tree = fopen(opt->inFile.c_str(),"rt");
    if (tree==NULL){
        cout<<"Error: can not open the input file"<<endl;
        exit(EXIT_FAILURE);
    }
    stack<int> pileNode;
    char c = readBracket(tree,"input tree");
    int a=1;
    int s=0;
    int nbChild=0;
    int n=0;
    int m=0;
    do{
        c = readChar(tree,"input tree");
        if (c==')'){
            a--;
            s=0;
            nbChild=0;
            while (!pileNode.empty() && s!=-1) {
                s=pileNode.top();pileNode.pop();
                if (s!=-1){
                    nbChild++;
                }
            }
            string sp=readSupport(tree,"input tree");
            if (a>0) {
                double d = readdouble(tree,"input tree");
            }
            pileNode.push(1);
            n++;
            m++;
        }
        else if (c!='(' && c!=',' && c!=-1 && c!=';' && c!='\n'){
            string s= readLabel(c,tree,a);
            pileNode.push(1);
            double d =readdouble(tree,"input tree");
            m++;
        }
        else if (c=='(') {a++;pileNode.push(-1);}
        else if (c=='\n') {
            c=readChar(tree,"input tree");
        }
    }  while (a>0);
    fclose(tree);
    m--;
    if (m==2*n) {
        opt->rooted=true;
    }
    else if (m==2*n+1 && nbChild==3){
        opt->rooted=false;
    }
    else if (nbChild==2) {
        opt->rooted=true;
    }
    else{
        opt->rooted=false;
    }
    if (opt->rooted){
        opt->nbINodes=n;
        opt->nbBranches=m;
    }
    else{
        opt->nbINodes=n+1;
        opt->nbBranches=m+1;
    }
}

int firstCharacter(string s,int p){
    while (p<s.length() && (s.at(p)<33 || s.at(p)>126)) {
        p++;
    }
    return p;
}

int lastCharacter(string s,int p){
    while (p<s.length() && (s.at(p)>=33 && s.at(p)<=126)) {
        p++;
    }
    return p;
}

int getInternalNodeId(Pr* pr,Node** nodes,string s){
    int k = getPosition(nodes,s,0,pr->nbBranches+1);
    if (k==-1 && (s.substr(0,4).compare("mrca")==0)){
        vector<int> mr;
        char c='(';
        int p = 5;
        while (c!=')'){
            int newp = s.find_first_of(",)",p);
            if (newp==-1){
                cout<<s<<": wrong format"<<endl;
                exit(EXIT_FAILURE);
            }
            c=s.at(newp);
            string s1 = s.substr(p,newp-p);
            p = newp+1;
            int k1=getPosition(nodes,s1,0,pr->nbBranches+1);
            if (k1!=-1){
                mr.push_back(k1);
            }
            else{
                cout<<"taxa "<<s1<<" not found"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        if (mr.size()>0){ k=mrca(nodes,mr);}
    }
    return k;
}

int assignRecursive(int r,Node** nodes,int g){
    vector<int> children = nodes[r]->suc;
    int s = 0;
    for (int i=0; i<children.size(); i++) {
        if (nodes[children[i]]->rateGroup!=-1 && nodes[children[i]]->rateGroup!=g) {
            nodes[children[i]]->rateGroup = g;
            s+=1+assignRecursive(children[i],nodes,g);
        }
    }
    return s;
}

int assignRateGroupToSubTree(Subtree* subtree,Pr* pr,Node** nodes,int g){
    Pair* root = subtree->root;
    int r = getInternalNodeId(pr,nodes,root->name);
    int s = 0;
    for (int i=0; i<subtree->tips.size(); i++) {
        Pair* tip = subtree->tips[i];
        int t = getInternalNodeId(pr,nodes,tip->name);
        nodes[t]->rateGroup = g;
        s++;
    }
    if (root->include) {
        s += assignRecursive(r,nodes,g);
    }
    else{
        vector<int> children = nodes[r]->suc;
        for (int k=0; k<children.size(); k++) {
            if (nodes[children[k]]->rateGroup!=g) {
                s += assignRecursive(children[k],nodes,g);
            }
        }
    }
    return s;
}

void assignRateGroupToTree(Pr* pr,Node** nodes){
    vector<int> subroot;
    
    for (int i=0; i<pr->ratePartition.size(); i++) {
        Part* group = pr->ratePartition[i];
        for (int j=0; j<group->subtrees.size(); j++) {
            Pair* root = group->subtrees[j]->root;
            int r = getInternalNodeId(pr,nodes,root->name);
            if (contain(r,subroot)) {
                cout<<"Warning: "<<group->name<<" there are overlapped subtrees in the partition file"<<endl;
            }
            else{
                subroot.push_back(r);
            }
            if (root->include) {
                nodes[r]->rateGroup = -1;
            }
            else{
                vector<int> children = nodes[r]->suc;
                for (int k=0; k<children.size(); k++) {
                    nodes[children[k]]->rateGroup = -1;
                }
            }
        }
    }
    double nbBranchesPartition = 0;
    for (int i=0; i<pr->ratePartition.size(); i++) {
        Part* group = pr->ratePartition[i];
        for (int j=0; j<group->subtrees.size(); j++) {
            Subtree* subtree = group->subtrees[j];
            nbBranchesPartition += assignRateGroupToSubTree(subtree,pr,nodes,i+1);
        }
    }
    for (int i=0; i<pr->ratePartition.size(); i++) {
        Part* group = pr->ratePartition[i];
        for (int j=0; j<group->subtrees.size(); j++) {
            Pair* root = group->subtrees[j]->root;
            int r = getInternalNodeId(pr,nodes,root->name);
            if (root->include) {
                nodes[r]->rateGroup = i+1;
                nbBranchesPartition ++;
            }
            else{
                vector<int> children = nodes[r]->suc;
                for (int k=0; k<children.size(); k++) {
                    if (nodes[children[k]]->rateGroup != (i+1)) {
                        nodes[children[k]]->rateGroup = i+1;
                        nbBranchesPartition ++;
                    }
                }
            }
        }
    }
    pr->multiplierRate = new double[1+pr->ratePartition.size()];
    for (int i=0; i<=pr->ratePartition.size(); i++) {
        pr->multiplierRate[i]=1;
    }
    if (nbBranchesPartition==pr->nbBranches) {
        pr->multiplierRate[0]=-1;//Full partition
    }
}
