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
#include "estimate_root.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////estimate root without constraint (by using LD algorithm)///////////////////////////////////////////

list<double> without_constraint_lambda(double br,Pr* par,Node** nodes,list<int> active_set){
    //P: rooted tree
    //compute optimized solution without constraint (LD algorithm) with the position of root
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int r=(*iter);//r=r1
    iter++;
    int pr=(*iter);//pr=r2
    if (br==0) {
        nodes[r]->B=0;
        nodes[pr]->B=0;
        without_constraint(par,nodes);
        return computeLambda(active_set,par,nodes);
    }
    else{
        list<int> pos = postorder_polytomy(par,nodes);
        double *W= new double[par->nbINodes];
        double *C = new double[par->nbINodes];
        double *X = new double[par->nbINodes];//nodes[i]->D=W[i].T[a(i)]+C[i]+X[i]/par->rho
        for (list<int>::iterator iter=pos.begin();iter!=pos.end();iter++){
            int i =  *iter;
            if (leaf(nodes[i])) {
                W[i]=0;
                C[i]=nodes[i]->D;
                X[i]=0;
            }
            else{
                if (i==0){
                    if (leaf(nodes[r]) && leaf(nodes[pr])){
                        C[i]=(nodes[r]->D+nodes[pr]->D)/2;
                        X[i]=-br/2.;
                    }
                    else if (leaf(nodes[pr])){
                        if (r<par->nbINodes){
                            C[i]=(C[r]+nodes[pr]->D)/2.;
                            X[i]=(X[r]-br)/2.;
                        }
                        else{
                            C[i]=nodes[pr]->D;
                            X[i]=-br/2.;
                        }
                    }
                    else if (leaf(nodes[r])){
                        if (pr<par->nbINodes){
                            C[i]=(C[pr]+nodes[r]->D)/2;
                            X[i]=(X[pr]-br)/2.;
                        }
                        else{
                            C[i]=nodes[r]->D;
                            X[i]=-br/2.;
                        }
                    }
                    else{
                        if (r<par->nbINodes && pr<par->nbINodes){
                            C[i]=(C[r]+C[pr])/2;
                            X[i]=(X[r]+X[pr]-br)/2.;
                        }
                        else if (r>=par->nbINodes){
                            C[i]=nodes[pr]->D;
                            X[i]=-br/2.;
                        }
                        else{
                            C[i]=nodes[r]->D;
                            X[i]=-br/2.;
                        }
                    }
                    
                }
                else if (i==r || i==pr){
                    int nc=r;
                    if (i==r){
                        nc=pr;
                    }
                    double coefs=0;
                    double xtemp=0;
                    double ctemp=0;
                    double wtemp=0;
                    if (leaf(nodes[0]) && leaf(nodes[nc])){
                        coefs=1/nodes[r]->V;
                        ctemp=(2*nodes[0]->D-nodes[nc]->D)/nodes[r]->V;
                        xtemp=br/nodes[r]->V;
                    }
                    else if (leaf(nodes[0]) && nc<par->nbINodes){
                        coefs=1/nodes[r]->V;
                        ctemp=2*nodes[0]->D/nodes[r]->V;
                        xtemp=br/nodes[r]->V;
                        wtemp=-1/nodes[r]->V;
                    }
                    for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
                        if (leaf(nodes[*iter])) {
                            coefs+=1/nodes[*iter]->V;
                            ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                            xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                        }
                        else if (*iter<par->nbINodes){
                            coefs+=(1-W[*iter])/nodes[*iter]->V;
                            ctemp+=C[*iter]/nodes[*iter]->V;
                            xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                        }
                    }
                    if (coefs==0) {
                        cout<<"The input temporal constraints are not sufficient to have unique solution. Please either add more temporal constraints or ignore date file to estimate relative dates"<<endl;
                        exit(EXIT_FAILURE);
                    }
                    C[i]=ctemp/coefs;
                    X[i]=xtemp/coefs;
                    W[i]=wtemp/coefs;
                }
                
                
                else{
                    int p = nodes[i]->P;
                    double coefs=1./nodes[i]->V;
                    double wtemp=1./nodes[i]->V;
                    double ctemp=0;
                    double xtemp=nodes[i]->B/nodes[i]->V;
                    for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
                        if (leaf(nodes[*iter])) {
                            coefs+=1/nodes[*iter]->V;
                            ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                            xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                        }
                        else if (*iter<par->nbINodes){
                            coefs+=(1-W[*iter])/nodes[*iter]->V;
                            ctemp+=C[*iter]/nodes[*iter]->V;
                            xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                        }
                        
                    }
                    if (!leaf(nodes[p])){
                        W[i]=wtemp/coefs;
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                    else{
                        ctemp+=nodes[p]->D/nodes[i]->V;
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                }
            }
            
        }
        
        list<int> pre = preorder_polytomy(par,nodes);
        
        for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
            int i = *iter;
            if (leaf(nodes[i])){
                C[i]=nodes[i]->D;
                X[i]=0;
            }
            else if ((i==r || i==pr) && leaf(nodes[0])){
                int nc=r;
                if (i==r) nc=pr;
                if (nc<par->nbINodes){
                    C[i]=(C[i]+W[i]*C[nc])/(1-W[i]*W[nc]);
                    X[i]=(X[i]+W[i]*X[nc])/(1-W[i]*W[nc]);
                    W[i]=0;
                }
            }
            else if (i!=0){
                int p = nodes[i]->P;
                if (!leaf(nodes[p])){
                    C[i]=W[i]*C[p]+C[i];
                    X[i]=W[i]*X[p]+X[i];
                }
            }
        }
        if (!par->givenRate){
            double *F= new double[par->nbBranches+1];//
            double *G = new double[par->nbBranches+1];//
            if (!par->givenRate){
                F[0]=-2*C[0];
                G[0]=-2*X[0];
                for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
                    int i = *iter;
                    if (i==r || i==pr){
                        F[0]+=C[i];
                        G[0]+=X[i];
                    }
                    else if (i!=0){
                        F[i] = C[i]-C[nodes[i]->P];
                        G[i] = X[i]-X[nodes[i]->P];
                    }
                }
                for (int i=par->nbINodes;i<=par->nbBranches;i++){
                    if (leaf(nodes[i])) {
                        if (i==r || i==pr){
                            F[0]+=nodes[i]->D;
                        }
                        else{
                            F[i]=nodes[i]->D-C[nodes[i]->P];
                            G[i]=-X[nodes[i]->P];
                        }
                    }
                    else if (i==r || i==pr){
                        F[0]=0;
                        G[0]=0;
                    }
                }
                double a = 0;
                double b = 0;
                double c = 0;//Fi^2w^2 +2w*FiGi-wFibi-Gibi+Gi^2 =0
                for (int i=0;i<=par->nbBranches;i++){
                    if (i==0){
                        a += F[i]*F[i]/nodes[r]->V;
                        b += (2*F[i]*G[i]-F[i]*br)/nodes[r]->V;
                        c += (G[i]*G[i]-G[i]*br)/nodes[r]->V;
                    }
                    else if (i!=r && i!=pr && (i<par->nbINodes || leaf(nodes[i]))){
                        a += F[i]*F[i]/nodes[i]->V;
                        b += (2*F[i]*G[i]-F[i]*nodes[i]->B)/nodes[i]->V;
                        c += (G[i]*G[i]-G[i]*nodes[i]->B)/nodes[i]->V;
                    }
                }
                double delta = b*b-4*a*c;
                par->rho=(-b+sqrt(delta))/2/a;
                if (delta<0 || par->rho<par->rho_min) par->rho=par->rho_min;
                delete[] F;
                delete[] G;
            }
        }
        for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
            int i = *iter;
            nodes[i]->D = C[i]+X[i]/par->rho;
        }
        for (int i=par->nbINodes; i<=par->nbBranches; i++) {
            if (!leaf(nodes[i])) {
                if (i==r || i==pr){
                    int nc=pr;
                    if (i==pr) nc=r;
                    nodes[i]->D=br/par->rho-nodes[nc]->D+2*nodes[0]->D;
                }
                else {
                    nodes[i]->D=nodes[nodes[i]->P]->D+nodes[i]->B/par->rho;
                }
            }
        }
        computeObjectiveEstimateRoot(r, pr, br, par, nodes);
        delete[] W;
        delete[] C;
        delete[] X;
        
        int *as=new int[par->nbBranches+1];
        for (int i=0;i<=par->nbBranches;i++) as[i]=-1;
        int count = 0;
        for (list<int>::iterator iter = active_set.begin();iter!=active_set.end();iter++){
            int a = *iter;
            as[a]=count;
            count++;
        }
        double* lambda = new double[count];
        for (list<int>::iterator iter = active_set.begin();iter!=active_set.end();iter++){
            int i=*iter;
            double lambdaPC=0;
            if (i==0){
                lambdaPC=4*par->rho*(br-par->rho*nodes[r]->D-par->rho*nodes[pr]->D+2*par->rho*nodes[0]->D)/nodes[r]->V;
            }
            else{
                if (i<par->nbINodes) {
                    for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
                        int s=*iter;
                        lambdaPC+=2*par->rho*(nodes[s]->B-par->rho*nodes[s]->D+par->rho*nodes[i]->D)/nodes[s]->V;
                    }
                }
                if (i!=r && i!=pr)
                    lambdaPC-=2*par->rho*(nodes[i]->B-par->rho*nodes[i]->D+par->rho*nodes[nodes[i]->P]->D)/nodes[i]->V;
                else
                    lambdaPC-=2*par->rho*(br-par->rho*nodes[r]->D-par->rho*nodes[pr]->D+2*par->rho*nodes[0]->D)/nodes[i]->V;
            }
            if (lower(nodes[i])){
                lambda[as[i]]=lambdaPC;
            }
            else if (upper(nodes[i])){
                lambda[as[i]]=-lambdaPC;
            }
        }
        list<double> ld;
        for (int i=0;i<active_set.size();i++){
            if (myabs(lambda[i])<(10e-15))
                lambda[i]=0;
            ld.push_back(lambda[i]);
        }
        
        /*    double sr=(2*nodes[0]->D-nodes[r]->D-nodes[pr]->D)*(br-par->rho*(nodes[r]->D+nodes[pr]->D-2*nodes[0]->D))/nodes[r]->V;
         for (int i=0; i<=par->nbBranches; i++) {
         if (i>0 && i!=r && i!=pr){
         sr+=(nodes[nodes[i]->P]->D-nodes[i]->D)*(nodes[i]->B-par->rho*(nodes[i]->D-nodes[nodes[i]->P]->D))/nodes[i]->V;
         }
         if ((nodes[i])->type!='p') {
         double s=0;
         if (lower(nodes[i])){
         s-=lambda[as[i]];
         }
         if (upper(nodes[i])){
         s+=lambda[as[i]];
         }
         if (i==0){
         s+=4*par->rho*(br-par->rho*(nodes[r]->D+nodes[pr]->D-2*nodes[0]->D))/nodes[r]->V;
         }
         else if (i==r || i==pr){
         s-=2*par->rho*(br-par->rho*(nodes[r]->D+nodes[pr]->D-2*nodes[0]->D))/nodes[i]->V;
         for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
         int s1=*iter;
         s+=2*par->rho*(nodes[s1]->B-par->rho*nodes[s1]->D+par->rho*nodes[i]->D)/nodes[s1]->V;
         }
         }
         else{
         s-=2*par->rho*(nodes[i]->B-par->rho*(nodes[i]->D-nodes[nodes[i]->P]->D))/nodes[i]->V;
         for (vector<int>::iterator iter = nodes[i]->suc.begin();iter!=nodes[i]->suc.end();iter++){
         int s1=*iter;
         s+=2*par->rho*(nodes[s1]->B-par->rho*nodes[s1]->D+par->rho*nodes[i]->D)/nodes[s1]->V;
         }
         }
         if (myabs(s)>1e-6) {
         cout<<"TEST PROBLEM "<<i<<" "<<s<<" "<<nodes[i]->P<<endl;
         }
         }
         }
         if (myabs(sr)>1e-5 && par->rho!=par->rho_min) {
         cout<<"TEST PROBLEM rho "<<sr<<endl;
         }*/
        delete[] lambda;
        delete[] as;
        return ld;
    }
}

