

#include "BP_FORM_A.h"


//#define set_covering_version
#define set_partitioning_version

#define print_solution
#define write_lp


/***********************************************************************************/
int position_x(int i,int b,int item_number,int bin_number)
/***********************************************************************************/
{
	return bin_number*i+b;
}

/***********************************************************************************/
int position_y(int b,int item_number,int bin_number)
/***********************************************************************************/
{
	return (item_number*bin_number) + b;
}

/***********************************************************************************/
void BPP_load_cplex(data *BPP_instance)
/***********************************************************************************/
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * setting the CPLEX environment

	//opening the environment
	BPP_instance->env=CPXopenCPLEX(&(BPP_instance->status));
	if(BPP_instance->status!=0)
	{
		printf("cannot open CPLEX environment\n");
		exit(-1);
	}

	//opening the pointer to the problem
	BPP_instance->lp=CPXcreateprob(BPP_instance->env,&(BPP_instance->status),"BPP");
	if(BPP_instance->status!=0)
	{
		printf("cannot create problem\n");
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * creating the variables *
	BPP_instance->ccnt=BPP_instance->item_number*BPP_instance->heur_bin_number+BPP_instance->heur_bin_number;
	BPP_instance->obj=(double*) calloc(BPP_instance->ccnt,sizeof(double));
	BPP_instance->lb=(double*) calloc(BPP_instance->ccnt,sizeof(double));
	BPP_instance->ub=(double*) calloc(BPP_instance->ccnt,sizeof(double));
	BPP_instance->xctype=(char*) calloc(BPP_instance->ccnt,sizeof(char));


	BPP_instance->colname=(char**) calloc(BPP_instance->ccnt,sizeof(char*));
	for(int i=0;i<BPP_instance->ccnt;i++){BPP_instance->colname[i]=(char*) calloc(2000,sizeof(char));}

	int counter=0;
	for(int i=0; i<BPP_instance->item_number; i++)
	{
		for(int b=0; b<BPP_instance->heur_bin_number; b++)
		{

			BPP_instance->obj[counter]=0.0;
			BPP_instance->lb[counter]=0.0;
			BPP_instance->ub[counter]=1.0;
			BPP_instance->xctype[counter]='B';
			//cout << "POSITION X\t" << position_x(i,b,BPP_instance->item_number,BPP_instance->heur_bin_number) << endl;
			sprintf(BPP_instance->colname[counter], "x%d.%d",i+1,b+1);
			counter++;
		}
	}

	for(int b=0; b<BPP_instance->heur_bin_number; b++)
	{

		BPP_instance->obj[counter]=1.0;
		BPP_instance->lb[counter]=0.0;
		BPP_instance->ub[counter]=1.0;
		BPP_instance->xctype[counter]='B';
		//cout << "POSITION Y\t" << position_y(b,BPP_instance->item_number,BPP_instance->heur_bin_number) << endl;
		sprintf(BPP_instance->colname[counter], "y%d",b+1);
		counter++;
	}

	BPP_instance->status=CPXnewcols(BPP_instance->env,BPP_instance->lp,BPP_instance->ccnt,BPP_instance->obj,BPP_instance->lb,BPP_instance->ub,BPP_instance->xctype,BPP_instance->colname);
	if(BPP_instance->status!=0)
	{
		printf("error in CPXnewcols\n");
		exit(-1);
	}

	free(BPP_instance->obj);
	free(BPP_instance->lb);
	free(BPP_instance->ub);
	free(BPP_instance->xctype);

	for(int i=0;i<BPP_instance->ccnt;i++){free(BPP_instance->colname[i]);}
	free(BPP_instance->colname);


	// * setting the objective function in the minimization form
	CPXchgobjsen(BPP_instance->env,BPP_instance->lp,CPX_MIN);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	for(int b=0; b<BPP_instance->heur_bin_number; b++)
	{
		// * creating the knapsack constraint *
		BPP_instance->rcnt=1;
		BPP_instance->nzcnt=BPP_instance->item_number+1;
		BPP_instance->rhs=(double*) calloc(BPP_instance->rcnt,sizeof(double));
		BPP_instance->sense=(char*) calloc(BPP_instance->rcnt,sizeof(double));

		BPP_instance->rhs[0]=0.0;
		BPP_instance->sense[0]='L';


		BPP_instance->rmatbeg=(int*) calloc(BPP_instance->rcnt,sizeof(int));
		BPP_instance->rmatind=(int*) calloc(BPP_instance->nzcnt,sizeof(int));
		BPP_instance->rmatval=(double*) calloc(BPP_instance->nzcnt,sizeof(double));

		for(int i=0; i<BPP_instance->item_number; i++)
		{
			BPP_instance->rmatval[i]=BPP_instance->weights[i];
			BPP_instance->rmatind[i]=position_x(i,b,BPP_instance->item_number,BPP_instance->heur_bin_number);
		}

		BPP_instance->rmatval[BPP_instance->item_number]=-BPP_instance->capacity;
		BPP_instance->rmatind[BPP_instance->item_number]=position_y(b,BPP_instance->item_number,BPP_instance->heur_bin_number);


		BPP_instance->rmatbeg[0]=0;

		BPP_instance->status=CPXaddrows(BPP_instance->env,BPP_instance->lp,0,BPP_instance->rcnt,BPP_instance->nzcnt,BPP_instance->rhs,BPP_instance->sense,BPP_instance->rmatbeg,BPP_instance->rmatind,BPP_instance->rmatval,NULL,NULL);
		if(BPP_instance->status!=0)
		{
			printf("error in CPXaddrows\n");
			exit(-1);
		}

		free(BPP_instance->rmatbeg);
		free(BPP_instance->rmatval);
		free(BPP_instance->rmatind);
		free(BPP_instance->rhs);
		free(BPP_instance->sense);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	for(int i=0; i<BPP_instance->item_number; i++)
	{
		// * creating assignment constraints *
		BPP_instance->rcnt=1;
		BPP_instance->nzcnt=BPP_instance->heur_bin_number;
		BPP_instance->rhs=(double*) calloc(BPP_instance->rcnt,sizeof(double));
		BPP_instance->sense=(char*) calloc(BPP_instance->rcnt,sizeof(double));

		BPP_instance->rhs[0]=1.0;

#ifdef 	set_partitioning_version
		BPP_instance->sense[0]='E';
#endif

#ifdef 	set_covering_version
		BPP_instance->sense[0]='G';
#endif

		BPP_instance->rmatbeg=(int*) calloc(BPP_instance->rcnt,sizeof(int));
		BPP_instance->rmatind=(int*) calloc(BPP_instance->nzcnt,sizeof(int));
		BPP_instance->rmatval=(double*) calloc(BPP_instance->nzcnt,sizeof(double));

		for(int b=0; b<BPP_instance->heur_bin_number; b++)
		{
			BPP_instance->rmatval[b]=1.0;
			BPP_instance->rmatind[b]=position_x(i,b,BPP_instance->item_number,BPP_instance->heur_bin_number);
		}


		BPP_instance->rmatbeg[0]=0;

		BPP_instance->status=CPXaddrows(BPP_instance->env,BPP_instance->lp,0,BPP_instance->rcnt,BPP_instance->nzcnt,BPP_instance->rhs,BPP_instance->sense,BPP_instance->rmatbeg,BPP_instance->rmatind,BPP_instance->rmatval,NULL,NULL);
		if(BPP_instance->status!=0)
		{
			printf("error in CPXaddrows\n");
			exit(-1);
		}

		free(BPP_instance->rmatbeg);
		free(BPP_instance->rmatval);
		free(BPP_instance->rmatind);
		free(BPP_instance->rhs);
		free(BPP_instance->sense);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef write_lp
	// * writing the created ILP model on a file *
	BPP_instance->status=CPXwriteprob(BPP_instance->env,BPP_instance->lp,"BPP.lp",NULL);
	if(BPP_instance->status!=0) {
		printf("error in CPXwriteprob\n");
		exit(-1);
	}
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////




}

/***********************************************************************************/
void BPP_free_cplex(data *BPP_instance)
/***********************************************************************************/
{


	BPP_instance->status=CPXfreeprob(BPP_instance->env,&(BPP_instance->lp));
	if(BPP_instance->status!=0) {printf("error in CPXfreeprob\n");exit(-1);}

	BPP_instance->status=CPXcloseCPLEX(&(BPP_instance->env));
	if(BPP_instance->status!=0) {printf("error in CPXcloseCPLEX\n");exit(-1);}

}

/***********************************************************************************/
int BPP_solve_cplex(data *BPP_instance)
/***********************************************************************************/
{


	CPXsetintparam (BPP_instance->env, CPX_PARAM_SCRIND, CPX_ON);

	BPP_instance->number_of_CPU=1;

//	// * Set relative tolerance *
//	BPP_instance->status = CPXsetdblparam (BPP_instance->env, CPX_PARAM_EPAGAP, 0.0);
//	if (BPP_instance->status)
//	{
//		printf ("error for CPX_PARAM_EPAGAP\n");
//	}
//
//	// * Set a tolerance *
//	BPP_instance->status = CPXsetdblparam (BPP_instance->env, CPX_PARAM_EPGAP, 0.0);
//	if (BPP_instance->status)
//	{
//		printf ("error for CPX_PARAM_EPGAP\n");
//	}
//
//
//	// * Set mip tolerances integrality *
//	BPP_instance->status = CPXsetdblparam (BPP_instance->env, CPX_PARAM_EPINT, 0.0);
//	if (BPP_instance->status)
//	{
//		printf ("error for CPX_PARAM_EPINTP\n");
//	}
//
//	// * Set Feasibility tolerance *
//	BPP_instance->status = CPXsetdblparam (BPP_instance->env, CPX_PARAM_EPRHS, 1e-9);
//	if (BPP_instance->status)
//	{
//		printf ("error for CPX_PARAM_EPRHS\n");
//	}

	// * Set number of CPU*

	BPP_instance->status = CPXsetintparam (BPP_instance->env, CPX_PARAM_THREADS, BPP_instance->number_of_CPU);
	if (BPP_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

	// * Set time limit *

	BPP_instance->status = CPXsetdblparam (BPP_instance->env, CPX_PARAM_TILIM,BPP_instance->timeLimit);
	if (BPP_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}



	///////////////////////////////////////////////////////////////////////////////////
	// * solving the MIP model
	clock_t time_start=clock();


	BPP_instance->status=CPXmipopt(BPP_instance->env,BPP_instance->lp);
	if(BPP_instance->status!=0)
	{
		printf("error in CPXmipopt\n");
		exit(-1);
	}

	clock_t time_end=clock();
	double solution_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	///////////////////////////////////////////////////////////////////////////////////


	// * getting the solution

	BPP_instance->x=(double*) calloc(BPP_instance->item_number*BPP_instance->heur_bin_number+BPP_instance->heur_bin_number,sizeof(double));


	BPP_instance->status=CPXgetmipx(BPP_instance->env,BPP_instance->lp,BPP_instance->x,0,BPP_instance->item_number*BPP_instance->heur_bin_number+BPP_instance->heur_bin_number-1);
	if(BPP_instance->status!=0)
	{
		printf("error in CPXgetmipx\n");
		exit(-1);
	}

	BPP_instance->status=CPXgetmipobjval(BPP_instance->env,BPP_instance->lp,&(BPP_instance->objval));
	if(BPP_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
		exit(-1);
	}

	printf("\n\nMIP solution value ->\t\%f",BPP_instance->objval);

#ifdef print_solution
	printf("\n\nSolution\n");
	for (int b = 0; b < BPP_instance->heur_bin_number; b++)
	{
		printf("bins %d -> %d\n",b+1 ,(int)(BPP_instance->x[position_y(b,BPP_instance->item_number,BPP_instance->heur_bin_number)]+0.5));
	}
	printf("\n\n");

	cout << "Position items in bins\n";
	for (int i = 0; i < BPP_instance->item_number; i++){
		for (int b = 0; b < BPP_instance->heur_bin_number; b++)
		{
			printf("%d",(int)(BPP_instance->x[position_x(i,b,BPP_instance->item_number,BPP_instance->heur_bin_number)]+0.5));
		}
		printf("\n");
	}
	printf("\n");
#endif

	BPP_instance->status=CPXgetbestobjval(BPP_instance->env,BPP_instance->lp,&(BPP_instance->bestobjval));
	if(BPP_instance->status!=0)
	{
		printf("error in CPXgetbestobjval\n");
		exit(-1);
	}

	BPP_instance->lpstat=CPXgetstat(BPP_instance->env,BPP_instance->lp);
	BPP_instance->nodecount = CPXgetnodecnt(BPP_instance->env, BPP_instance->lp);


	///////////////////////////////////////////////////////////////////////////////////
	/* linear programming relaxation*/

	CPXsetintparam (BPP_instance->env, CPX_PARAM_SCRIND, CPX_OFF);

	double solution_time_lp=0;

	BPP_instance->status = CPXchgprobtype (BPP_instance->env, BPP_instance->lp, CPXPROB_LP);

	clock_t time_start_lp=clock();
	BPP_instance->status=CPXlpopt(BPP_instance->env,BPP_instance->lp);
	if(BPP_instance->status!=0)
	{
		printf("err_FILEor in CPXlpopt slave solve\n");
		exit(-1);
	}

	clock_t time_end_lp=clock();

	solution_time_lp=(double)(time_end_lp-time_start_lp)/(double)CLOCKS_PER_SEC;

	double cplex_lp;

	BPP_instance->status=CPXgetobjval(BPP_instance->env,BPP_instance->lp,&cplex_lp);
	if(BPP_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
		exit(-1);
	}

	printf("\n\nLP solution value ->\t\%f",cplex_lp);

	BPP_instance->status=CPXgetmipx(BPP_instance->env,BPP_instance->lp,BPP_instance->x,0,BPP_instance->item_number*BPP_instance->heur_bin_number+BPP_instance->heur_bin_number-1);
	if(BPP_instance->status!=0)
	{
		printf("error in CPXgetmipx\n");
		exit(-1);
	}

#ifdef print_solution
	printf("\n\nSolution\n");
	for (int b = 0; b < BPP_instance->heur_bin_number; b++)
	{
		printf("bins %d -> %f\n",b+1 ,(BPP_instance->x[position_y(b,BPP_instance->item_number,BPP_instance->heur_bin_number)]));
	}
	printf("\n\n");
#endif

	///////////////////////////////////////////////////////////////////////////////////

	BPP_instance->cur_numcols=CPXgetnumcols(BPP_instance->env,BPP_instance->lp);
	BPP_instance->cur_numrows=CPXgetnumrows(BPP_instance->env,BPP_instance->lp);

	printf("\nnumcols\t%d\n",BPP_instance->cur_numcols);
	printf("\nnumrows\t%d\n",BPP_instance->cur_numrows);


	ofstream compact_file;
	compact_file.open("info_BPP.txt", ios::app);
	compact_file << fixed
			<< BPP_instance->istname << "\t"

			<< BPP_instance->item_number << "\t"
			<< BPP_instance->heur_bin_number << "\t"

			<< BPP_instance->objval << "\t"
			<< BPP_instance->bestobjval << "\t"
			<< solution_time << "\t"

			<< BPP_instance->lpstat << "\t"
			<< BPP_instance->nodecount << "\t"

			<<  BPP_instance->cur_numcols<< "\t"
			<<  BPP_instance->cur_numrows<< "\t"

			<< endl;
	compact_file.close();


	free(BPP_instance->x);

	return BPP_instance->objval;

}

