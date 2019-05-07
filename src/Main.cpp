/*
 *		Main.cpp
 *		Created on: 04/03/2012
 *		Author: Fabio Furini
 */

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

using namespace std;


#include "global_functions.h"
#include "global_variables.h"
#include "BP_FORM_A.h"


/*******************************************************************************************************/
int main(int argc, char** argv)
/*******************************************************************************************************/
{


	data BPP_instance;


	BPP_instance.istname=(char*) calloc(2000,sizeof(char));
	if (argc == 3) {
		strcpy(BPP_instance.istname, argv[1]);
		BPP_instance.timeLimit=atof(argv[2]);
	}
	else {
		printf("ERROR INPUT: \t instance \t time limit\n");
		exit(2);
	}

	cout <<         "---------------------------------------------" << endl ;
	cout << "Instance\t->" << BPP_instance.istname << endl;
	cout <<         "---------------------------------------------" << endl ;
	cout << "Time limit\t->" << BPP_instance.timeLimit << endl;

	//	///////////////////////////////////////////////////////////////////////////////////
	bin_packing_instance_read(&BPP_instance);
	//	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	BPP_instance.heur_bin_number=BPP_instance.item_number;
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	BPP_load_cplex(&BPP_instance);
	printf("\n-> model loaded!\n");
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	BPP_solve_cplex(&BPP_instance);
	printf("\n-> model solved!\n");
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	BPP_free_cplex(&BPP_instance);
	printf("\n-> model freed!\n");
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	bin_packing_instance_free(&BPP_instance);
	///////////////////////////////////////////////////////////////////////////////////


	printf("\nDONE!\n");


	return 1;
}

