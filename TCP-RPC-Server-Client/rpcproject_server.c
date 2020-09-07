// Kourias Triantafyllos-Dimitrios cs141092
// RPC Server

#include "rpcproject.h"

// RPC Server Function called by RPC Client and do the requested calculation
// In every function the result variable is declared, the struct sent by client and
// after each calculation the struct/variable result is being returned to the RPC Client

float *
average_1_svc(User_array *argp, struct svc_req *rqstp)
{
	static float  result;

	printf("RPC-Client requested Average Value...\n");

	float sum=0.0;
	int i;

	for(i=0;i<argp->userarray_size;i++)
		sum+=argp->userarray.userarray_val[i];

	result=sum/argp->userarray_size;

	printf("...Returning Average value: %1.f\n",result);
	printf("\n");
	return &result;
}

max_min *
maxmin_1_svc(User_array *argp, struct svc_req *rqstp)
{
	static max_min  result;

	printf("RPC-Client requested Max-Min Value...\n");

	int i;

	result.max=-999999;
	result.min=999999;

	for(i=0;i<argp->userarray_size;i++)
		if(argp->userarray.userarray_val[i]>result.max)
			result.max=argp->userarray.userarray_val[i];

	for(i=0;i<argp->userarray_size;i++)
		if(argp->userarray.userarray_val[i]<result.min)
			result.min=argp->userarray.userarray_val[i];

	printf("...Returning value Max: %d, Min: %.d\n",result.max,result.min);
	printf("\n");
	return &result;
}

product_array *
product_1_svc(multiplication *argp, struct svc_req *rqstp)
{
	static product_array  result;
	printf("RPC-Client requested Multiplication Product Array...\n");

	result.prodarr.prodarr_len=argp->userarray_size;
	result.prodarr.prodarr_val=(float *)malloc(argp->userarray_size*sizeof(float));
	int i;

	for(i=0;i<argp->userarray_size;i++)
		{
	  	result.prodarr.prodarr_val[i]=argp->multiplier*argp->userarray.userarray_val[i];
	    //printf("Product[%d]: %.1f\n",i,result.prodarr.prodarr_val[i]); //print product array on rpc-server
		}

	printf("...Returning Product Array after multiplication\n");
	printf("\n");
	return &result;
}
