#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER


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

using namespace std;


/******************************************************************************************/
void bin_packing_instance_read(data *KP_instance);
/******************************************************************************************/

/******************************************************************************************/
void bin_packing_instance_free(data *BP_instance);
/******************************************************************************************/

#endif
