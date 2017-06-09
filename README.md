`matlab-json` is a fast and efficient JSON-parser for MATLAB

Examples:

```
>> a = struct(); a.name = 'Hello, World!'; a.data = magic(3);
>> tojson(a);
ans =
{ "name": "Hello, World!", "data": [ [ 8, 3, 4 ], [ 1, 5, 9 ], [ 6, 7, 2 ] ] }

>> fromjson('{"name": "Hello, World", "data": 123}')
ans = 
    name: 'Hello, World'
    data: 123
```

Requirements
------------ 

[libjson (aka. JSON-c)](https://github.com/json-c/json-c) (0.11 minimum)

To enable maximum precision of floating point values, consider using the Git version or patching "json_object.c" in libjson, 
by changing the formatting string in the function "json_object_double_to_json_string" from ["%f" to %.17g"](https://github.com/json-c/json-c/commit/06450206c4f3de4af8d81bb6d93e9db1d5fedec1).

Install
-----------
Run "make" within MATLAB to build the MEX functions.

### Mac install
Install libjson (aka. JSON-c) to /usr/local.  It is easiest to do this with homebrew: ```brew install json-c```
Then run "make" within MATLAB.


### License
GPL-3.0
