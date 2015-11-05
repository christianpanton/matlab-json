# Build Instructions for Windows

There are two main options for compiling matlab-json on Windows:

1. Build json-c from source
2. Use pre-built json-c library

Instructions for both options follow.

## Build from Source

### Prerequisites
In order to compile json-c and matlab-json, you will need the following tools:

* [Windows SDK 7.1][1] ([Mathworks instructions][3])
* [Visual Studio 2013][2] (recommended to use Community edition)

[1]: http://www.microsoft.com/en-us/download/details.aspx?id=8279
[2]: http://www.visualstudio.com/
[3]: http://uk.mathworks.com/matlabcentral/answers/101105-how-do-i-install-microsoft-windows-sdk-7-1

### Code
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

### Compile JSON-C
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

### Compile MATLAB-JSON
* Open MATLAB and change to the matlab-json directory
* Run `mex -setup`, if this hasn't been done before and you're using R2013b or earlier, and select "Microsoft Software Development Kit (SDK) 7.1"
* Run `make`

## Use Pre-built Library
This approach relies on the [Appveyor][7] build scripts added to the json-c
project in [Pull Request #200][6] to build the json-c library in a Continuous
Integration (CI) environment, rather than installing the necessary tools and
compiling it locally.

As of 5-Nov-2015, this pull request has not yet been merged and there is no
official or semi-official CI project for json-c, but the CI build script in this
pull request can still be used.

[6]: https://github.com/json-c/json-c/pull/200
[7]: http://www.appveyor.com/

### Prerequisities
The only prerequisite tools for this option is the Windows SDK 7.1 (see above).

### Obtain pre-built library
Since there is no official or semi-official CI project for json-c, there are two
main options for obtaining the pre-built library for this approach:
1. Create your own fork of json-c that merges the required pull request and any
others that are required to make the Windows version build correctly, and create
your own Appveyor project.
2. Use someone else's version of option 1 such as [this one][8]. In this
version, as of 5-Nov-2015, the `ci_dev` branch is the most up-to-date.

From the Appveyor project:
* Navigate to the appropriate (passing) build
* Open the job for "Debug" configuration for "Windows7.1SDK"
* Open the "Artifacts" tab
* Download the artifact `json-c.lib.zip`

[8]: https://ci.appveyor.com/project/Nzbuu/json-c

### Extract pre-built library
The contents of the `json-c.lib.zip` file described above should be extracted
into the root directory of the matlab-json source. The resulting directory
structure will be as follows:

* matlab-json/
  * include/
    * json-c/
      * json.h
      * ...
  * lib/
    * json-c.lib
  * make.m
  * fromjson.c
  * ...

### Compile MATLAB-JSON
The steps for compiling matlab-json itself are the same as the other approach (see above).

The `make` script will detect the compiled library and header files in the
matlab-json directory and use these in preference to any that are in the json-c
source directory (if it exists).
