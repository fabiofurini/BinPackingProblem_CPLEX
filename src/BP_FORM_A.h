#ifndef BP_FormA_HEADER
#define BP_FormA_HEADER


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


#include "global_variables.h"
#include "global_functions.h"


using namespace std;

/*****************************************************************/
int position_x(int i,int b,int item_number,int bin_number);
/*****************************************************************/

/*****************************************************************/
int position_y(int b,int item_number,int bin_number);
/*****************************************************************/

/*****************************************************************/
void BPP_load_cplex(data *BPP_instance);
/*****************************************************************/

/*****************************************************************/
int	 BPP_solve_cplex(data *BPP_instance);
/*****************************************************************/

/*****************************************************************/
void BPP_free_cplex(data *BPP_instance);
/*****************************************************************/


#endif
