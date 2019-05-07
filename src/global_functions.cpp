

#include "global_functions.h"



/***********************************************************************************/
void bin_packing_instance_free(data *BPP_instance)
/***********************************************************************************/
{
	delete []BPP_instance->weights;
	delete []BPP_instance->vector_item_bin;

	free(BPP_instance->istname);

}


/***********************************************************************************/
void bin_packing_instance_read(data *BPP_instance)
/***********************************************************************************/
{


	ifstream in(BPP_instance->istname);

	if(in.fail())
	{
		cout << "Instance reading ERROR" << endl ;
		exit(1);
	}

	in >> BPP_instance->item_number;
	in >> BPP_instance->capacity;

	BPP_instance->weights=new double[BPP_instance->item_number];

	for (int i = 0; i < BPP_instance->item_number; i++){
		in >> BPP_instance->weights[i];
	}

	in.close();

	cout << "ItemTypeNumber\t\t" << BPP_instance->item_number << endl;
	cout << "Capacity\t\t" << BPP_instance->capacity << endl;

	cout << "Weights:\n";
	for (int i = 0; i < BPP_instance->item_number; i++){
		cout << "item\t"<< i << "\t weight \t" << BPP_instance->weights[i] << "\n";
	}


}