bool starting_point_without_constraint_lambda(double br,Pr* pr,Node** nodes,list<int> &active_set){
    for (vector<int>::iterator iter=nodes[0]->suc.begin(); iter!=nodes[0]->suc.end(); iter++) {
        nodes[*iter]->B=br/2.;
    }
    without_constraint(pr,nodes);
    double* lowerX = new double[pr->nbBranches+1];
    bool* bl = new bool[pr->nbBranches+1];
    list<int> pre = preorder_polytomy(pr,nodes);
    for (int i=0;i<=pr->nbBranches;i++){
        if (nodes[i]->type=='l' || nodes[i]->type=='b'){
            bl[i]=true;
            lowerX[i]=nodes[i]->lower;
        }
        else if (nodes[i]->type=='p'){
            bl[i]=true;
            lowerX[i]=nodes[i]->D;
        }
        else bl[i]=false;
    }
    for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
        int i=*iter;
        if (bl[i]){
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                int s=*iter;
                if (!bl[s] || (nodes[s]->type=='l' && (nodes[s]->lower<lowerX[i]))){
                    lowerX[s]=lowerX[i];
                    nodes[s]->lower=lowerX[i];
                    bl[s]=true;
                }
                else if (nodes[s]->type=='u' && nodes[s]->upper<lowerX[i]){
                    return false;
                }
                else if (nodes[s]->type=='b'){
                    if (nodes[s]->upper>=lowerX[i]){
                        if (nodes[s]->lower<lowerX[i]){
                            lowerX[s]=lowerX[i];
                            nodes[s]->lower=lowerX[i];
                            bl[s]=true;
                        }
                    }
                    else {
                        return false;
                    }
                }
                else if ((nodes[s]->type=='p') && nodes[s]->D<lowerX[i]){
                    return false;
                }
                if (nodes[s]->D<lowerX[s]){
                    nodes[s]->D=lowerX[s];
                }
            }
        }
    }
    delete[] bl;
    delete[] lowerX;
    for (int i =0;i<pr->nbINodes;i++) {
        if (lower(nodes[i]) || upper(nodes[i])) {
            active_set.push_back(i);
        }
        else if (nodes[i]->type!='p') {
            if ((nodes[i]->type=='l' || nodes[i]->type=='b') && nodes[i]->D<nodes[i]->lower) {
                activeLower(nodes[i]);
                nodes[i]->D=nodes[i]->lower;
                active_set.push_back(i);
            }
            else if ((nodes[i]->type=='u' || nodes[i]->type=='b') && nodes[i]->D>nodes[i]->upper) {
                activeUpper(nodes[i]);
                nodes[i]->D=nodes[i]->upper;
                active_set.push_back(i);
            }
        }
    }
    for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
        if (lower(nodes[i]) || upper(nodes[i])) {
            active_set.push_back(i);
        }
        else if (nodes[i]->type!='p') {
            if ((nodes[i]->type=='l' || nodes[i]->type=='b') && nodes[i]->D<nodes[i]->lower) {
                activeLower(nodes[i]);
                nodes[i]->D=nodes[i]->lower;
                active_set.push_back(i);
            }
            else if ((nodes[i]->type=='u' || nodes[i]->type=='b') && nodes[i]->D>nodes[i]->upper) {
                activeUpper(nodes[i]);
                nodes[i]->D=nodes[i]->upper;
                active_set.push_back(i);
            }
        }
    }
    return true;
}

bool without_constraint_active_set_lambda(double br,Pr* pr,Node** nodes){
    initialize_status(pr,nodes);
    list<int> active_set;
    bool consistent=starting_point_without_constraint_lambda(br,pr,nodes,active_set);
    if (consistent) {
        double* D_old = new double[pr->nbBranches+1];
        for (int i=0; i<=pr->nbBranches; i++) {
            D_old[i]=nodes[i]->D;
        }
        list<double> lambda = without_constraint_lambda(br,pr,nodes,active_set);
        int nb_iter=0;
        double alpha;
        double* dir = new double[pr->nbBranches+1];
        while (!conditions(lambda,pr,nodes) && nb_iter<=maxIter){
            for (int i=0;i<=pr->nbBranches;i++)  {dir[i]=nodes[i]->D-D_old[i];}
            alpha=1;
            int as=0;
            double a;
            for (int i=0;i<=pr->nbBranches;i++){
                if (nodes[i]->status==0 && (nodes[i]->type=='l' || nodes[i]->type=='u' || nodes[i]->type=='b')){
                    if (dir[i]<0 && (nodes[i]->type=='l' || nodes[i]->type=='b') ){
                        a = (nodes[i]->lower-D_old[i])/dir[i];
                        if (a<alpha){
                            alpha = a;
                            as = i+pr->nbBranches+1;
                        }
                    }
                    if (dir[i]>0 && (nodes[i]->type=='u' || nodes[i]->type=='b')){
                        a = (nodes[i]->upper-D_old[i])/dir[i];
                        if (a<alpha){
                            alpha = a;
                            as = i+2*pr->nbBranches+2;
                        }
                    }
                }
            }
            int asrm;
            if (remove_ne_lambda(lambda,active_set,asrm)){
                active_set.remove(asrm);
                desactive(nodes[asrm]);
            }
            for (int i=0;i<=pr->nbBranches;i++) D_old[i]=D_old[i]+alpha*dir[i];
            if (as!=0) {
                if (as>2*pr->nbBranches+1) {
                    active_set.push_back(as-2*pr->nbBranches-2);
                    activeUpper(nodes[as-2*pr->nbBranches-2]);
                    nodes[as-2*pr->nbBranches-2]->D=nodes[as-2*pr->nbBranches-2]->upper;
                }
                else {
                    active_set.push_back(as-pr->nbBranches-1);
                    activeLower(nodes[as-pr->nbBranches-1]);
                    nodes[as-pr->nbBranches-1]->D=nodes[as-pr->nbBranches-1]->lower;
                }
            }
            lambda=without_constraint_lambda(br,pr,nodes,active_set);
            nb_iter++;
        }
        if (nb_iter>maxIter){
            for (int i=0;i<=pr->nbBranches;i++) nodes[i]->D=D_old[i];
        }
        delete[] D_old;
        delete[] dir;
        return true;
    }
    return false;
}


