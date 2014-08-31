% from json.org demonstration
test_string = '{"glossary": {"title": "example glossary", "GlossDiv": {"title": "S", "GlossList": { "GlossEntry": { "ID": "SGML", "SortAs": "SGML", "GlossTerm": "Standard Generalized Markup Language", "Acronym": "SGML", "Abbrev": "ISO 8879:1986", "GlossDef": { "para": "A meta-markup language, used to create markup languages such as DocBook.", "GlossSeeAlso": ["GML", "XML"] }, "GlossSee": "markup" } } } }}';
matlab_object = fromjson(test_string);

% testing strings, lists and objects
assert(strcmp(matlab_object.glossary.title,'example glossary'));
assert(strcmp(matlab_object.glossary.GlossDiv.GlossList.GlossEntry.GlossDef.GlossSeeAlso{2},'XML'));

% testing numbers
test_string = '{"float": 1234567890000.123, "integer": 1234567890000}';
matlab_object = fromjson(test_string);

% testing strings, lists and objects
assert(matlab_object.float ==  1234567890000.123);
assert(matlab_object.integer == 1234567890000);

% testing special floats, must be converted to null according to spec
matlab_object = [NaN, Inf, -Inf];
test_string = tojson(matlab_object);
assert(strcmp(test_string, '[ null, null, null ]'));

display('All tests OK');
clear test_string;
clear matlab_object;