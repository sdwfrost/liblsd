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
#include "options.h"

Pr* getOptions( int argc, char** argv )
{
    if( argc>1 )
        return getCommandLine( argc, argv );
    else
        return getInterface();
}

Pr* getCommandLine( int argc, char** argv)
{
    Pr* opt = new Pr();
    int c;
    string s;
    bool iflag = false,
    dflag = false,
    flagA=false,
    flagZ=false,
    sflag=false,
    fflag=false;
    while ( (c = getopt(argc, argv, ":i:d:o:s:n:g:r:v:ct:w:b:ha:z:f:kp:")) != -1 )
    {
        switch (c)
        {
            case 'i':
                if( access( optarg, R_OK )!=0 )
                    myExit( "Cannot read the file named \"%s\"\n", optarg );
                opt->inFile = optarg;
                iflag = true;
                break;
            case 'd':
                if( access( optarg, R_OK )!=0 )
                    myExit( "Cannot read the file named \"%s\"\n", optarg );
                opt->inDateFile = optarg;
                dflag = true;
                break;
            case 'p':
                if( access( optarg, R_OK )!=0 )
                    myExit( "Cannot read the file named \"%s\"\n", optarg );
                opt->partitionFile = optarg;
                break;
            case 'o':
                opt->outFile = optarg;
                break;
            case 's':
                if( !isInteger(optarg) )
                    myExit("Argument of option -s must be an integer.\n");
                opt->seqLength = atoi(optarg);
                if( opt->seqLength<1 )
                    myExit("Argument of option -s must be strictly positive.\n");
                sflag = true;
                break;
            case 'n':
                if( !isInteger(optarg) )
                    myExit("Argument of option -n must be an integer.\n");
                opt->nbData = atoi(optarg);
                if( opt->nbData<1 )
                    myExit("Argument of option -n must be strictly positive.\n");
                break;
            case 'g':
                if( access( optarg, R_OK )!=0 )
                    myExit( "Cannot read the file named \"%s\"\n", optarg );
                opt->fnOutgroup = optarg;
                break;
            case 'k':
                if (opt->fnOutgroup!=""){
                	opt->keepOutgroup=true;
                	opt->estimate_root="k";
                }
                break;
            case 'r':
                opt->estimate_root = optarg;
                if (opt->estimate_root.compare("l")!=0 && opt->estimate_root.compare("a")!=0 && opt->estimate_root.compare("as")!=0)
                    myExit("Argument of option -r must be either \"l\" or \"a\" or \"as\".\n");
                break;
            case 'v':
                if( !isInteger(optarg) )
                    myExit("Argument of option -v must be either 1 or 2.\n");
                opt->variance = atoi(optarg);
                if (opt->variance!=1 && opt->variance!=2){
                    myExit("Argument of option -v must be either 1 (for using orginal branches to compute variance) or 2 (LSD will be run twice, the second time uses the variances based on the estimated branch lengths of the first time).\n");
                }
                break;
            case 'c':
                opt->constraint = true;
                break;
            case 'b':
                if( !isInteger(optarg) )
                    myExit("Argument of option -b must be an integer.\n");
                opt->c = atoi( optarg );
                if (opt->rho_min<0)
                    myExit("Argument of option -b must not be negative.\n");
                break;
            case 't':
                if( !isReal(optarg) )
                    myExit("Argument of option -t must be a real.\n");
                opt->rho_min = atof(optarg);
                if (opt->rho_min<=0)
                    myExit("Argument of option -t must be strictly positive.\n");
                break;
            case 'w':
                if( access( optarg, R_OK )!=0 )
                    myExit( "Cannot read the file named \"%s\"\n", optarg );
                opt->rate = optarg;
                opt->givenRate = true;
                break;
            case 'a':
                if( !isInteger(optarg) )
                    myExit("Argument of option -a must be an integer.\n");
                opt->mrca=atof(optarg);
                flagA=true;
                break;
            case 'z':
                if( !isInteger(optarg) )
                    myExit("Argument of option -z must be an integer.\n");
                opt->leaves=atof(optarg);
                flagZ=true;
                break;
            case 'h':
                printHelp();
                exit( EXIT_SUCCESS );
                break;
            case 'f':
                if( !isInteger(optarg) )
                    myExit("Argument of option -f must be the number of samplings to compute confidence intervals, e.g. 100 ...\n");
                opt->nbSampling = atoi( optarg );
                if (opt->nbSampling<0)
                    myExit("Argument of option -f must be a positive integer.\n");
                opt->ci=true;
                fflag = true;
                break;
            case '?':
                myExit("Unrecognized option: -%c\n", optopt);
            case ':':
                if (optopt=='v') myExit("Argument of option -v must be either 1 (for using orginal branches to compute variance) or 2 (for using branches estimated by LSD to compute variance, i.e LSD will be run 2 times: the first time is just used to compute the variance).\n");
                else myExit("Option -%c requires an operand\n", optopt );
            default:
                myExit("?? getopt returned character code 0%o ??\n", c );
        }
    }
    if( !(iflag) )
        myExit("Argument -i is necessary to continue.\n");
    if (!dflag && (flagA && !flagZ)){
        myExit("The input date file is not provided, so option -z is required to use with option -a to estimate relative dates.\n");
    }
    if (!dflag && (!flagA && flagZ)){
        myExit("The input date file is not provided, so option -a is required to use with option -z to estimate relative dates.\n");
    }
    if (!dflag && (!flagA && !flagZ)){
        opt->relative=true;
        opt->mrca=0;
        opt->leaves=1;
    }
    if (!dflag && (flagA && flagZ)){
        if (opt->mrca >= opt->leaves)
            myExit("The root date must be strictly smaller than the tips date.\n");
        opt->relative=true;
    }
    if (fflag && !sflag) {
        myExit("Argument -s is required to calculate confidence intervals.\n");
    }
    if (dflag)
        opt->relative=false;
    checkRooted(opt);
    if (!opt->rooted){
        if (opt->estimate_root=="" && opt->fnOutgroup=="") {
            cout<<"The input trees are not rooted, use either option -g to specify the outgroups file or -r to estimate the root"<<endl;
            exit(EXIT_FAILURE);
        }
    }
    if (!opt->rooted && opt->fnOutgroup=="" && opt->estimate_root!="" && opt->estimate_root.compare("l")==0) {
        cout<<"The input trees are not rooted, so the \"l\" method for rooting function is turned to the \"a\" method."<<endl;
        opt->estimate_root="a";
    }
    if (!opt->constraint && opt->estimate_root.compare("as")==0){
        cout<<"The non constrained mode is chosen, so the \"as\" method for rooting function is the same as the \"a\" method."<<endl;
        opt->estimate_root="a";
    }
    if( opt->outFile=="") opt->outFile = opt->inFile + ".result";
    return opt;
}

