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

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <list>
#include <math.h>
#include <ctime>
#include "../include/lsd.h"

using namespace std;

int main( int argc, char** argv ){
    Pr* opt = getOptions( argc, argv);
    FILE * result = fopen(opt->outFile.c_str(),"wt");
    if (result==NULL){
        cout<<"Error: can not create the output file "<<opt->outFile<<endl;
        exit(EXIT_FAILURE);
    }
    printInterface( result, opt);
    fprintf(result,"\n");
    clock_t start = clock();
    double elapsed_time;
    if (opt->fnOutgroup!=""){
        list<string> outgroup = getOutgroup(opt->fnOutgroup);
        extrait_outgroup(opt,outgroup);
    }
    checkRooted(opt);
    FILE * tree = fopen(opt->inFile.c_str(),"rt");
    if (tree==NULL){
        cout<<"Error: can not open the input file"<<endl;
        exit(EXIT_FAILURE);
    }
    opt->treeFile1=opt->outFile+".nexus";
    opt->treeFile2=opt->outFile+".newick";
    opt->treeFile3=opt->outFile+".date.newick";
    FILE * tree1 = fopen(opt->treeFile1.c_str(),"wt");
    FILE * tree2 = fopen(opt->treeFile2.c_str(),"wt");
    FILE * tree3 = fopen(opt->treeFile3.c_str(),"wt");
    FILE * gr=NULL;//given rate
    if (tree1==NULL || tree2==NULL || tree3==NULL){
        cout<<"Error: can not create the output tree file "<<opt->treeFile1<<endl;
    }
    fprintf(tree1,"#NEXUS\n");
    Node** nodes = new Node*[opt->nbBranches+1];
    if (opt->givenRate) {
        gr = fopen(opt->rate.c_str(),"rt");
    }
    bool constraintConsistent=true;
    int s=0;
    if (opt->partitionFile!="") {
        cout<<"Reading partition file"<<endl;
        readPartitionFile(opt);
    }
    for (int y=1;y<=opt->nbData;y++){
        fprintf(result,"Tree %d ",y);
        cout<<"Reading the tree "<<y<<endl;
        opt->initConstraints();
        if (!opt->relative) s=tree2data(tree,opt,nodes,constraintConsistent);
        else s=tree2dataS(tree,opt,nodes);
        if (!opt->rooted){
            nodes = unrooted2rooted(opt,nodes,s);
        }
        if (opt->relative){
            for (int i=0;i<opt->nbINodes;i++) nodes[i]->removeConstraint();
            for (int i=opt->nbINodes;i<=opt->nbBranches;i++){
                nodes[i]->newPConstraint('p',opt->leaves);
            }
            Date* dateRoot = new Date('p',opt->mrca,0,0);
            opt->internalConstraints.clear();
            opt->internalConstraints.push_back(dateRoot);
        }
        if (y==1){
            fprintf(tree1,"Begin trees;\n");
        }
        computeVariance(opt,nodes);
        constraintConsistent=initConstraint(opt, nodes);
        double br=0;
        if (opt->givenRate) opt->rho=readdouble(gr,opt->rate);
        if (!opt->constraint){//LD without constraints
            if (opt->estimate_root==""){//keep the given root
                if (constraintConsistent){
                    cout<<"Dating without temporal constraints ..."<<endl;
                    without_constraint_multirates(opt,nodes,true);
                    output(br,y,opt,nodes,result,tree1,tree2,tree3);
                }
                else{
                    cout<<"There's conflict in the input temporal constraints. Try the option -r to re-estimate the root position."<<endl;
                }
            }
            else if (opt->estimate_root=="k"){
                cout<<"Estimating the root position on the branch defined by given outgroups ..."<<endl;
                double br=0;
                vector<int>::iterator iter=nodes[0]->suc.begin();
                int s1=(*iter);
                iter++;
                int s2=(*iter);
                br=nodes[s1]->B+nodes[s2]->B;
                nodes[s1]->V=variance(opt,br);
                nodes[s2]->V=nodes[s1]->V;
                without_constraint_active_set_lambda_multirates(br,opt,nodes,true);
                //br=(nodes[s1]->D+nodes[s2]->D-2*nodes[0]->D)*opt->rho;
                output(br,y,opt,nodes,result,tree1,tree2,tree3);
            }
            else{//estimate the root
                int r;
                if (opt->estimate_root.compare("l")==0){//improve locally the root around the given root
                    cout<<"Estimating the root position locally around the given root ..."<<endl;
                    r=estimate_root_without_constraint_local_rooted(opt,nodes);
                }
                else{ //forget the given root and re-estimate the position of the root over all branhces
                    cout<<"Estimating the root position on all branches ..."<<endl;
                    r=estimate_root_without_constraint_rooted(opt,nodes);
                }
                if (r!=0){
                    Node** nodes_new = cloneLeaves(opt,nodes,0);
                    vector<int>::iterator iter=nodes[0]->suc.begin();
                    int s1=(*iter);
                    iter++;
                    int s2=(*iter);
                    for (int i=opt->nbINodes; i<=opt->nbBranches; i++) {
                        nodes_new[i]->status=nodes[i]->status;
                    }
                    reroot_rootedtree(br,r,s1,s2,opt,nodes,nodes_new);
                    without_constraint_active_set_lambda_multirates(br,opt,nodes_new,true);
                    //br=(nodes[s1]->D+nodes[s2]->D-2*nodes[0]->D)*opt->rho;
                    output(br,y,opt,nodes_new,result,tree1,tree2,tree3);
                    for (int i=0;i<opt->nbBranches+1;i++) delete nodes_new[i];
                    delete[] nodes_new;
                }
            }

        }
        else {//QPD with temporal constrains
                if (opt->estimate_root==""){//keep the given root
                if (constraintConsistent){
                    cout<<"Dating under temporal constraints mode ..."<<endl;
                    bool consistent = with_constraint_multirates(opt,nodes,true);
                    if (consistent) {
                        output(br,y,opt,nodes,result,tree1,tree2,tree3);
                    }
                    else{
                        cout<<"There's conflict in the input temporal constraints. Try the option -r to re-estimate the root position."<<endl;
                    }
                }
                else{
                    cout<<"There's conflict in the input temporal constraints. Try the option -r to re-estimate the root position."<<endl;
                }
            }
            else if (opt->estimate_root=="k"){
                    cout<<"Estimating the root position on the branch defined by given outgroups ..."<<endl;
                    double br=0;
                    vector<int>::iterator iter=nodes[0]->suc.begin();
                    int s1=(*iter);
                    iter++;
                    int s2=(*iter);
                    br=nodes[s1]->B+nodes[s2]->B;
                    nodes[s1]->V=variance(opt,br);
                    nodes[s2]->V=nodes[s1]->V;
                    with_constraint_active_set_lambda_multirates(br,opt,nodes,true);
                    //br=(nodes[s1]->D+nodes[s2]->D-2*nodes[0]->D)*opt->rho;
                    output(br,y,opt,nodes,result,tree1,tree2,tree3);
            }
            else{//estimate the root
                int r;
                if (opt->estimate_root.compare("l")==0){//improve locally the root around the given root
                    cout<<"Estimating the root position locally around the given root ..."<<endl;
                    r=estimate_root_with_constraint_local_rooted(opt,nodes);
                }
                else if (opt->estimate_root.compare("a")==0){ //forget the given root and re-estimate the position of the root over all branhces
                    cout<<"Estimating the root position on all branches using fast method ..."<<endl;
                    r=estimate_root_with_constraint_fast_rooted(opt,nodes);
                }
                else{ //forget the given root and re-estimate the position of the root over all branhces
                    cout<<"Estimating the root position on all branches ..."<<endl;
                    r=estimate_root_with_constraint_rooted(opt,nodes);
                }
                if (r>0){
                    Node** nodes_new = cloneLeaves(opt,nodes,0);
                    vector<int>::iterator iter=nodes[0]->suc.begin();
                    int s1=(*iter);
                    iter++;
                    int s2=(*iter);
                    for (int i=opt->nbINodes; i<=opt->nbBranches; i++) {
                        nodes_new[i]->status=nodes[i]->status;
                    }
                    reroot_rootedtree(br,r,s1,s2,opt,nodes,nodes_new);
                    with_constraint_active_set_lambda_multirates(br,opt,nodes_new,true);
                    //br=(nodes[s1]->D+nodes[s2]->D-2*nodes[0]->D)*opt->rho;
                    output(br,y,opt,nodes_new,result,tree1,tree2,tree3);
                    for (int i=0;i<opt->nbBranches+1;i++) delete nodes_new[i];
                    delete[] nodes_new;
                }
            }
        }
    }
    for (int i=0;i<opt->nbBranches+1;i++) delete nodes[i];
    delete[] nodes;
    delete opt;
    elapsed_time = (double)(clock()-start)/CLOCKS_PER_SEC;
    fprintf(result, "\n*********************************************************\n" );
    fprintf(result, "Elapsed time: %.2f seconds\n", elapsed_time);
    fprintf(tree1,"End;");
    cout<<"Elapsed time: "<<elapsed_time<<" seconds"<<endl;;
    fclose(tree);
    fclose(result);
    fclose(tree1);
    fclose(tree2);
    fclose(tree3);
    if (opt->rate!="") fclose(gr);
    return EXIT_SUCCESS;
}