void without_constraint_active_set_lambda_secondTime(double br,Pr* pr,Node** nodes){
    list<int> active_set=getActiveSet(pr, nodes);
    computeNewVarianceEstimateRoot(pr, nodes);
    double* D_old = new double[pr->nbBranches+1];
    for (int i=0; i<=pr->nbBranches; i++) {
        D_old[i]=nodes[i]->D;
    }
    list<double> lambda = without_constraint_lambda(br,pr,nodes,active_set);
    int nb_iter=0;
    double alpha;
    double* dir = new double[pr->nbBranches+1];
    while (!conditions(lambda,pr,nodes) && nb_iter<=maxIter){
        for (int i=0;i<=pr->nbBranches;i++)  {dir[i]=nodes[i]->D-D_old[i];}
        alpha=1;
        int as=0;
        double a;
        for (int i=0;i<=pr->nbBranches;i++){
            if (nodes[i]->status==0 && (nodes[i]->type=='l' || nodes[i]->type=='u' || nodes[i]->type=='b')){
                if (dir[i]<0 && (nodes[i]->type=='l' || nodes[i]->type=='b') ){
                    a = (nodes[i]->lower-D_old[i])/dir[i];
                    if (a<alpha){
                        alpha = a;
                        as = i+pr->nbBranches+1;
                    }
                }
                if (dir[i]>0 && (nodes[i]->type=='u' || nodes[i]->type=='b')){
                    a = (nodes[i]->upper-D_old[i])/dir[i];
                    if (a<alpha){
                        alpha = a;
                        as = i+2*pr->nbBranches+2;
                    }
                }
            }
        }
        int asrm;
        if (remove_ne_lambda(lambda,active_set,asrm)){
            active_set.remove(asrm);
            desactive(nodes[asrm]);
        }
        for (int i=0;i<=pr->nbBranches;i++) D_old[i]=D_old[i]+alpha*dir[i];
        if (as!=0) {
            if (as>2*pr->nbBranches+1) {
                active_set.push_back(as-2*pr->nbBranches-2);
                activeUpper(nodes[as-2*pr->nbBranches-2]);
                nodes[as-2*pr->nbBranches-2]->D=nodes[as-2*pr->nbBranches-2]->upper;
            }
            else {
                active_set.push_back(as-pr->nbBranches-1);
                activeLower(nodes[as-pr->nbBranches-1]);
                nodes[as-pr->nbBranches-1]->D=nodes[as-pr->nbBranches-1]->lower;
            }
        }
        lambda=without_constraint_lambda(br,pr,nodes,active_set);
        nb_iter++;
    }
    if (nb_iter>maxIter){
        for (int i=0;i<=pr->nbBranches;i++) nodes[i]->D=D_old[i];
    }
    delete[] D_old;
    delete[] dir;
}