Pr* getInterface()
{
    Pr* opt = new Pr();
    opt->inFile = getInputFileName("Enter your Input Tree File name> ");
    checkRooted(opt);
    cout<<"Do you have a date file? y/n "<<endl;
    char letter[3];
    do{
        fgets(letter,3,stdin);
        if (*letter=='n' || *letter=='N') {
            cout<<"There is no date file, so the program will estimate relative dates with root date = 0 and tips date = 1. Type 'y' to continue or 'n' to modify the root date and the tips date"<<endl;
            char letter1[3];
            do {
                fgets( letter1, 3, stdin );
                if (*letter1=='n' || *letter1=='N'){
                    do {
                        opt->mrca = getInputReal("Enter the root date (default=0)> ");
                        opt->leaves = getInputReal("Enter the tips date (default=1)> ");
                        if (opt->leaves <= opt->mrca) cout<<"Root date must be smaller than the tips date."<<endl;
                    } while (opt->leaves <= opt->mrca);
                }
                else if (*letter1=='y' || *letter1=='Y'){
                    opt->mrca=0;
                    opt->leaves=1;
                }
                else {
                    cout<<"Type 'y' to continue or 'n' to modify the root date and tips date"<<endl;
                }
                opt->relative=true;
            } while (*letter1!='n' && *letter1!='N' && *letter1!='y' && *letter1!='Y');
        }
        else if (*letter=='y' || *letter=='Y'){
            opt->inDateFile = getInputFileName("Enter you input date file name>");
            opt->relative=false;
        }
    } while (*letter!='n' && *letter!='N' && *letter!='y' && *letter!='Y');
    opt->outFile = opt->inFile+".result";
    if (!opt->rooted) {
        opt->estimate_root="a";
    }
    char letter2[3];
    bool sflag = false;
    bool fflag = true;
    bool vflag = true;
    bool cont = true;
    do
    {
        printInterface( stdout, opt );
        cout<<endl;
        fgets( letter2, 3, stdin );
        if( isOptionActivate( opt, *letter2 ) ){
            setOptionsWithLetter( opt, *letter2);
            if (*letter2=='s' || *letter2=='S') sflag = true;
            if ((*letter2=='y' || *letter2=='Y')){
                if ((opt->ci || opt->variance) && !sflag){
                    do{
                        cout<<"Sequence length is required but not specified by option S. Do you want to take the default value 1000? y/n"<<endl;
                        fgets( letter2, 3, stdin );
                    }  while (*letter2!='y' && *letter2!='Y' && *letter2!='n' && *letter2 != 'N');
                    if ((*letter2=='y' || *letter2=='Y')) cont = false;
                    else if ((*letter2=='n' || *letter2=='N')){
                        setOptionsWithLetter( opt, 'S');
                        sflag = true;
                    }
                }
                else cont = false;
            }
        }
    } while( cont );
    return opt;
}


