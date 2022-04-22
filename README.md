# JSON Parser

This is a library written from scratch that provides a top-down ([recursive descent](https://en.wikipedia.org/wiki/Recursive_descent_parser)) parser for JSON files.
As a result of the parsing phase an AST [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) is built.
The user can navigate the AST directly jumping from one node to its children or
automatically through the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern).

The context-free grammar on which the AST is 
Every node of the AST build out of the parsing phase matches a production of the [CFG grammar](https://en.wikipedia.org/wiki/Context-free_grammar) defined in [grammar.ebnf](grammar.ebnf) 
and formatted in the [Extended Backus–Naur form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form).

**Work in progress!**
This implementation is not fully [RFC7159](https://www.rfc-editor.org/info/rfc7159)-compliant
yet.

To compile all examples (default compiler: `g++`):
```
make
```

To compile with a different compiler, e.g. `clang++`:
```
make CC=clang++
```

## GNU Checker with Bison+Flex

For the sake of performance comparison, the `gnu` directory contains the implementation
of a JSON parser (named `gnuchecker` in this repo) with [Bison](https://www.gnu.org/software/bison/) and [Flex](https://github.com/westes/flex).
The executable can be used as a fast checker since it parses the grammar productions
without performing any action.

It can be compiled with:
```
make gnuchecker
```

Bison and Flex can be installed with `apt install bison` and `apt install flex` respectively.

**Examples**

In the root directory there are some examples of how this library can be used.
As a general guideline, by implementing a new _visitor_ it is possibile to explore
the entire AST (Abstract Syntax Tree) generated.

```c++
json_parser::AST ast1("[1,3.2,\"tag\"]");
ast1.Build();
std::cout << ast1.children[0]->children[0]->ToString() << "\n"; // 1
std::cout << ast1.children[0]->children[1]->ToString() << "\n"; // 3.2
std::cout << ast1.children[0]->children[2]->ToString() << "\n"; // "tag"
```
```c++
json_parser::AST ast2("{\"key\":3}");
ast2.Build();
std::cout << ast2.children[0]->children[0]->children[0]->ToString() << "\n"; // "key"
std::cout << ast2.children[0]->children[0]->children[1]->ToString() << "\n"; // 3

```

Using the `FormatterVisitor` visitor already implemented in [`formatter.cpp`](formatter.cpp)):

```c++
// ast_node = instance of a derived class of json_parser::ASTNode
//            e.g. json_parser::Array
FormatterVisitor fv;
ast_node.Accept(fv);
std::cout << fv.GetResult();
```

## Formatter

The command-line tool [`formatter.cpp`](formatter.cpp) implements a visitor that produces a formatted version of any JSON file.

Example of `formatter` in action on the `data/short.json`:
```json
{"a":{"b":1.2,"c":3,"d":[4,5]},"e":[]}
```
```
$ ./formatter data/short.json
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


## Benchmarking

To run a parsing benchmark on the set of JSON files provided in `data/benchmark` use `make benchmark`.
The command uses `checker` as the default program during the benchmark,
to set another one set the `CMD` make variable appropriately.

Here are some examples:
```
make benchmark CMD=checker
make benchmark CMD=formatter
make benchmark CMD=gnuchecker CC=g++
make benchmark CMD=gnuchecker CC=clang++
```

The command will produce an output like the following (time in seconds):
```
data/benchmark/canada.json         .250
data/benchmark/citm_catalog.json   .097
data/benchmark/twitter.json        .048
```

To obtain benchmark results in CSV format directly use `./benchmark`.
For example:
```
./benchmark.sh formatter
```

## Testing

To run a conformance test:
```
make test CMD=checker
```
or just `make test`.
This will produce an output like 
```
Total: 39 Passed: 38 Failed: 1
```

For a more verbose output with the details of each single test use `./test.sh`.

## Bugs

Here is a list of known bugs regarding both the C++ checker and the Bison/Flex program.
- `data/fail/fail18.json` gets incorrectly accepted as there are no checks regarding the depth
of nested arrays.

## $cat author.json

```json
{
    "name": "Federico Sossai",
    "country": "Italy",
    "year": 2022,
    "email": "federico.sossai@gmail.com"
}
```