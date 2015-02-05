# Build instructions for Windows

## Prerequisites
In order to compile json-c and matlab-json, you will need the following tools:

* [Windows SDK 7.1][1] ([Mathworks instructions][3])
* [Visual Studio 2013][2] (recommended to use Community edition)

[1]: http://www.microsoft.com/en-us/download/details.aspx?id=8279
[2]: http://www.visualstudio.com/
[3]: http://uk.mathworks.com/matlabcentral/answers/101105-how-do-i-install-microsoft-windows-sdk-7-1

## Code
The `make` file for Windows assumes that the [json-c][4] source code is available
in a directory called `json-c` within the same parent directory as the directory
containing the [matlab-json][5] source code.

A typical layout would be:

* src/
	* json-c/
		* json.h
		* ...
	* matlab-json/
		* make.m
		* fromjson.c
		* ...

[4]: https://github.com/json-c/json-c
[5]: https://github.com/christianpanton/matlab-json

## Compile JSON-C
* Open `json-c.vcxproj` file in Visual Studio 2013
* Select "Debug" configuration
* Select appropriate platform for the target MEX file (win32 => mexw32, x64 => mexw64)
* In the project properties, set the following properties
	* "Configuration Properties\General\Platform Toolset" = "Windows7.1SDK"
* Check the following project properties:
	* "Configuration Properties\General\Output Directory" = "$(SolutionDir)$(Configuration)\" (win32 only)
	* "Configuration Properties\General\Output Directory" = "$(SolutionDir)$(Platform)\$(Configuration)\" (x64 only)
	* "Configuration Properties\General\Target Name" = "$(ProjectName)"
	* "Configuration Properties\General\Target Extension" = ".lib"
	* "Configuration Properties\General\Configuration Type" = "Static Library"
	* "Configuration Properties\C\C++\Advanced\CompileAs" = "Compile as C++ Code (/TP)"
	* "Configuration Properties\C\C++\Code Generation\Runtime Library" = "Multi-threaded DLL (/MD)"
	* "Configuration Properties\Librarian\General\Output File" = "$(OutDir)$(TargetName)$(TargetExt)"
* Build the project

The json-c library file `json-c.lib` should have been created in the location
that is expected by the matlab-json `make` function.

## Compile MATLAB-JSON
* Open MATLAB and change to the matlab-json directory
* Run `mex -setup`, if this hasn't been done before and you're using R2013b or earlier, and select "Microsoft Software Development Kit (SDK) 7.1"
* Run `make`