void printInterface( FILE* in, Pr* opt)
{
    fprintf(in,"\nLEAST-SQUARE METHODS TO ESTIMATE RATES AND DATES - " VERSION" \n\n");
    fprintf(in,"\nInput files:\n");
    fprintf(in,"  I                                 Input tree file : %s\n",opt->inFile.c_str());
    if (opt->relative==true)
        fprintf(in,"  D                         Estimate relative dates : mrca date = %.2f, tips date =%.2f\n",opt->mrca,opt->leaves);
    else
        fprintf(in,"  D                                 Input date file : %s\n",opt->inDateFile.c_str());
    fprintf(in,"  P                                  Partition file : ");
    if (opt->partitionFile=="")        fprintf(in,"No\n");
    else fprintf(in,"%s\n",opt->partitionFile.c_str());
    fprintf(in,"Output file:\n");
    fprintf(in,"  O                                    Output file  : %s\n",opt->outFile.c_str());
    fprintf(in,"Parameters:\n");
    fprintf(in,"  C                                With constraints : ");
    if (!opt->constraint) fprintf(in,"No\n");
    else {
        fprintf(in,"Yes\n");
    }
    fprintf(in,"  T                        Lower bound for the rate : %.3e\n",opt->rho_min);
    fprintf(in,"  V                                  With variances : ");
    if (opt->variance==0) fprintf(in,"No\n");
    else {
        if (opt->variance==1) fprintf(in,"Yes, use variances based on original branches\n");
        else if (opt->variance==2) fprintf(in,"Yes, use variances based on branches estimated by LSD\n");
        fprintf(in,"  B                          Parameter of variances : %d\n",opt->c);
    }
    fprintf(in,"  S                                 Sequence Length : %i\n",opt->seqLength);
    fprintf(in,"  R                               Estimate the root : ");
    if (opt->estimate_root==""){
        fprintf(in,"No\n");
    }
    else if (opt->estimate_root.compare("l")==0){
        fprintf(in,"Around the given root\n");
    }
    else if (opt->estimate_root.compare("a")==0 && opt->constraint){
        fprintf(in,"Use fast method to search on all branches\n");
    }
    else if (opt->estimate_root.compare("a")==0 && !opt->constraint){
        fprintf(in,"Search on all branches\n");
    }
    else if (opt->estimate_root.compare("as")==0){
        fprintf(in,"Use constrained mode on all branches\n");
    }
    fprintf(in,"  W                         Given substitution rate : ");
    if (opt->rate=="") fprintf(in,"No\n");
    else fprintf(in,"%s\n",opt->rate.c_str());
    if (opt->fnOutgroup=="")
        fprintf(in,"  G                                 Given outgroups : No\n");
    else {
        fprintf(in,"  G                         File contains outgroups : %s\n",opt->fnOutgroup.c_str());
        if (opt->keepOutgroup) {
            fprintf(in,"  K           Keep outgroups in the estimating tree :  Yes\n");
        }
        else{
            fprintf(in,"  K           Keep outgroups in the estimating tree : No\n");
        }
    }
    fprintf(in,"  N                               Multiple data set : ");
    if( opt->nbData< 2 )
        fprintf(in,"No\n");
    else
        fprintf(in,"Yes, %i data sets\n",opt->nbData);
    fprintf(in,"  F                    Compute confidence intervals : ");
    if (opt->ci)
        fprintf(in,"Yes, sampling %d times\n",opt->nbSampling);
    else
        fprintf(in,"No\n");
    fprintf(in,"\n  H to print Help ");
    fprintf(in,"\n  Y to accept or type a letter to change an option (Q = Exit) ");
}

