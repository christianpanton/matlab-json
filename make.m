function make(varargin)

mexargs_common = [{'-g'} varargin];
mexargs_json = [mexargs_common {'-ljson-c'}];
matlab_json_base = fileparts(mfilename('fullpath'));

if ispc
    % Windows7/Visual Studio 2010
    
    % Assume json-c and matlab-json are checked out in the same directory
    %   (Not required if pre-built libraries are used)
    json_c_path = fullfile(fileparts(pwd), 'json-c');
    
    json_lib_path = fullfile(matlab_json_base, 'lib');
    if exist(fullfile(json_lib_path, 'json-c.lib'), 'file')
        % Assume json-c.lib is in lib directory
    else
        % Assume json-c has been built with default VS settings
        json_lib_path = json_c_path;
        if strcmp(computer('arch'), 'win64')
            json_lib_path = fullfile(json_lib_path, 'x64');
        end
        if any(strcmp(mexargs_json, '-g'))
            json_lib_path = fullfile(json_lib_path, 'Debug');
        else
            json_lib_path = fullfile(json_lib_path, 'Release');
        end
    end
    
    json_inc_path = fullfile(matlab_json_base, 'include', 'json-c');
    if exist(json_inc_path, 'dir')
        % Assume json-c headers are in include\json-c directory
    else
        json_inc_path = json_c_path;
    end
    
    mexargs_json = [{
        ['-I' json_inc_path], ['-L' json_lib_path], '-Dinline=__inline', ...
        ... % 'LINKFLAGS="$LINKFLAGS' '/NODEFAULTLIB:MSVSRT' '/NODEFAULTLIB:MSVCRTD' '/NODEFAULTLIB:LIBCMT' '/NODEFAULTLIB:LIBCMTD"', ...
        } mexargs_json];
    mex(mexargs_json{:}, 'fromjson.c')
    mex(mexargs_json{:}, 'tojson.c')
else
    % Linux/Ubuntu/GCC
    mexargs_json = ['-D JSON_C_DIR_PREFIXED ' mexargs_json];
    mex(mexargs_json{:}, 'fromjson.c')
    mex(mexargs_json{:}, '-lm', 'tojson.c')
end

mex(mexargs_common{:}, 'setjsonfield.c')