list<double> with_constraint_lambda(double br,Pr* pr,Node** nodes,list<int> active_set){
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int r=(*iter);//r=r1
    iter++;
    int p_r=(*iter);//pr=r2
    if (br==0) {
        nodes[0]->B=0;
        nodes[p_r]->B=0;
        return with_constraint(pr,nodes,active_set);
    }
    else {
        list<double> ld;
        list<int>* Suc = new list<int>[pr->nbINodes];
        int* Pre=new int[pr->nbBranches+1];
        for (int i=0;i<=pr->nbBranches;i++){
            Pre[i]=-1;
            if (leaf(nodes[i])) activeMarkLeaf(nodes[i]);
            else desactiveMarkLeaf(nodes[i]);
        }
        list<int> ls;
        for (list<int>::iterator iter = active_set.begin();iter!=active_set.end();iter++){
            int i = *iter;
            if (i>0) {//time constraint
                if (leaf(nodes[i])) {
                    ls.push_back(i);
                }
            }
            else if (!tc(nodes[-i]) && leaf(nodes[-i])) ls.push_back(-i);//only limit constraint
        }
        stack<int>* feuilles = computeFeuilles_polytomy(ls,pr,nodes);
        list<int> top;
        for (int i=0;i<pr->nbINodes;i++){
            bool bl=false;
            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                if (tc(nodes[*iter])) {
                    bl=true;
                }
            }
            if (!tc(nodes[i]) && (!markLeaf(nodes[i]) || nodes[i]->type=='p')  && bl){
                top.push_back(i);
            }
        }
        list<int>* internal = new list<int>[top.size()];
        reduceTree_polytomy(pr,nodes,Pre,Suc,internal);
        list<int> pos = postorder_polytomy(pr,nodes);
        list<int> pre = preorder_polytomy(pr,nodes);
        double *W= new double[pr->nbINodes];//nodes[i]->D=W[i].T[a(i)]+C[i]+X[i]/rho
        double *C = new double[pr->nbINodes];
        double *X =new double[pr->nbINodes];
        for (int i=0;i<pr->nbINodes;i++){
            if (markLeaf(nodes[i])) C[i]=nodes[i]->D;
            else C[i]=0;
            W[i]=0;X[i]=0;
        }
        for (list<int>::iterator it = pos.begin();it!=pos.end();it++){
            int i = *it;
            if (!tc(nodes[i]) && !markLeaf(nodes[i])){
                int p = Pre[i];
                if (p==-1){
                    if (tc(nodes[r]) && tc(nodes[p_r])){
                        double coefs=0;
                        double ctemp=0;
                        double xtemp=0;
                        for (list<int>::iterator iter=Suc[i].begin(); iter!=Suc[i].end(); iter++) {
                            if (markLeaf(nodes[*iter])) {
                                coefs+=1/nodes[*iter]->V;
                                ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                                xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                            }
                            else {
                                coefs+=(1-W[*iter])/nodes[*iter]->V;
                                ctemp+=C[*iter]/nodes[*iter]->V;
                                xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                            }
                        }
                        if (coefs==0) {
                            cout<<"The input temporal constraints are not sufficient to have unique solution. Please either add more temporal constraints or ignore date file to estimate relative dates"<<endl;
                            exit(EXIT_FAILURE);
                        }
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                    else if (tc(nodes[r]) || tc(nodes[p_r])){//(br+rho*T0-rho*Tr)-sum(Ts-rho*(Ts-T0))=0 ### to check
                        int nc=r;
                        if (tc(nodes[r])){
                            nc=p_r;
                        }
                        double coefs=1/nodes[nc]->V;//(1-W[nc])/nodes[nc]->V;
                        double ctemp=0;//C[nc]/nodes[nc]->V;
                        double xtemp=-br/nodes[nc]->V;//(-br+X[nc])/nodes[nc]->V;
                        if (markLeaf(nodes[nc])){
                            ctemp+=nodes[nc]->D/nodes[nc]->V;
                        }
                        else if (nc<pr->nbINodes){
                            coefs-=W[nc]/nodes[nc]->V;
                            ctemp+=C[nc]/nodes[nc]->V;
                            xtemp+=X[nc]/nodes[nc]->V;
                        }
                        else{
                            coefs=0;
                            ctemp=0;
                            xtemp=0;
                        }
                        for (list<int>::iterator iter=Suc[i].begin();iter!=Suc[i].end();iter++){
                            if (markLeaf(nodes[*iter])) {
                                if (*iter!=nc){
                                    coefs+=1/nodes[*iter]->V;
                                    ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                                    xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                                }
                            }
                            else{
                                if (*iter!=nc){
                                    coefs+=(1-W[*iter])/nodes[*iter]->V;
                                    ctemp+=C[*iter]/nodes[*iter]->V;
                                    xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                                }
                            }
                        }
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                    else {
                        if (markLeaf(nodes[r]) && markLeaf(nodes[p_r])){
                            C[i]=(nodes[r]->D+nodes[p_r]->D)/2;
                            X[i]=-br/2.;
                        }
                        else if (markLeaf(nodes[p_r])){
                            if (r<pr->nbINodes){
                                C[i]=(C[r]+nodes[p_r]->D)/2.;
                                X[i]=(X[r]-br)/2.;
                            }
                            else{
                                C[i]=nodes[p_r]->D;
                                X[i]=-br/2.;
                            }
                        }
                        else if (markLeaf(nodes[r])){
                            if (p_r<pr->nbINodes){
                                C[i]=(C[p_r]+nodes[r]->D)/2;
                                X[i]=(X[p_r]-br)/2.;
                            }
                            else{
                                C[i]=nodes[r]->D;
                                X[i]=-br/2.;
                            }
                        }
                        else{
                            if (r<pr->nbINodes && p_r<pr->nbINodes){
                                C[i]=(C[r]+C[p_r])/2;
                                X[i]=(X[r]+X[p_r]-br)/2.;
                            }
                            else if (r>=pr->nbINodes){
                                C[i]=nodes[p_r]->D;
                                X[i]=-br/2.;
                            }
                            else{
                                C[i]=nodes[r]->D;
                                X[i]=-br/2.;
                            }
                        }
                    }
                }
                else if (i==r || i==p_r){
                    if (tc(nodes[r]) || tc(nodes[p_r])){
                        double coefs=1./nodes[r]->V;
                        double wtemp=1./nodes[r]->V;
                        double ctemp=0;
                        double xtemp=br/nodes[r]->V;
                        for (list<int>::iterator iter = Suc[i].begin();iter!=Suc[i].end();iter++){
                            if (markLeaf(nodes[*iter])) {
                                coefs+=1/nodes[*iter]->V;
                                ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                                xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                            }
                            else{
                                coefs+=(1-W[*iter])/nodes[*iter]->V;
                                ctemp+=C[*iter]/nodes[*iter]->V;
                                xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                            }
                        }
                        W[i]=wtemp/coefs;
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                        if (markLeaf(nodes[0])){
                            C[i]+=W[i]*nodes[0]->D;
                            W[i]=0;
                        }
                    }
                    else {
                        int nc=r;
                        if (i==r){
                            nc=p_r;
                        }
                        double coefs=0;
                        double xtemp=0;
                        double ctemp=0;
                        double wtemp=0;
                        if (markLeaf(nodes[0]) && markLeaf(nodes[nc])){
                            coefs=1/nodes[r]->V;
                            ctemp=(2*nodes[0]->D-nodes[nc]->D)/nodes[r]->V;
                            xtemp=br/nodes[r]->V;
                        }
                        else if (markLeaf(nodes[0]) && nc<pr->nbINodes){
                            coefs=1/nodes[r]->V;
                            ctemp=2*nodes[0]->D/nodes[r]->V;
                            xtemp=br/nodes[r]->V;
                            wtemp=-1/nodes[r]->V;
                        }
                        for (list<int>::iterator iter = Suc[i].begin();iter!=Suc[i].end();iter++){
                            if (markLeaf(nodes[*iter])) {
                                coefs+=1/nodes[*iter]->V;
                                ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                                xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                            }
                            else{
                                coefs+=(1-W[*iter])/nodes[*iter]->V;
                                ctemp+=C[*iter]/nodes[*iter]->V;
                                xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                            }
                        }
                        if (coefs==0) {
                            cout<<"The input temporal constraints are not sufficient to have unique solution. Please either add more temporal constraints or ignore date file to estimate relative dates"<<endl;
                            exit(EXIT_FAILURE);
                        }
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                        W[i]=wtemp/coefs;
                    }
                }
                else{
                    double coefs=1./nodes[i]->V;
                    double wtemp=1./nodes[i]->V;
                    double ctemp=0;
                    double xtemp=nodes[i]->B/nodes[i]->V;
                    for (list<int>::iterator iter = Suc[i].begin();iter!=Suc[i].end();iter++){
                        if (markLeaf(nodes[*iter])) {
                            coefs+=1/nodes[*iter]->V;
                            ctemp+=nodes[*iter]->D/nodes[*iter]->V;
                            xtemp-=nodes[*iter]->B/nodes[*iter]->V;
                        }
                        else{
                            coefs+=(1-W[*iter])/nodes[*iter]->V;
                            ctemp+=C[*iter]/nodes[*iter]->V;
                            xtemp+=(X[*iter]-nodes[*iter]->B)/nodes[*iter]->V;
                        }
                    }
                    if (!markLeaf(nodes[p])){
                        W[i]=wtemp/coefs;
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                    else{
                        ctemp+=nodes[p]->D/nodes[i]->V;
                        C[i]=ctemp/coefs;
                        X[i]=xtemp/coefs;
                    }
                }
            }
        }
        for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
            int i = *iter;
            if (markLeaf(nodes[i])){
                C[i]=nodes[i]->D;
                X[i]=0;
            }
            else if (tc(nodes[i])){
                C[i]=C[nodes[i]->P];
                X[i]=X[nodes[i]->P];
            }
            else if ((i==r || i==p_r) && markLeaf(nodes[0])){
                int nc=r;
                if (i==r) nc=p_r;
                if (nc<pr->nbINodes){
                    C[i]=(C[i]+W[i]*C[nc])/(1-W[i]*W[nc]);
                    X[i]=(X[i]+W[i]*X[nc])/(1-W[i]*W[nc]);
                    W[i]=0;
                }
            }
            else if (i!=0 && !markLeaf(nodes[Pre[i]])){
                C[i]=W[i]*C[Pre[i]]+C[i];
                X[i]=W[i]*X[Pre[i]]+X[i];
            }
        }
        double *F= new double[pr->nbBranches+1];//
        double *G = new double[pr->nbBranches+1];//
        if (!pr->givenRate){
            F[0]=-2*C[0];
            G[0]=-2*X[0];
            for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
                int i = *iter;
                if (i==r || i==p_r){
                    F[0]+=C[i];
                    G[0]+=X[i];
                }
                else if (i!=0){
                    F[i] = C[i]-C[nodes[i]->P];
                    G[i] = X[i]-X[nodes[i]->P];
                }
            }
            for (int i=pr->nbINodes;i<=pr->nbBranches;i++){
                if (markLeaf(nodes[i])) {
                    if (i==r || i==p_r){
                        F[0]+=nodes[i]->D;
                    }
                    else{
                        F[i]=nodes[i]->D-C[nodes[i]->P];
                        G[i]=-X[nodes[i]->P];
                    }
                }
                else if (i==r || i==p_r){
                    F[0]=0;
                    G[0]=0;
                }
            }
            double a = 0;
            double b = 0;
            double c = 0;//Fi^2w^2 +2w*FiGi-wFibi-Gibi+Gi^2 =0
            for (int i=0;i<=pr->nbBranches;i++){
                if (i==0){
                    a += F[i]*F[i]/nodes[r]->V;
                    b += (2*F[i]*G[i]-F[i]*br)/nodes[r]->V;
                    c += (G[i]*G[i]-G[i]*br)/nodes[r]->V;
                }
                else if (i!=r && i!=p_r && (i<pr->nbINodes || markLeaf(nodes[i]))){
                    a += F[i]*F[i]/nodes[i]->V;
                    b += (2*F[i]*G[i]-F[i]*nodes[i]->B)/nodes[i]->V;
                    c += (G[i]*G[i]-G[i]*nodes[i]->B)/nodes[i]->V;
                }
            }
            double delta = b*b-4*a*c;
            pr->rho=(-b+sqrt(delta))/2/a;
            if (delta<0 || pr->rho<pr->rho_min) pr->rho=pr->rho_min;
            delete[] F;
            delete[] G;
        }
        for (list<int>::iterator iter = pre.begin();iter!=pre.end();iter++){
            int i = *iter;
            nodes[i]->D = C[i]+X[i]/pr->rho;
        }
        for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
            if (!markLeaf(nodes[i])) {
                if (tc(nodes[i])) {
                    nodes[i]->D=nodes[nodes[i]->P]->D;
                }
                else{
                    if (i==r || i==p_r){
                        int nc=p_r;
                        if (i==p_r) nc=r;
                        nodes[i]->D=br/pr->rho-nodes[nc]->D+2*nodes[0]->D;
                    }
                    else {
                        nodes[i]->D=nodes[nodes[i]->P]->D+nodes[i]->B/pr->rho;
                    }
                }
            }
        }
        computeObjectiveEstimateRoot(r, p_r, br, pr, nodes);
        delete[] W;
        delete[] C;
        delete[] X;
        int *as=new int[2*pr->nbBranches+2];
        for (int i=0;i<=2*pr->nbBranches+1;i++) as[i]=-1;
        int count = 0;
        for (list<int>::iterator iter = active_set.begin();iter!=active_set.end();iter++){
            int a = *iter;
            if (a>0) {
                as[a]=count;
            }
            else{
                as[-a+pr->nbBranches+1]=count;
            }
            count++;
        }
        bool *bl=new bool[pr->nbBranches+1];
        for (int i=0;i<=pr->nbBranches;i++) bl[i]=false;
        double* lambda = new double[count];
        for (int y=0;y<ls.size();y++){
            int i=0;
            while (!feuilles[y].empty()){
                i=feuilles[y].top();
                feuilles[y].pop();
                if (tc(nodes[i])){//compute lambda of active tc
                    int p=nodes[i]->P;
                    if (i==r || i==p_r){
                        lambda[as[i]]=-2*pr->rho*(br+2*pr->rho*nodes[0]->D-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D)/nodes[r]->V;
                    }
                    else{
                        lambda[as[i]]=-2*pr->rho*(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[p]->D)/nodes[i]->V;
                    }
                    if (i>=pr->nbINodes && !leaf(nodes[i])) bl[i]=true;
                    if (i<pr->nbINodes && !leaf(nodes[i])){
                        if (!limit(nodes[i])){
                            bl[i]=true;
                            for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                                int s=*iter;
                                if (!tc(nodes[s])||bl[s]) {
                                    lambda[as[i]]+=2*pr->rho*(nodes[s]->B-pr->rho*nodes[s]->D+pr->rho*nodes[i]->D)/nodes[s]->V;
                                    if (tc(nodes[s])) {
                                        lambda[as[i]]+=lambda[as[s]];
                                    }
                                }
                                else bl[i]=false;
                            }
                        }
                    }
                    if (!bl[i]){//top-down order
                        lambda[as[i]]=0;
                        if (p==0){
                            lambda[as[i]]=-4*pr->rho*(br+2*pr->rho*nodes[0]->D-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D)/nodes[r]->V;
                        }
                        else{
                            for (vector<int>::iterator iter=nodes[p]->suc.begin(); iter!=nodes[p]->suc.end(); iter++) {
                                int j=*iter;
                                lambda[as[i]]-=2*pr->rho*(nodes[j]->B-pr->rho*nodes[j]->D+pr->rho*nodes[p]->D)/nodes[j]->V;
                                if (j!=i && tc(nodes[j])) {
                                    lambda[as[i]]-=lambda[as[j]];
                                }
                            }
                            if (nodes[p]->P!=-1){
                                if (p==r || p==p_r){
                                    lambda[as[i]]+=2*pr->rho*(br+2*pr->rho*nodes[0]->D-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D)/nodes[r]->V;
                                }
                                else {
                                    lambda[as[i]]+=2*pr->rho*(nodes[p]->B-pr->rho*nodes[p]->D+pr->rho*nodes[nodes[p]->P]->D)/nodes[p]->V;
                                }
                                
                            }
                        }
                        if (tc(nodes[p])) {
                            lambda[as[i]]+=lambda[as[p]];
                        }
                        bl[i]=true;
                    }
                }
            }
            if (limit(nodes[i])){//compute lambda of active limit constraints
                double lambdaPC=0;
                if (i==0){
                    lambdaPC=4*pr->rho*(br-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D+2*pr->rho*nodes[0]->D)/nodes[r]->V;
                    if (tc(nodes[r])) lambdaPC+=lambda[as[r]];
                    if (tc(nodes[p_r])) lambdaPC+=lambda[as[p_r]];
                }
                else{
                    for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                        int s=*iter;
                        lambdaPC+=2*pr->rho*(nodes[s]->B-pr->rho*nodes[s]->D+pr->rho*nodes[i]->D)/nodes[s]->V;
                        if (tc(nodes[s])){
                            lambdaPC+=lambda[as[s]];
                        }
                    }
                    if (i!=r && i!=p_r)
                        lambdaPC-=2*pr->rho*(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[nodes[i]->P]->D)/nodes[i]->V;
                    else
                        lambdaPC-=2*pr->rho*(br-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D+2*pr->rho*nodes[0]->D)/nodes[i]->V;
                }
                if (tc(nodes[i])) lambdaPC-=lambda[as[i]];
                if (lower(nodes[i])){
                    lambda[as[i+pr->nbBranches+1]]=lambdaPC;
                }
                else if (upper(nodes[i])){
                    lambda[as[i+pr->nbBranches+1]]=-lambdaPC;
                }
            }
        }
        for (int y=0;y<top.size();y++){
            for (list<int>::iterator iter = internal[y].begin();iter!=internal[y].end();iter++){
                int i= *iter;
                int p = nodes[i]->P;
                if (i==r || i==p_r){
                    lambda[as[i]]=-2*pr->rho*(br+2*pr->rho*nodes[0]->D-pr->rho*nodes[r]->D-pr->rho*nodes[p_r]->D)/nodes[r]->V;
                }
                else{
                    lambda[as[i]]=-2*pr->rho*(nodes[i]->B-pr->rho*nodes[i]->D+pr->rho*nodes[p]->D)/nodes[i]->V;
                }
                if (i>=pr->nbINodes) bl[i]=true;
                for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                    int s=*iter;
                    lambda[as[i]]+=2*pr->rho*(nodes[s]->B-pr->rho*nodes[s]->D+pr->rho*nodes[i]->D)/nodes[s]->V;
                    if (tc(nodes[s])) {
                        lambda[as[i]]+=lambda[as[s]];
                    }
                }
                bl[i]=true;
            }
        }
        for (int i=0;i<active_set.size();i++){
            if (myabs(lambda[i])<(10e-15))
                lambda[i]=0;
            ld.push_back(lambda[i]);
        }
        
        /*       double sr=(2*nodes[0]->D-nodes[r]->D-nodes[p_r]->D)*(br-pr->rho*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D))/nodes[r]->V;
         for (int i=0; i<=pr->nbBranches; i++) {
         if (i>0 && i!=r && i!=p_r){
         sr+=(nodes[nodes[i]->P]->D-nodes[i]->D)*(nodes[i]->B-pr->rho*(nodes[i]->D-nodes[nodes[i]->P]->D))/nodes[i]->V;
         }
         if (nodes[i]->type!='p') {
         double s=0;
         if (tc(nodes[i])) {
         s-=lambda[as[i]];
         }
         if (lower(nodes[i])){
         s-=lambda[as[i+pr->nbBranches+1]];
         }
         if (upper(nodes[i])){
         s+=lambda[as[i+pr->nbBranches+1]];
         }
         if (i==0){
         s+=4*pr->rho*(br-pr->rho*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D))/nodes[r]->V;
         if (tc(nodes[r])) s+=lambda[as[r]];
         if (tc(nodes[p_r])) s+=lambda[as[p_r]];
         }
         else if (i==r || i==p_r){
         s-=2*pr->rho*(br-pr->rho*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D))/nodes[i]->V;
         for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
         int s1=*iter;
         s+=2*pr->rho*(nodes[s1]->B-pr->rho*nodes[s1]->D+pr->rho*nodes[i]->D)/nodes[s1]->V;
         if (tc(nodes[s1])) {
         s+=lambda[as[s1]];
         }
         }
         }
         else{
         s-=2*pr->rho*(nodes[i]->B-pr->rho*(nodes[i]->D-nodes[nodes[i]->P]->D))/nodes[i]->V;
         for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
         int s1=*iter;
         s+=2*pr->rho*(nodes[s1]->B-pr->rho*nodes[s1]->D+pr->rho*nodes[i]->D)/nodes[s1]->V;
         if (tc(nodes[s1])) {
         s+=lambda[as[s1]];
         }
         }
         }
         if (myabs(s)>1e-6) {
         cout<<"TEST PROBLEM "<<i<<" "<<s<<" "<<nodes[i]->P<<endl;
         }
         }
         }
         if (myabs(sr)>1e-5 && pr->rho!=pr->rho_min) {
         cout<<"TEST PROBLEM rho "<<sr<<endl;
         }*/
        delete[] Pre;
        delete[] Suc;
        delete[] feuilles;
        delete[] internal;
        delete[] as;
        delete[] bl;
        delete[] lambda;
        return ld;
    }
}