void printHelp( void )
{
    printf(BOLD"LSD: LEAST-SQUARES METHODS TO ESTIMATE RATES AND DATES - " VERSION" by Thu-Hien To\n\n");
    printf(BOLD"DESCRIPTION\n"
           FLAT"\tThis program estimates the rate and the dates of the input phylogenies given some temporal constraints.\n"
           FLAT"\tIt minimizes the square errors of the branch lengths under normal distribution model.\n\n"
           );
    printf(BOLD"SYNOPSIS\n"
           FLAT"\t" BOLD"./lsd " FLAT"[" BOLD"-i " LINE"inputFile" FLAT"] "
           FLAT"[" BOLD"-d " LINE"inputDateFile" FLAT"] "
           FLAT"[" BOLD"-o " LINE"outputFile" FLAT"] "
           FLAT"[" BOLD"-c" FLAT"] "
           FLAT"[" BOLD"-v " LINE"mode" FLAT"] "
           FLAT"[" BOLD"-s " LINE"sequenceLength" FLAT"] "
           FLAT"[" BOLD"-n " LINE"datasetNumber" FLAT"]\n"
           FLAT"\t     [" BOLD"-t " LINE"lowerBoundRate" FLAT"] "
           FLAT"[" BOLD"-r " LINE"rootingMethod" FLAT"] "
           FLAT"[" BOLD"-b " LINE"varianceParameter" FLAT"] "
           FLAT"[" BOLD"-w " LINE"givenRateFile" FLAT"] "
           FLAT"[" BOLD"-g " LINE"outgroupFile" FLAT"] "
	   FLAT"[" BOLD"-f " LINE"nbSamplings" FLAT"] "
           FLAT"[" BOLD"-h" FLAT"]\n"
           FLAT"\n");
    
    printf(BOLD"OPTIONS\n"
           FLAT"\t" BOLD"-a " LINE"root date\n"
           FLAT"\t   If the dates of all tips are equal (which is given by option -z), you must use this option to provide the root date.\n"
           FLAT"\t   In this case, the input date file can be omitted, and the program estimates only the relative dates based on the given\n"
           FLAT"\t   root date and tips date. By default, T[root]=0 and T[tips]=1.\n"
           FLAT"\t" BOLD"-b " LINE"parameter of variances\n"
           FLAT"\t   The parameter to compute the variances in option -v. By default b=10.\n"
           FLAT"\t" BOLD"-c \n"
           FLAT"\t   By using this option, we impose the constraints that the date of every node is equal or smaller then\n"
           FLAT"\t   the dates of its descendants. Without constraints, the runtime is linear (LD). With constraints, the\n"
           FLAT"\t   problem is a quadratic programming and is solved efficiently by the active-set method.\n"
           FLAT"\t" BOLD"-d " LINE"inputDateFile\n"
           FLAT"\t   This options is used to read the name of the input date file which contains temporal constraints of internal nodes\n"
           FLAT"\t   or tips. An internal node can be defined either by its label (given in the input tree) or by a subset of tips that have it as \n"
           FLAT"\t   the most recent common ancestor (mrca).\n"
           FLAT"\t   The first line of this file is the number of temporal constraints. A temporal constraint can be a real, or a \n"
           FLAT"\t   lower bound " FLAT LINE"l(value)" FLAT", or an upper bound " FLAT LINE"u(value)" FLAT", or an interval " LINE"b(v1,v2)\n"
           FLAT"\t   For example, if the input tree has 4 taxa a,b,c,d, and an internal node named in, then following is a possible date file:\n"
           FLAT"\t    6\n"
           FLAT"\t    a l(2003)\n"
           FLAT"\t    b u(2007)\n"
           FLAT"\t    c 2005\n"
           FLAT"\t    d b(2001,2007)\n"
           FLAT"\t    mrca(a,b,c,d) b(2000,2001)\n"
	   FLAT"\t    in l(2004)\n"
	   FLAT"\t   If this option is omitted, the program will estimate relative dates by giving T[root]=0 and T[tips]=1.\n"
           FLAT"\t" BOLD"-f " LINE"samplingNumber\n"
           FLAT"\t   This option calculates the confidence intervals of the estimated rate and dates. The branch lengths of the esimated\n"
           FLAT"\t   tree are sampled " FLAT LINE"samplingNumber" FLAT " times to generate a set of trees. We use Poisson distribution to generate \n"
	   FLAT"\t   branch lengths, hence the sequence length should be precised in the option -s. Dating is then processed on each tree. The \n"
	   FLAT"\t   confidence intervals are inferred from these simulated trees and are written in the nexus tree output with the name \"CI\".\n"
           FLAT"\t" BOLD"-g " LINE"outgroupFile\n"
           FLAT"\t   If your data contain outgroups, specify the name of the outgroup file here.\n"
           FLAT"\t   The program will remove the outgroups from the trees and take the ingroup trees as the input. If you want to keep the outgroups,\n"
	   FLAT"\t   then use this option combined with option -k. The format of this file should be:\n"
           FLAT"\t        n\n"
           FLAT"\t        OUTGROUP1\n"
           FLAT"\t        OUTGROUP2\n"
           FLAT"\t        ...\n"
           FLAT"\t        OUTGROUPn\n"
           FLAT"\t" BOLD"-k\n"
           FLAT"\t   Use this option to keep the outgroups (given in option -g) in the estimated tree. The root position is then estimated only on the\n"
           FLAT"\t   branch determined by the outgroups. Otherwise, the outgroups will be removed.\n"
           FLAT"\t" BOLD"-h " LINE"help\n"
           FLAT"\t   Print this message.\n"
           FLAT"\t" BOLD"-i " LINE"inputTreesFile\n"
           FLAT"\t   The name of the input trees file. It contains tree(s) in newick format, each tree on one line. Note that the taxa sets of all\n"
           FLAT"\t   trees must be the same.\n"
           FLAT"\t" BOLD"-n " LINE"datasetNumber\n"
           FLAT"\t   The number of trees that you want to read.\n"
           FLAT"\t" BOLD"-o " LINE"outputFile\n"
           FLAT"\t   The base name of the output files to write the results and the time-scale trees.\n"
           FLAT"\t" BOLD"-p " LINE"partitionFile\n"
           FLAT"\t   The file that specifies the branche partition when the input phylogeny has more than one  substitution rate. In the partition file, each\n"
           FLAT"\t   line contains the name of one part and a list of subtrees whose branches are supposed to have the same substitution rate. Every branch that\n"
           FLAT"\t   is not defined to any subtree will have a same rate which is different from the ones defined in the partition file. A subtree is defined between\n"
           FLAT"\t   {}: its first term corresponds to the root of the subtree, and the following terms (if there any) correspond to the tips of the subtree. If \n"
           FLAT"\t   the tips of the subtree are not defined (so there's only the defined root), then by default this subtree is extended down to the tips\n"
           FLAT"\t   of the full tree. For example the input tree is ((A:0.12,D:0.12)n1:0.3,((B:0.3,C:0.5)n2:0.4,(E:0.5,(F:0.2,G:0.3)n3:0.33)n4:0.22)n5:0.2)root;\n"
           FLAT"\t   and you have the following partition file:\n"
           FLAT"\t         group1 {n1} {n5 n4}\n"
           FLAT"\t         group2 {n3}\n"
           FLAT"\t   then there are 3 rates: the first one is the rate on the branches (n1,A), (n1,D), (n5,n4), (n5,n2), (n2,B), (n2,C), the second one is the rate on\n"
           FLAT"\t   the branches (n3,F), (n3,G), and the last one is the rate on the remaining branches of the tree. Note that if the internal nodes don't have labels,\n"
           FLAT"\t   then they can be defined by mrca of at least two tips, for example n1 is mrca(A,D)\n."
           FLAT"\t" BOLD"-r " LINE"rootingMethod\n"
           FLAT"\t   This option is used to specify the rooting method to estimate the position of the root for unrooted trees, or\n"
           FLAT"\t   re-estimate the root for rooted trees. The principle is to search for the position of the root that minimizes\n"
           FLAT"\t   the objective function.\n"
           FLAT"\t   If the tree is rooted, then either using operand \"l\" for searching the root locally around the given root, or  \"a\" for\n"
           FLAT"\t   searching the root on all branches. Moreover, when the constrained mode is chosen (option -c), method \"a\" first\n"
           FLAT"\t   estimates the root without using the constraints. After that, it uses the constrained mode to improve locally the position\n"
           FLAT"\t   of the root around this pre-estimated root. To use constrained mode on all branches in this case, specify \"as\".\n"
           FLAT"\t   If the tree is not rooted, then the program searches the root on all branches. Similarly for the previous case, if\n"
           FLAT"\t   the constrained mode is chosen, method \"a\" uses only constrained mode to improve the root position around the pre-estimated\n"
           FLAT"\t   root which is computed without constraints. To use constraints on all branches, use \"as\".\n"
           FLAT"\t" BOLD"-s " LINE"sequenceLength\n"
           FLAT"\t   This option is used to specify the sequence length to compute the variances in the option -v.\n"
           FLAT"\t   By default it is 1000.\n"
           FLAT"\t" BOLD"-t " LINE"lower bound for the rate\n"
           FLAT"\t   This option corresponds to the lower bound for the estimating rate.\n"
           FLAT"\t   It is 1e-10 by default.\n"
           FLAT"\t" BOLD"-v " LINE"variance\n"
           FLAT"\t   Use this option if you want to apply variances for the branch lengths in order to recompense big errors \n"
           FLAT"\t   on long estimated branch lengths. The variance of the branch Bi is Vi = (Bi+b/seq)/seq where b is specified\n"
           FLAT"\t   by option -b and seq is specified by option -s.\n"
           FLAT"\t   If " FLAT LINE"variance" FLAT"=1, then LSD runs once. If " FLAT LINE"variance" FLAT"=2, then LSD runs twice where the second time it uses the variances \n"
           FLAT"\t   based on the estimated branch lengths of the first run.\n"
           FLAT"\t" BOLD"-w " LINE"given rate\n"
           FLAT"\t   This option is used to specify the name of the file containing the substitution rates.\n"
           FLAT"\t   In this case, the program will use the given rates to estimate the dates of the nodes.\n"
           FLAT"\t   This file should have the following format\n"
           FLAT"\t        RATE1\n"
           FLAT"\t        RATE2\n"
           FLAT"\t        ...\n"
           FLAT"\t  where RATEi is the rate of the tree i in the inputTreesFile.\n"
           FLAT"\t" BOLD"-z " LINE"tips date\n"
           FLAT"\t   This option is used to give the date of the tips when they are all equal. It must be used with option -a to give the\n"
           FLAT"\t   root date. In this case the input date file can be omitted, and the program estimates only the relative dates based on\n"
           FLAT"\t   the given root date and tips date. By default, T[root]=0 and T[tips]=1.\n"
           );
}
string getInputString(string msg)
{
    string s;
    cout<<msg<<endl;
    cin>>s;
    return s;
}

