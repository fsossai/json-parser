# JSON Parser

This is a library written from scratch that provides a top-down ([recursive descent](https://en.wikipedia.org/wiki/Recursive_descent_parser)) parser for JSON files.
As a result of the parsing phase an AST [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) is built.
The user can navigate the AST directly jumping from one node to its children or
automatically through the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern).

**Work in progress!**
This implementation is not fully [RFC7159](https://www.rfc-editor.org/info/rfc7159)-compliant.

Here is a list of known bugs:
- Line breaks in keys are allowed
- Tabs in keys are allowed


**Examples**

```c++
json_parser::AST ast1("[1,3.2,\"tag\"]");
ast1.Build();
std::cout << ast1.children[0]->children[0]->ToString() << "\n"; // 1
std::cout << ast1.children[0]->children[1]->ToString() << "\n"; // 3.2
std::cout << ast1.children[0]->children[2]->ToString() << "\n"; // "tag"

json_parser::AST ast2("{\"key\":3}");
ast2.Build();
std::cout << ast2.children[0]->children[0]->children[0]->ToString() << "\n"; // "key"
std::cout << ast2.children[0]->children[0]->children[1]->ToString() << "\n"; // 3

```

Using a visitor (`FormatterVisitor` is already implemented in [`formatter.cpp`](formatter.cpp)):

```c++
// ast_node = instance of a derived class of json_parser::ASTNode
//            e.g. json_parser::Array
FormatterVisitor fv;
ast_node.Accept(fv);
std::cout << fv.GetResult();
```

In the root directory there are some examples of how this library can be used.
As a general guideline, by implementing a new _visitor_ it is possibile to explore
the entire AST (Abstract Syntax Tree) generated.

To compile all examples (default compiler: `g++`):
```
make
```

To compile with a different compiler, e.g. `clang++`:
```
make -e CC=clang++
```

## Formatter

The command-line tool [`formatter.cpp`](formatter.cpp) implements a visitor that produces a formatted version of any JSON file.

Example of `formatter` in action:
```
$ cat short.json
{"a":{"b":1.2,"c":3,"d":[4,5]},"e":[]}
$ ./formatter short.json
{
    "a": {
        "b": 1.2,
        "c": 3,
        "d": [
            4,
            5
        ]
    },
    "e": []
}
```
