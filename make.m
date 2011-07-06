mex -ljson -g fromjson.c
mex -ljson -lm -g tojson.c
mex -lcurl -g jsonrpcpost.c
mex -g setjsonfield.c
