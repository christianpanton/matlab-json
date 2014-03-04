clear mex % avoid lock on file
if ispc
    % Windows7/Visual Studio 2010
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json-c\Release\ fromjson.c
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json-c\Release\ tojson.c 
else
    % Linux/Ubuntu/GCC/MacOS with Homebrew
    [~, flags] = system('pkg-config json-c --cflags');
    mex(flags, '-ljson-c', '-g', 'fromjson.c');
    mex(flags, '-ljson-c', '-lm', '-g', 'tojson.c');
end

mex setjsonfield.c