bool with_constraint_active_set_lambda(double br,Pr* pr,Node** nodes){
    for (vector<int>::iterator iter=nodes[0]->suc.begin(); iter!=nodes[0]->suc.end(); iter++) {
        nodes[*iter]->B=br/2.;
    }
    list<int> active_set;
    bool consistent = starting_pointQP(pr,nodes,active_set);
    if (consistent){
        list<double> lambda;
        double* dir = new double[pr->nbBranches+1];
        double* D_old = new double[pr->nbBranches+1];
        
        for (int i=0; i<=pr->nbBranches; i++) {
            D_old[i]=nodes[i]->D;
        }
        lambda = with_constraint_lambda(br,pr,nodes,active_set);
        int nb_iter=0;
        double alpha;
        while (!conditionsQP(lambda,pr,nodes) && nb_iter<=maxIter){
            for (int i=0;i<=pr->nbBranches;i++)  {dir[i]=nodes[i]->D-D_old[i];}
            alpha=1;
            int as=2*pr->nbBranches+2;
            double a;
            for (int i=0; i<=pr->nbBranches; i++) {
                int p = nodes[i]->P;
                if (p!=-1){
                    if (dir[p]>dir[i] && !tc(nodes[i])){
                        a = (D_old[i]-D_old[p])/(dir[p]-dir[i]);
                        if (a<alpha){
                            alpha = a;
                            as = i;
                        }
                    }
                }
                if (dir[i]<0 && (nodes[i]->type=='l' || nodes[i]->type=='b') && !lower(nodes[i])){
                    a = (nodes[i]->lower-D_old[i])/dir[i];
                    if (a<alpha){
                        alpha = a;
                        as = -i;
                    }
                }
                if (dir[i]>0 && (nodes[i]->type=='u' || nodes[i]->type=='b') && !upper(nodes[i])){
                    a = (nodes[i]->upper-D_old[i])/dir[i];
                    if (a<alpha){
                        alpha = a;
                        as = i+pr->nbBranches+1;
                    }
                }
            }
            int asrm;
            if (remove_ne_lambda(lambda,active_set,asrm)){
                active_set.remove(asrm);
                if (asrm>0) {
                    desactiveTC(nodes[asrm]);
                }
                else {
                    desactiveLimit(nodes[-asrm]);
                }
            }
            if (as<2*pr->nbBranches+2) {
                for (int i=0;i<=pr->nbBranches;i++) D_old[i]=D_old[i]+alpha*dir[i];
                if (as>pr->nbBranches) {
                    active_set.push_back(-(as-pr->nbBranches-1));
                    nodes[(as-pr->nbBranches-1)]->D=nodes[(as-pr->nbBranches-1)]->upper;
                    activeUpper(nodes[(as-pr->nbBranches-1)]);
                }
                else if (as>0) {
                    active_set.push_back(as);
                    activeTC(nodes[as]);
                }
                else{
                    active_set.push_back(as);
                    nodes[-as]->D=nodes[-as]->lower;
                    activeLower(nodes[-as]);
                }
            }
            lambda=with_constraint_lambda(br,pr,nodes,active_set);
            nb_iter++;
        }
        if (nb_iter>maxIter){
            for (int i=0;i<=pr->nbBranches;i++) nodes[i]->D=D_old[i];
        }
        delete[] D_old;
        delete[] dir;
        return true;
    }
    else return false;
}