string getInputFileName( string msg)
{
    string outfile;
    do
    {
        outfile = getInputString(msg);
        if( access(outfile.c_str(), F_OK)==0 || string(outfile).length()==0)
            break;
        printf( "The file \"%s\" does not exist.\n", outfile.c_str() );
    } while( true );
    if( access(outfile.c_str(), R_OK)!=0 && string(outfile).length()!=0)
        myExit("Could not access to the file named \"%s\" in reading.\n", outfile.c_str() );
    return outfile;
}

string getOutgroupFileName( string msg)
{
    string outfile;
    do
    {
        outfile = getInputString(msg);
        if( access(outfile.c_str(), F_OK)==0 || outfile.compare("")==0)
            break;
        printf( "The file \"%s\" does not exist.\n", outfile.c_str() );
    } while( true );
    if (outfile.compare("")==0) {
        return outfile;
    }
    if( access(outfile.c_str(), R_OK)!=0 )
        myExit("Could not access to the file named \"%s\" in reading.\n", outfile.c_str() );
    return outfile;
}

double getInputReal( string msg )
{
    string word;
    do
    {
        word = getInputString( msg );
        if( isReal(word.c_str()) )
            break;
        myErrorMsg("Your word is not recognized as a real.\n");
    } while( true );
    return atof( word.c_str() );
}

