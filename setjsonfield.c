#include <stdio.h>
#include <string.h>
#include "mex.h"


void mexFunction (int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{  
    void *ptr;
    void *newptr;
    int size;
	int strlen;
    char* name;
	mxArray* value;

    if (nrhs != 3) { 
        mexErrMsgTxt("Three input arguments required (struct, name, value).");
    } 

    if(!mxIsStruct(prhs[0])){
        mexErrMsgTxt("First argument must me a struct");
    }

    
    
    value = mxDuplicateArray(prhs[2]);

    strlen = mxGetN(prhs[1]) * mxGetM(prhs[1]);
    name = mxMalloc(sizeof(char*) * (strlen + 1));
    mxGetString(prhs[1], name, strlen+1);

    if(mxGetField(prhs[0], 0, name) == NULL){
        mxAddField(prhs[0], name);
    }

    mxSetField(prhs[0], 0, name, value);

}