void with_constraint_active_set_lambda_secondTime(double br,Pr* pr,Node** nodes){
    list<int> active_set = getActiveSet(pr,nodes);
    computeNewVarianceEstimateRoot(pr,nodes);
    list<double> lambda;
    double* dir = new double[pr->nbBranches+1];
    double* D_old = new double[pr->nbBranches+1];
    
    for (int i=0; i<=pr->nbBranches; i++) {
        D_old[i]=nodes[i]->D;
    }
    lambda = with_constraint_lambda(br,pr,nodes,active_set);
    int nb_iter=0;
    double alpha;
    while (!conditionsQP(lambda,pr,nodes) && nb_iter<=maxIter){
        for (int i=0;i<=pr->nbBranches;i++)  {dir[i]=nodes[i]->D-D_old[i];}
        alpha=1;
        int as=2*pr->nbBranches+2;
        double a;
        for (int i=0; i<=pr->nbBranches; i++) {
            int p = nodes[i]->P;
            if (p!=-1){
                if (dir[p]>dir[i] && !tc(nodes[i])){
                    a = (D_old[i]-D_old[p])/(dir[p]-dir[i]);
                    if (a<alpha){
                        alpha = a;
                        as = i;
                    }
                }
            }
            if (dir[i]<0 && (nodes[i]->type=='l' || nodes[i]->type=='b') && !lower(nodes[i])){
                a = (nodes[i]->lower-D_old[i])/dir[i];
                if (a<alpha){
                    alpha = a;
                    as = -i;
                }
            }
            if (dir[i]>0 && (nodes[i]->type=='u' || nodes[i]->type=='b') && !upper(nodes[i])){
                a = (nodes[i]->upper-D_old[i])/dir[i];
                if (a<alpha){
                    alpha = a;
                    as = i+pr->nbBranches+1;
                }
            }
        }
        int asrm;
        if (remove_ne_lambda(lambda,active_set,asrm)){
            active_set.remove(asrm);
            if (asrm>0) {
                desactiveTC(nodes[asrm]);
            }
            else {
                desactiveLimit(nodes[-asrm]);
            }
        }
        if (as<2*pr->nbBranches+2) {
            for (int i=0;i<=pr->nbBranches;i++) D_old[i]=D_old[i]+alpha*dir[i];
            if (as>pr->nbBranches) {
                active_set.push_back(-(as-pr->nbBranches-1));
                nodes[(as-pr->nbBranches-1)]->D=nodes[(as-pr->nbBranches-1)]->upper;
                activeUpper(nodes[(as-pr->nbBranches-1)]);
            }
            else if (as>0) {
                active_set.push_back(as);
                activeTC(nodes[as]);
            }
            else{
                active_set.push_back(as);
                nodes[-as]->D=nodes[-as]->lower;
                activeLower(nodes[-as]);
            }
        }
        lambda=with_constraint_lambda(br,pr,nodes,active_set);
        nb_iter++;
    }
    if (nb_iter>maxIter){
        for (int i=0;i<=pr->nbBranches;i++) nodes[i]->D=D_old[i];
    }
    delete[] D_old;
    delete[] dir;
}

int estimate_root_without_constraint_local_rooted(Pr* pr,Node** nodes){
    //P: rooted tree, recherche la nouvelle racine autour de l'ancien racine.
    //estimate root locally with LD algorithm for rooted tree////////////////////////
    //cout<<"Re-estimating the root without constraints around the given root ... ";
    double phi1=-1;
    Node** nodes_new = cloneLeaves(pr,nodes,0);
    double* cv = new double[pr->nbBranches+1];
    for (int i=0;i<=pr->nbBranches;i++) cv[i]=0;
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s1=(*iter);
    iter++;
    int s2=(*iter);
    double br=0;
    int r=0;
    double* multiplier = new double[pr->ratePartition.size()+1];
    cout<<"Optimizing the root position on the original branch "<<s1<<" ... ";
    bool bl = reroot_rootedtree(br,s1,s1,s2,pr,nodes,nodes_new);
    if (bl){
        bool consistent=without_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
        if (consistent) {
            cv[s1]=pr->objective;
            printf("%.10f\n",cv[s1]);
            cv[s2]=cv[s1];
            phi1=cv[s1];
            r=s1;
            for (int i=1; i<=pr->ratePartition.size(); i++) {
                multiplier[i] = pr->multiplierRate[i];
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
    }
    else{
        printf("Temporal constraints conflict\n");
    }
    list<int> next;
    if (s1<pr->nbINodes){
        for (vector<int>::iterator iter=nodes[s1]->suc.begin(); iter!=nodes[s1]->suc.end(); iter++) {
            next.push_back(*iter);
        }
    }
    if (s2<pr->nbINodes){
        for (vector<int>::iterator iter=nodes[s2]->suc.begin(); iter!=nodes[s2]->suc.end(); iter++) {
            next.push_back(*iter);
        }
    }
    while (!next.empty()){
        int i = next.back();
        for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
            nodes_new[i]->status=nodes[i]->status;
        }
        bl=reroot_rootedtree(br,i,s1,s2,pr,nodes,nodes_new);
        cout<<"Optimizing the root position on the branch "<<i<<" ... ";
        if (bl){
            bool consistent=without_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
            if (consistent) {
                cv[i]=pr->objective;
                printf("%.10f\n",cv[i]);
                if (cv[i]<cv[nodes[i]->P] || r==0){
                    if (i<pr->nbINodes){
                        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                            next.push_back(*iter);
                        }
                    }
                    if (cv[i]<phi1 || r==0){
                        phi1=cv[i];r=i;
                        for (int i=1; i<=pr->ratePartition.size(); i++) {
                            multiplier[i] = pr->multiplierRate[i];
                        }
                    }
                }
            }
            else{
                printf("Temporal constraints conflict\n");
                if (i<pr->nbINodes){
                    for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                        next.push_back(*iter);
                    }
                }
            }
        }
        else{
            printf("Temporal constraints conflict\n");
            if (i<pr->nbINodes){
                for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                    next.push_back(*iter);
                }
            }
        }
        next.remove(i);
    }
    if (r==0) cout<<"Temporal constraints conflict."<<endl;
    else if (r==s1 || r==s2) cout<<"The new root is on the original branch."<<endl;
    else cout<<"The new root is on the branch "<<r<<endl;
    delete[] cv;
    for (int i=0;i<pr->nbBranches+1;i++) delete nodes_new[i];
    delete[] nodes_new;
    for (int i=1; i<=pr->ratePartition.size(); i++) {
        pr->multiplierRate[i] = multiplier[i];
    }
    delete[] multiplier;
    return r;
}

