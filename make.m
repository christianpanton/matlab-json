if ispc
    % Windows7/Visual Studio 2010
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json-c\Release\ fromjson.c
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json-c\Release\ tojson.c 
else
    % Linux/Ubuntu/GCC
    mex -ljson-c -g fromjson.c
    mex -ljson-c -lm -g tojson.c
end

mex setjsonfield.c