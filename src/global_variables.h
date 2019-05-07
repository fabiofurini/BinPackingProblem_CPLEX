#ifndef VARIABLE_HEADER
#define VARIABLE_HEADER


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip> // for precision

////////////////////////////////////////////////////////////////////////////////////////////////////
#include </Users/fabiofurini/Applications/IBM/ILOG/CPLEX_Studio127/cplex/include/ilcplex/cplex.h>
//#include </home/fabio/ILOG/CPLEX_Studio_AcademicResearch127/cplex/include/ilcplex/cplex.h>
////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;


typedef struct data
{
	double timeLimit;
	int number_of_CPU;

	//for the first fit heuristic
	int heur_bin_number;
	int *vector_item_bin;

	int item_number;
	double capacity;
	double *weights;

	char *istname;

	int status,ccnt,rcnt,nzcnt,lpstat,nodecount,cur_numrows,cur_numcols;
	int* rmatbeg,*rmatind,*cmatbeg, *cmatind;
	double *rmatval,*cmatval,*rngval,*x,*pi,*obj, *lb, *ub,*rhs,coef_p,objval,bestobjval;
	char *xctype,*sense;
	char **rowname,**colname;

	CPXENVptr env;
	CPXLPptr  lp;

} data;



#endif