int estimate_root_without_constraint_rooted(Pr* pr,Node** &nodes){
    //P: rooted tree
    //estimate root with LD algorithm for rooted tree//////////////////////////////////////////
    //cout<<"Re-estimating the root without constraints on all branches... ";
    double phi1;
    Node** nodes_new = cloneLeaves(pr,nodes,0);
    int y=1;
    double br=0;
    int r=0;
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s1=(*iter);
    iter++;
    int s2=(*iter);
    double* multiplier = new double[pr->ratePartition.size()+1];
    cout<<"Optimizing the root position on the branch "<<y<<" ... ";
    bool bl=reroot_rootedtree(br,y,s1,s2,pr,nodes,nodes_new);
    if (bl){
        bool consistent=without_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
        if (consistent) {
            phi1=pr->objective;
            printf("%.10f\n",phi1);
            r=y;
            for (int i=1; i<=pr->ratePartition.size(); i++) {
                multiplier[i] = pr->multiplierRate[i];
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
    }
    else{
        printf("Temporal constraints conflict\n");
    }
    y++;
    double phi;
    while (y<=pr->nbBranches){
        for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
            nodes_new[i]->status=nodes[i]->status;
        }
        cout<<"Optimizing the root position on the branch "<<y<<" ... ";
        bl=reroot_rootedtree(br,y,s1,s2,pr,nodes,nodes_new);
        if (bl){
            bool consistent=without_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
            if (consistent) {
                phi=pr->objective;
                printf("%.10f\n",phi);
                if (phi1>phi || r==0){
                    phi1=phi;
                    r=y;
                    for (int i=1; i<=pr->ratePartition.size(); i++) {
                        multiplier[i] = pr->multiplierRate[i];
                    }
                }
            }
            else{
                printf("Temporal constraints conflict\n");
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
        y++;
    }
    if (r==0) cout<<"Temporal constraints conflict."<<endl;
    else if (r==s1 || r==s2) cout<<"The new root is on the original branch."<<endl;
    else cout<<"The new root is on the branch "<<r<<endl;
    for (int i=0;i<pr->nbBranches+1;i++) delete nodes_new[i];
    delete[] nodes_new;
    for (int i=1; i<=pr->ratePartition.size(); i++) {
        pr->multiplierRate[i] = multiplier[i];
    }
    delete[] multiplier;
    return r;
}


int estimate_root_with_constraint_local_rooted(Pr* pr,Node** nodes){
    //P: rooted tree, recherche la nouvelle racine autour de l'ancienne racine.
    /////////////estimate root locally with QPD algorithm for rooted tree////////////////////////////////////////////////////////////
    //    cout<<"Re-estimating the root with constraints around the given root ... ";
    Node** nodes_new = cloneLeaves(pr,nodes,0);
    double* cv = new double[pr->nbBranches+1];
    for (int i=0;i<=pr->nbBranches;i++) cv[i]=0;
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s1=(*iter);
    iter++;
    int s2=(*iter);
    double br=0;
    int r=0;
    double phi1;
    double* multiplier = new double[pr->ratePartition.size()+1];
    cout<<"Optimizing the root position on the original branch "<<s1<<" ... ";
    bool bl = reroot_rootedtree(br,s1,s1,s2,pr,nodes,nodes_new);
    if (bl){
        bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
        if (consistent){
            cv[s1]=pr->objective;
            printf("%.10f\n",cv[s1]);
            cv[s2]=cv[s1];
            r=s1;
            phi1=cv[s1];
            for (int i=1; i<=pr->ratePartition.size(); i++) {
                multiplier[i] = pr->multiplierRate[i];
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
    }
    else{
        printf("Temporal constraints conflict\n");
    }
    list<int> next;
    if (s1<pr->nbINodes){
        for (vector<int>::iterator iter=nodes[s1]->suc.begin(); iter!=nodes[s1]->suc.end(); iter++) {
            next.push_back(*iter);
        }
    }
    if (s2<pr->nbINodes){
        for (vector<int>::iterator iter=nodes[s2]->suc.begin(); iter!=nodes[s2]->suc.end(); iter++) {
            next.push_back(*iter);
        }
    }
    while (!next.empty()){
        int i = next.back();
        for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
            nodes_new[i]->status=nodes[i]->status;
        }
        cout<<"Optimizing the root position on the branch "<<i<<" ... ";
        bl=reroot_rootedtree(br,i,s1,s2,pr,nodes,nodes_new);
        if (bl){
            bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
            if (consistent){
                cv[i]=pr->objective;
                printf("%.10f \n",cv[i]);
                if (cv[i]<cv[nodes[i]->P] || r==0){
                    if (i<pr->nbINodes){
                        for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                            next.push_back(*iter);
                        }
                    }
                    if (cv[i]<phi1 || r==0){
                        phi1=cv[i];r=i;
                        for (int i=1; i<=pr->ratePartition.size(); i++) {
                            multiplier[i] = pr->multiplierRate[i];
                        }
                    }
                }
            }
            else{
                printf("Temporal constraints conflict\n");
                if (i<pr->nbINodes){
                    for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                        next.push_back(*iter);
                    }
                }
            }
        }
        else{
            printf("Temporal constraints conflict\n");
            if (i<pr->nbINodes){
                for (vector<int>::iterator iter=nodes[i]->suc.begin(); iter!=nodes[i]->suc.end(); iter++) {
                    next.push_back(*iter);
                }
            }
        }
        next.remove(i);
    }
    if (r==0) cout<<"Temporal constraints conflict."<<endl;
    else if (r==s1 || r==s2) cout<<"The new root is on the original branch."<<endl;
    else cout<<"The new root is on the branch "<<r<<endl;
    delete[] cv;
    for (int i=0;i<pr->nbBranches+1;i++) delete nodes_new[i];
    delete[] nodes_new;
    for (int i=1; i<=pr->ratePartition.size(); i++) {
        pr->multiplierRate[i] = multiplier[i];
    }
    delete[] multiplier;
    return r;
}

int estimate_root_with_constraint_fast_rooted(Pr* pr,Node** nodes){
    //P: rooted tree, oublier la racine, recherche la nouvelle racine sur toutes les branches
    /////////////estimate root on all branches with QPD algorithm for rooted tree////////////////////////////////////////////////////////////
    //cout<<"Re-estimating the root with constraints on all branches using fast method."<<endl;
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s01=(*iter);
    iter++;
    int s02=(*iter);
    double* multiplier = new double[pr->ratePartition.size()+1];
    cout<<"Pre-estimating the position of the root without using temporal constraints ..."<<endl;
    int r=estimate_root_without_constraint_rooted(pr,nodes);
    cout<<"Re-estimating the position of the root with temporal constraints around the pre-estimated root ..."<<endl;
    if (r>0){
        double phi1=pr->objective;
        //cout<<"Re-estimating the root with constraints around the given root ... ";
        //cout<<"Optimizing the root position on the branch "<<r<<" ... ";
        Node** nodes_new = cloneLeaves(pr,nodes,0);
        int* P_ref = new int[pr->nbBranches+1];
        int* tab = new int[pr->nbBranches+1];
        double br=0;
        double* cv = new double[pr->nbBranches+1];
        for (int i=0;i<=pr->nbBranches;i++) cv[i]=0;
        cout<<"Optimizing the root position on the branch "<<r<<" ... ";
        bool bl=reroot_rootedtree(br,r,s01,s02,pr,nodes,nodes_new,P_ref,tab);
        if (bl){
            bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
            if (consistent){
                cv[r]=pr->objective;
                printf("%.10f\n",cv[r]);
                phi1=cv[r];
                for (int i=1; i<=pr->ratePartition.size(); i++) {
                    multiplier[i] = pr->multiplierRate[i];
                }
            }
            else{
                printf("Temporal constraints conflict\n");
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
        list<int> next;
        int* Suc1_ref = new int[pr->nbINodes];
        int* Suc2_ref = new int[pr->nbINodes];
        computeSuc(P_ref,Suc1_ref,Suc2_ref,pr->nbBranches+1,pr->nbINodes);
        int s1=Suc1_ref[0];
        int s2=Suc2_ref[0];
        if (s1<pr->nbINodes){
            next.push_back(Suc1_ref[s1]);
            next.push_back(Suc2_ref[s1]);
        }
        if (s2<pr->nbINodes){
            next.push_back(Suc1_ref[s2]);
            next.push_back(Suc2_ref[s2]);
        }
        while (!next.empty()){
            int i = next.back();
            int e = tab[i];
            for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
                nodes_new[i]->status=nodes[i]->status;
            }
            cout<<"Optimizing the root position on the branch "<<e<<" ... ";
            bl=reroot_rootedtree(br,e,s01,s02,pr,nodes,nodes_new);
            if (bl){
                bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
                if (consistent){
                    cv[e]=pr->objective;
                    printf("%.10f\n",cv[e]);
                    if (cv[e]<cv[tab[P_ref[i]]] || r==0){
                        if (i<pr->nbINodes){
                            next.push_back(Suc1_ref[i]);
                            next.push_back(Suc2_ref[i]);
                        }
                        if (cv[i]<phi1 || r==0){
                            phi1=cv[e];r=e;
                            for (int i=1; i<=pr->ratePartition.size(); i++) {
                                multiplier[i] = pr->multiplierRate[i];
                            }
                        }
                    }
                }
                else{
                    printf("Temporal constraints conflict\n");
                    if (i<pr->nbINodes){
                        next.push_back(Suc1_ref[i]);
                        next.push_back(Suc2_ref[i]);
                    }
                }
            }
            else{
                printf("Temporal constraints conflict\n");
                if (i<pr->nbINodes){
                    next.push_back(Suc1_ref[i]);
                    next.push_back(Suc2_ref[i]);
                }
            }
            next.remove(i);
        }
        if (r==s01 || r==s02) cout<<"The new root is on the original branch."<<endl;
        else cout<<"The new root is on the branch "<<r<<endl;
        delete[] cv;
        delete[] P_ref;
        delete[] tab;
        delete[] Suc1_ref;
        delete[] Suc2_ref;
        for (int i=0;i<pr->nbBranches+1;i++) delete nodes_new[i];
        delete[] nodes_new;
        for (int i=1; i<=pr->ratePartition.size(); i++) {
            pr->multiplierRate[i] = multiplier[i];
        }
        delete[] multiplier;
    }
    return r;
}


int estimate_root_with_constraint_rooted(Pr* pr,Node** nodes){
    //P: rooted tree
    //estimate root with QPD algorithm for rooted tree//////////////////////////////////
    int y=1;
    Node** nodes_new = cloneLeaves(pr,nodes,0);
    double br=0;
    double phi1;
    double phi;
    int r=0;
    double* multiplier = new double[pr->ratePartition.size()+1];
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int s1=(*iter);
    iter++;
    int s2=(*iter);
    cout<<"Optimizing the root position on the branch "<<y<<" ... ";
    bool bl=reroot_rootedtree(br,y,s1,s2,pr,nodes,nodes_new);
    if (bl){
        bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
        if (consistent){
            printf("%.10f\n",pr->objective);
            r=y;
            phi1=pr->objective;
            for (int i=1; i<=pr->ratePartition.size(); i++) {
                multiplier[i] = pr->multiplierRate[i];
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
    }
    else{
        printf("Temporal constraints conflict\n");
    }
    y++;
    while (y<=pr->nbBranches){
        for (int i=pr->nbINodes; i<=pr->nbBranches; i++) {
            nodes_new[i]->status=nodes[i]->status;
        }
        cout<<"Optimizing the root position on the branch "<<y<<" ... ";
        bl=reroot_rootedtree(br,y,s1,s2,pr,nodes,nodes_new);
        if (bl){
            bool consistent=with_constraint_active_set_lambda_multirates(br,pr,nodes_new,true);
            if (consistent){
                phi=pr->objective;
                printf("%.10f\n",phi);
                if (phi1>phi  || r==0){
                    phi1=phi;
                    r=y;
                    for (int i=1; i<=pr->ratePartition.size(); i++) {
                        multiplier[i] = pr->multiplierRate[i];
                    }
                }
            }
            else{
                printf("Temporal constraints conflict\n");
            }
        }
        else{
            printf("Temporal constraints conflict\n");
        }
        y++;
    }
    if (r==0) cout<<"Temporal constraints conflict."<<endl;
    else if (r==s1 || r==s2) cout<<"The new root is on the original branch."<<endl;
    else cout<<"The new root is on the branch "<<r<<endl;
    for (int i=0;i<pr->nbBranches+1;i++) delete nodes_new[i];
    delete[] nodes_new;
    for (int i=1; i<=pr->ratePartition.size(); i++) {
        pr->multiplierRate[i] = multiplier[i];
    }
    delete[] multiplier;
    return r;
}


void calculateMultiplier_lambda(int r,int p_r,double br,Pr* pr,Node** nodes,bool* nan){
    int nbG = pr->ratePartition.size()+1;
    double* A =  new double[nbG];
    double* B =  new double[nbG];
    for (int i=1;i<nbG;i++){
        A[i] = 0;
        B[i] = 0;
    }
    int g = nodes[r]->rateGroup;
    A[g] += pr->rho*pr->rho*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D)*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D)/nodes[r]->V;
    B[g] += -2*br*pr->rho*(nodes[r]->D+nodes[p_r]->D-2*nodes[0]->D)/nodes[r]->V;
    for (int i=1; i<=pr->nbBranches; i++) {
        g = nodes[i]->rateGroup;
        if (i!=r && i!=p_r) {
            A[g] += pr->rho*pr->rho*(nodes[i]->D-nodes[nodes[i]->P]->D)*(nodes[i]->D-nodes[nodes[i]->P]->D)/nodes[i]->V;
            B[g] += -2*nodes[i]->B*pr->rho*(nodes[i]->D-nodes[nodes[i]->P]->D)/nodes[i]->V;
        }
    }
    for (int i=1; i<nbG; i++) {
        pr->multiplierRate[i] = -B[i]/2/A[i];
        if (pr->multiplierRate[i]*pr->rho < pr->rho_min){
            pr->multiplierRate[i] = pr->rho_min/pr->rho;
        }
        if (A[i]==0) {
            nan[i]=true;
        }
    }
}

bool without_constraint_active_set_lambda_multirates(double br,Pr* pr,Node** nodes,bool reassign){
    vector<int>::iterator iter=nodes[0]->suc.begin();    
    int r=(*iter);//r=r1
    iter++;
    int p_r=(*iter);//pr=r2
    double* B = new double[pr->nbBranches+1];
    double* V = new double[pr->nbBranches+1];
    double Br = br;
    for (int i=1; i<=pr->nbBranches; i++) {
        B[i] = nodes[i]->B;
        V[i] = nodes[i]->V;
    }
    if (pr->ratePartition.size()>0){
        if (reassign) assignRateGroupToTree(pr,nodes);
        for (int i=1; i<=pr->nbBranches; i++) {
            if (i!=r && i!=p_r) {
                double m = pr->multiplierRate[nodes[i]->rateGroup];
                nodes[i]->B = B[i]/m;
                nodes[i]->V = V[i]/m/m;
            }
        }
        br = br/pr->multiplierRate[nodes[r]->rateGroup];
    }
    bool consistent = without_constraint_active_set_lambda(br,pr,nodes);
    if (pr->ratePartition.size()>0) {
        vector<int>::iterator iter=nodes[0]->suc.begin();
        bool* nan = new bool[pr->ratePartition.size()+1];
        for (int i=1; i<=pr->ratePartition.size(); i++){
            nan[i] = false;
        }
        /*printf("ROUND 0 , objective function %.15e , rate %.15f ",pr->objective,pr->rho);
        for (int r=1; r<=pr->ratePartition.size(); r++) {
            if (!nan[r]) printf(" %.15f ",pr->rho*pr->multiplierRate[r]);
        }
        cout<<endl;*/
        double old_phi = 0;
        double old_rho = 0;
        double* old_multi = new double[pr->ratePartition.size()+1];
        old_multi[0]=1;
        int i= 1;
        bool cont = false;
        do {
            old_phi = pr->objective;
            old_rho = pr->rho;
            for (int r=1; r<=pr->ratePartition.size(); r++) {
                old_multi[r] = pr->multiplierRate[r];
            }
            br = Br;
            for (int r=1; r<=pr->nbBranches; r++) {
                nodes[r]->B = B[r];
                nodes[r]->V = V[r];
            }
            calculateMultiplier_lambda(r,p_r,br,pr,nodes,nan);
            br = br/pr->multiplierRate[nodes[r]->rateGroup];
            for (int j=1; j<=pr->nbBranches; j++) {
                double m = pr->multiplierRate[nodes[j]->rateGroup];
                nodes[j]->B = B[j]/m;
                nodes[j]->V = V[j]/m/m;
            }
            consistent = without_constraint_active_set_lambda(br,pr,nodes);
            cont = myabs((old_rho-pr->rho)/pr->rho) >= 1e-5;
            for (int j=1; j<=pr->ratePartition.size(); j++) {
                cont = cont || (myabs((old_multi[j]*old_rho-pr->multiplierRate[j]*pr->rho)/pr->multiplierRate[j]/pr->rho)>=1e-5);
            }
            /*printf("ROUND %d , objective function %.15e , rate %.15f ",i,pr->objective,pr->rho);
            for (int r=1; r<=pr->ratePartition.size(); r++) {
                if (!nan[r]) printf(" %.15f ",pr->rho*pr->multiplierRate[r]);
            }
            cout<<endl;*/
            i++;
        } while (cont);
        br = Br;
        for (int j=1; j<=pr->nbBranches; j++) {
            nodes[j]->B = B[j];
            nodes[j]->V = V[j];
        }
        /*printf("ROUND %d , objective function %.15e , rate %.15f ",i,pr->objective,pr->rho);
        for (int g=1; g<=pr->ratePartition.size(); g++) {
            if (!nan[g]) printf(" %.15f ",pr->rho*pr->multiplierRate[g]);
        }
        cout<<endl;*/
    }
    delete[] B;
    delete[] V;
    return consistent;
}

bool with_constraint_active_set_lambda_multirates(double br,Pr* pr,Node** nodes,bool reassign){
    vector<int>::iterator iter=nodes[0]->suc.begin();
    int r=(*iter);//r=r1
    iter++;
    int p_r=(*iter);//pr=r2
    double* B = new double[pr->nbBranches+1];
    double* V = new double[pr->nbBranches+1];
    double Br = br;
    for (int i=1; i<=pr->nbBranches; i++) {
        B[i] = nodes[i]->B;
        V[i] = nodes[i]->V;
    }
    if (pr->ratePartition.size()>0){
        if (reassign) assignRateGroupToTree(pr,nodes);
        for (int i=1; i<=pr->nbBranches; i++) {
            if (i!=r && i!=p_r) {
                double m = pr->multiplierRate[nodes[i]->rateGroup];
                nodes[i]->B = B[i]/m;
                nodes[i]->V = V[i]/m/m;
            }
        }
        br = br/pr->multiplierRate[nodes[r]->rateGroup];
    }
    bool consistent = with_constraint_active_set_lambda(br,pr,nodes);
    if (pr->ratePartition.size()>0) {
        double* B = new double[pr->nbBranches+1];
        double* V = new double[pr->nbBranches+1];
        double Br = br;
        for (int i=1; i<=pr->nbBranches; i++) {
            B[i] = nodes[i]->B;
            V[i] = nodes[i]->V;
        }
        bool* nan = new bool[pr->ratePartition.size()+1];
        for (int i=1; i<=pr->ratePartition.size(); i++){
            nan[i] = false;
        }
        /*printf("ROUND 0 , objective function %.15e , rate %.15f ",pr->objective,pr->rho);
         for (int r=1; r<=pr->ratePartition.size(); r++) {
         if (!nan[r]) printf(" %.15f ",pr->rho*pr->multiplierRate[r]);
         }
         cout<<endl;*/
        double old_phi = 0;
        double old_rho = 0;
        double* old_multi = new double[pr->ratePartition.size()+1];
        old_multi[0]=1;
        int i= 1;
        bool cont = false;
        do {
            old_phi = pr->objective;
            old_rho = pr->rho;
            for (int r=1; r<=pr->ratePartition.size(); r++) {
                old_multi[r] = pr->multiplierRate[r];
            }
            br = Br;
            for (int r=1; r<=pr->nbBranches; r++) {
                nodes[r]->B = B[r];
                nodes[r]->V = V[r];
            }
            calculateMultiplier_lambda(r,p_r,br,pr,nodes,nan);
            br = br/pr->multiplierRate[nodes[r]->rateGroup];
            for (int j=1; j<=pr->nbBranches; j++) {
                double m = pr->multiplierRate[nodes[j]->rateGroup];
                nodes[j]->B = B[j]/m;
                nodes[j]->V = V[j]/m/m;
            }
            consistent = with_constraint_active_set_lambda(br,pr,nodes);
            cont = myabs((old_rho-pr->rho)/pr->rho) >= 1e-5;
            for (int j=1; j<=pr->ratePartition.size(); j++) {
                cont = cont || (myabs((old_multi[j]*old_rho-pr->multiplierRate[j]*pr->rho)/pr->multiplierRate[j]/pr->rho)>=1e-5);
            }
            /*printf("ROUND %d , objective function %.15e , rate %.15f ",i,pr->objective,pr->rho);
             for (int r=1; r<=pr->ratePartition.size(); r++) {
             if (!nan[r]) printf(" %.15f ",pr->rho*pr->multiplierRate[r]);
             }
             cout<<endl;*/
            i++;
        } while (cont);
        br = Br;
        for (int j=1; j<=pr->nbBranches; j++) {
            nodes[j]->B = B[j];
            nodes[j]->V = V[j];
        }
        /*printf("ROUND %d , objective function %.15e , rate %.15f ",i,pr->objective,pr->rho);
        for (int g=1; g<=pr->ratePartition.size(); g++) {
            if (!nan[g]) printf(" %.15f ",pr->rho*pr->multiplierRate[g]);
        }
        cout<<endl;*/
    }
    delete[] B;
    delete[] V;
    return consistent;
}
