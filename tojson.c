#ifdef JSON_C_DIR_PREFIXED
    #include <json-c/json.h>
#else
    #include <json.h>
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void parse(mxArray *ma, json_object **jo);

void mexFunction (int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{

    struct json_object *jo = NULL;
    char* out;

    if (nrhs != 1) {
        mexErrMsgTxt("One input argument required.");
    }

    if (nlhs > 1) {
        mexErrMsgTxt("Too many output arguments.");
    }


    parse((mxArray *) prhs[0], &jo);

    if(jo){
        out = (char*) json_object_to_json_string(jo);
    }
    else{
        out = "null";
    }

    plhs[0] = mxCreateString(out);

    if(jo) json_object_put(jo); /* free memory */
}

void numeric(double dbl, json_object **jo){

    int intgr = round(dbl);

    if(mxIsNaN(dbl) || mxIsInf(dbl)){
        *jo = NULL;
    }
    else{
        if(fabs(dbl-(double)intgr) > 1e-18){
            *jo = json_object_new_double(dbl);
        }
        else{
            *jo = json_object_new_int(intgr);
        }
    }
}


void object(mxArray *ma, int i, json_object **jo){

    int j = 0;
    int n = mxGetNumberOfFields(ma);
    mxArray *tmpma;
    json_object *tmpobj;

    *jo = json_object_new_object();


    for(; j < n; j++){
        bool needDestroy = false;
        tmpma = mxGetFieldByNumber(ma, i, j);
        if (tmpma == NULL){
            /* Object is NULL, create an empty matrix instead */
            tmpma = mxCreateNumericMatrix(0, 0, mxDOUBLE_CLASS, mxREAL);
            needDestroy = true;
        }
        parse(tmpma, &tmpobj);
        json_object_object_add(*jo, mxGetFieldNameByNumber(ma, j), tmpobj);
        if (needDestroy)
            mxDestroyArray(tmpma);
    }
}

void parse(mxArray *ma, json_object **jo){

    int i = 0;
    size_t num_el = mxGetNumberOfElements(ma);
    size_t m = mxGetM(ma);
    size_t n = mxGetN(ma);

    json_object *lobj;
    json_object *tmpobj;
    mxArray *tmpma;

    /* was char - convert to string*/
    if(mxIsChar(ma)){
        char *str = mxArrayToString(ma);
        *jo = json_object_new_string(str);
    }
    /* was struct - convert to object */
    else if(mxIsStruct(ma)){
        if(num_el == 1){
            object(ma, 0, jo);
        }
        else{
            /* it was a list of objects! */
            *jo = json_object_new_array();

            if(n == 1){
                n = m;
                m = 1;
            }

            for(i = 0; i < num_el; i++){

                if(i % m == 0 && m > 1){
                    lobj = json_object_new_array();
                    json_object_array_add(*jo,lobj);
                }

                object(ma, i, &tmpobj);

                if(m > 1)
                    json_object_array_add(lobj, tmpobj);
                else
                    json_object_array_add(*jo, tmpobj);
            }
        }


    }
    /* was cell array - convert to list */
    else if(mxIsCell(ma)){

        *jo = json_object_new_array();

        if(n == 1){
            n = m;
            m = 1;
        }

        for(i = 0; i < num_el; i++){

            if(i % m == 0 && m > 1){
                lobj = json_object_new_array();
                json_object_array_add(*jo,lobj);
            }

            tmpma = mxGetCell(ma, i);

            if(tmpma){
                parse(tmpma, &tmpobj);

                if(m > 1)
                    json_object_array_add(lobj, tmpobj);
                else
                    json_object_array_add(*jo, tmpobj);
            }
        }
    }
    /* was numeric or bool - convert to number or list */
    else{

        /* single number or bool */
        if(num_el == 1){
            /* mxINT32_CLASS */

            if(mxIsComplex(ma)){
                mexErrMsgTxt("Data cannot be complex.");
            }

            if(mxIsNumeric(ma)){
                double dbl = mxGetScalar(ma);
                numeric(dbl, jo);
            }
            else if(mxIsLogical(ma)){
                *jo = json_object_new_boolean(mxIsLogicalScalarTrue(ma));
            }
            else{
                printf("Type is: %s\n", mxGetClassName(ma));
                mexErrMsgTxt("Encountered an unknown type.");
            }

        }
        else{

            double *dbl;
            int *intgr;
            mxLogical *bl;

            if(mxIsDouble(ma)){
                dbl = mxGetData(ma);
            }
            else if(mxIsClass(ma, "mxINT32_CLASS")){
                intgr = mxGetData(ma);
            }
            else if(mxIsLogical(ma)){
                bl = mxGetData(ma);
            }
            else{
                printf("Type is: %s\n", mxGetClassName(ma));
                mexErrMsgTxt("Encountered an unsupported type.");
            }

            *jo = json_object_new_array();

            /* vector - convert to list */
            if(n == 1){
                n = m;
                m = 1;
            }

            for(i = 0; i < num_el; i++){

                if(i % m == 0 && m > 1){
                    lobj = json_object_new_array();
                    json_object_array_add(*jo,lobj);
                }

                if(mxIsLogical(ma))
                    tmpobj = json_object_new_boolean(bl[i]);
                else if(mxIsDouble(ma))
                    numeric(dbl[i], &tmpobj);
                else
                    numeric((double)intgr[i], &tmpobj);

                if(m > 1)
                    json_object_array_add(lobj, tmpobj);
                else
                    json_object_array_add(*jo, tmpobj);

            }


        }

    }


}