double getInputPositiveReal( string msg )
{
    string word;
    do
    {
        word = getInputString( msg );
        if( isReal(word.c_str()) && atof( word.c_str())>0)
	  break;
        myErrorMsg("Your word is not recognized as a strictly positive real.\n");
    } while( true );
    return atof( word.c_str() );
}

int getInputInteger( string msg )
{
    string word;
    do
    {
        word = getInputString( msg );
        if( isInteger(word.c_str()) )
            break;
        myErrorMsg("Your word is not recognized as an integer.\n");
    } while( true );
    return atoi( word.c_str() );
}

int getPositiveInputInteger( string msg )
{
    int i;
    do
    {
        i = getInputInteger(msg);
        if( i>0 )
            break;
        myErrorMsg("It must be a strictly positive integer.\n");
    } while( true );
    return i;
}

bool isOptionActivate( Pr* opt, char l )
{
    switch(l)
    {
        case 'i':
        case 'I':
        case 'd':
        case 'D':
        case 'o':
        case 'O':
        case 'p':
        case 'P':
        case 's':
        case 'S':
        case 'c':
        case 'C':
        case 'v':
        case 'V':
        case 'b':
        case 'B':
        case 'r':
        case 'R':
        case 'g':
        case 'G':
        case 'k':
        case 'K':
        case 't':
        case 'T':
        case 'w':
        case 'W':
        case 'f':
        case 'F':
        case 'n':
        case 'N':
        case 'y':
        case 'Y':
        case 'q':
        case 'Q':
        case 'h':
        case 'H':
        return true;
    }
    return false;
}

