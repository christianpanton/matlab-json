if ispc
    % Windows7/Visual Studio 2010
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json\Release\ fromjson.c
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json\Release\ tojson.c 
else
    % Linux/Ubuntu/GCC
    mex -ljson -g fromjson.c
    mex -ljson -lm -g tojson.c
end

mex setjsonfield.c