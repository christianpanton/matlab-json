#ifdef JSON_C_DIR_PREFIXED
    #include <json-c/json.h>
    #include <json-c/json_object_private.h>
#else
    #include <json.h>
    #include <json_object_private.h>
#endif

#include <stdio.h>
#include <string.h>
#include "mex.h"



void object(json_object * jo, mxArray ** mxa); 
void parse(json_object * jo, mxArray ** mxa); 

void mexFunction (int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{

    char *buf;
    struct json_object *jo;

    if (nrhs != 1) { 
        mexErrMsgTxt("One input argument required.");

    } 
    if (nlhs > 1) {
        mexErrMsgTxt("Too many output arguments.");
    }

    if (!mxIsChar(prhs[0]) || (mxGetM(prhs[0]) != 1 ) )  {
        mexErrMsgTxt("Input argument must be a string.");
    }

    buf = mxArrayToString(prhs[0]);
    jo = json_tokener_parse(buf);

    if(jo == NULL)
        mexErrMsgTxt("error parsing json.");
    else
        parse(jo, &plhs[0]);

    /* free memory */
    json_object_put(jo);
    mxFree(buf);
    

}



void value(json_object *jo, mxArray ** mxa){

    enum json_type type = json_object_get_type(jo);
    mxArray *ma; 

    switch (type) {
        case json_type_boolean: 
            ma = mxCreateLogicalScalar(json_object_get_boolean(jo));
            break;
        case json_type_int: 
            ma = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
            *((int64_t *)mxGetData(ma)) = json_object_get_int64(jo);
            break;    
        case json_type_double: 
            ma = mxCreateDoubleScalar(json_object_get_double(jo));
            break;
        case json_type_string: 
            ma = mxCreateString(json_object_get_string(jo));
            break;
    }

    *mxa = ma;

}

void array( json_object *jo, char *key, mxArray ** mxa) {

    enum json_type type;
    int i;
    int len;

    struct json_object *jv;
    struct json_object *ja = jo;

    mxArray *ma;

    if(key){ 
        ja = json_object_object_get(jo, key);
    }

    len = json_object_array_length(ja); 

    *mxa = mxCreateCellMatrix(len, 1);

    for (i=0; i< len; i++){

        jv = json_object_array_get_idx(ja, i); 

        if(jv){
            type = json_object_get_type(jv);

            if (type == json_type_array) {
                array(jv, NULL, &ma);
            }

            else if (type != json_type_object) {
                value(jv, &ma);
            }

            else {
                object(jv, &ma);
            }
        }
        else{
            ma = mxCreateDoubleScalar(mxGetNaN());
        }

        mxSetCell(*mxa, i, ma);

    }


}

int keys_count(json_object * jo){

    int count = 0;
    
    json_object_object_foreach(jo, key, val)
        count++;

    return count;
}

void keys_fill(json_object * jo, char *** keys, int count){

    int i = 0;
    struct json_object_iter it;
    *keys = mxMalloc(count * sizeof(*keys));

    json_object_object_foreachC(jo, it){
        (*keys)[i] = mxMalloc(sizeof(char)*(strlen(it.key)+1));       
        strcpy((*keys)[i], it.key);
        i++;
    }
}

void object(json_object * jo, mxArray ** mxa) {

    enum json_type type;
    struct json_object_iter it;
    char ** keys;
    mxArray *ma;

    int count = keys_count(jo); 

    keys_fill(jo, &keys, count);

    *mxa = mxCreateStructMatrix(1, 1, count, (const char**) keys);

    json_object_object_foreachC(jo, it) {

        if(it.val){
            type = json_object_get_type(it.val);

            switch (type) {
                case json_type_boolean:
                case json_type_double:
                case json_type_int:
                case json_type_string:
                    value(it.val, &ma);
                    break;
                case json_type_object: 
                    object(json_object_object_get(jo, it.key), &ma);
                    break;
                case json_type_array: 
                    array(jo, it.key, &ma);
                    break;
            }
        }
        else{
            ma = mxCreateDoubleScalar(mxGetNaN());
        }

        mxSetField(*mxa, 0, it.key, ma);

    }
}


void parse(json_object * jo, mxArray ** ma) {

    enum json_type type;
    if(jo){    
    
        type = json_object_get_type(jo);

        switch (type) {
            case json_type_boolean:
            case json_type_double:
            case json_type_int:
            case json_type_string:
                value(jo, ma);
                break;
            case json_type_object: 
                object(jo, ma);
                break;
            case json_type_array: 
                array(jo, NULL, ma);
                break;
        }
    }else{
        *ma = mxCreateDoubleScalar(mxGetNaN());
    }
}