void setOptionsWithLetter( Pr* opt, char letter )
{
    //char* fnOut;
    switch( letter )
    {
        case 'q':
        case 'Q':
            exit( EXIT_SUCCESS );
        case 'i':
        case 'I':
            opt->inFile = getInputFileName("Enter your Input File name> ");
            checkRooted(opt);
            break;
        case 'd':
        case 'D':
            cout<<"Do you have a date file? y/n "<<endl;
            char letter[3];
            do{
                fgets(letter,3,stdin);
                if (*letter=='n' || *letter=='N') {
                    cout<<"There is no date file, so the program will estimate relative dates with root date = 0 and tips date = 1. Type 'y' to continue or 'n' to modify the root date and the tips date"<<endl;
                    char letter1[3];
                    do {
                        fgets( letter1, 3, stdin );
                        if (*letter1=='n' || *letter1=='N'){
                            do {
                                opt->mrca = getInputReal("Enter the root date (default=0)> ");
                                opt->leaves = getInputReal("Enter the tips date (default=1)> ");
                                if (opt->leaves <= opt->mrca) cout<<"Root date must be smaller than the tips date."<<endl;
                            } while (opt->leaves <= opt->mrca);
                        }
                        else if (*letter1=='y' || *letter1=='Y'){
                            opt->mrca=0;
                            opt->leaves=1;
                        }
                        else {
                            cout<<"Type 'y' to continue or 'n' to modify the root date and tips date"<<endl;
                        }
                        opt->relative=true;
                    } while (*letter1!='n' && *letter1!='N' && *letter1!='y' && *letter1!='Y');
                }
                else if (*letter=='y' || *letter=='Y'){
                    opt->inDateFile = getInputFileName("Enter you input date file name>");
                    opt->relative=false;
                }
            } while (*letter!='n' && *letter!='N' && *letter!='y' && *letter!='Y');
            break;
        case 'p':
        case 'P':
            opt->partitionFile = getInputFileName("Enter your Partition File name> ");
            break;
        case 's':
        case 'S':
            opt->seqLength = getPositiveInputInteger("Enter your sequence length> ");
            break;
        case 'n':
        case 'N':
            opt->nbData = getPositiveInputInteger("Enter your number of dataset> ");
            break;
        case 'o':
        case 'O':
            opt->outFile=getInputString("Enter your output file name > ");
            while( access( opt->outFile.c_str(), F_OK )==0){
                cout<<"File "<<opt->outFile<<" already exists. Do you want to overwrite it? Y/N"<<endl;
                char letter[3];
                fgets( letter, 3, stdin );
                if (*letter=='N' || *letter=='n'){
                    opt->outFile = getInputString("Enter your output file name > ");
                }
                if (*letter=='Y' || *letter=='y') {
                    break;
                }
            }
            break;
        case 'c':
        case 'C':
            opt->constraint=!opt->constraint;
            break;
        case 'v':
        case 'V':
            if (opt->variance==0 || opt->variance==1) opt->variance+=1;
            else if (opt->variance==2) opt->variance=0;
            break;
        case 'b':
        case 'B':
            if (opt->variance) opt->c = getPositiveInputInteger("Enter the parameter for the variances> ");
            break;
        case 'r':
        case 'R':
            if (opt->rooted || opt->fnOutgroup!=""){
                if (opt->estimate_root==""){
                    opt->estimate_root="l";
                }
                else if (opt->estimate_root.compare("l")==0){
                    opt->estimate_root="a";
                }
                else if (opt->estimate_root.compare("a")==0 && opt->constraint){
                    opt->estimate_root="as";
                }
                else if (opt->estimate_root.compare("a")==0 && !opt->constraint) opt->estimate_root="";
                else if (opt->estimate_root.compare("as")==0) opt->estimate_root="";
            }
            else {
                if (opt->estimate_root==""){
                    opt->estimate_root="a";
                }
                else if (opt->estimate_root.compare("a")==0 && opt->constraint){
                    opt->estimate_root="as";
                }
                else if (opt->estimate_root.compare("a")==0 && !opt->constraint){
                    cout<<"The trees are not rooted, you must use either option -g to specify the outgroups file or -r to estimate the root"<<endl;
                }
                else if (opt->estimate_root.compare("as")==0){
                    opt->estimate_root="a";
                }
            }
            break;
        case 'g':
        case 'G':
            if (opt->fnOutgroup==""){
                string fo = getOutgroupFileName("Enter the name of the file that contains your outgroups> ");
                if (fo.compare("")!=0) {
                    opt->fnOutgroup=fo;
                    opt->estimate_root="";
                }
            }
            else{
                opt->fnOutgroup="";
                if (!opt->rooted && opt->estimate_root==""){
                    opt->estimate_root="a";
                }
            }
            break;
        case 'k':
        case 'K':
            if (opt->fnOutgroup!=""){
            	opt->keepOutgroup=!opt->keepOutgroup;
            	if (opt->keepOutgroup) {
                	opt->estimate_root="k";
            	}
	    }
            break;
        case 'w':
        case 'W':
            if (opt->rate=="") {
                opt->rate = getInputFileName("Enter the name of the file that contains the rates> ");
                if (string(opt->rate).length()==0) opt->rate="";
                else opt->givenRate=true;
            }
            else{
                opt->rate="";
                opt->givenRate=false;
            }
            break;
        case 't':
        case 'T':
            opt->rho_min = getInputPositiveReal("Enter the lower bound for the rate> ");
            break;
        case 'f':
        case 'F':
            if (!opt->ci) {
                opt->nbSampling = getInputInteger("Enter the number of sampling> ");
                opt->ci=true;
            }
            else{
                opt->ci = false;
            }
            break;
        case 'h':
        case 'H':
            printHelp();
            break;
    }
}




