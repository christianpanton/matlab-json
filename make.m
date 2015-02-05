function make(varargin)

mexargs = [{'-ljson-c', '-g'} varargin];

if ispc
    % Windows7/Visual Studio 2010
    
    % Assume json-c and matlab-json are checked out in the same directory
    json_c_path = fullfile(fileparts(pwd), 'json-c');
    
    mexargs = [{
        ['-I' json_c_path], ['-L' fullfile(json_c_path, 'Release')], ...
        'LINKFLAGS="$LINKFLAGS' '/NODEFAULTLIB:MSVSRT' '/NODEFAULTLIB:MSVCRTD' '/NODEFAULTLIB:LIBCMT"', ...
        } mexargs];
    mex(mexargs{:}, 'fromjson.c')
    mex(mexargs{:}, 'tojson.c')
else
    % Linux/Ubuntu/GCC
    mex(mexargs{:}, 'fromjson.c')
    mex(mexargs{:}, '-lm', 'tojson.c')
end

mex setjsonfield.c