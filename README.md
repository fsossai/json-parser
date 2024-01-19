# JSON Parser

This is a library written from scratch that provides a top-down ([recursive descent](https://en.wikipedia.org/wiki/Recursive_descent_parser)) parser for JSON files.

It comprises the C++ API and the following four tools:
- **`checker`** checks the correctness of a JSON input.
- **`prettify`** formats a JSON input with 4-spaces indentation.
- **`gnu/checker`** acts like `checker` but it's made with Bison and Flex

When using the API, an AST [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) is built as a result of the parsing phase.
The user can navigate the AST directly jumping from one node to its children or
automatically through the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern).

Every node of the AST build out of the parsing phase matches a production of the [CFG grammar](https://en.wikipedia.org/wiki/Context-free_grammar) defined in [grammar.ebnf](grammar.ebnf) 
and formatted in the [Extended Backus–Naur form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form).


## GNU Checker with Bison+Flex

For the sake of performance comparison, the `gnu` directory contains a parser implemented
with [Bison](https://www.gnu.org/software/bison/) and [Flex](https://github.com/westes/flex).
This is a very fast bottom-up parser that we can refer to as a performance baseline.

Bison and Flex can be installed with `apt install bison` and `apt install flex` respectively.

## Install

```
make install
```
will install the header files and the shared library.

The Bison+Flex implementation can be compiled with `make gnu/checker`.

## Build Upon It

After you `make install`, just `source enable` to facilitate the compilation of your own tool:

```
g++ $JSON_PARSER_LIBS YourTool.cpp
```

where `$JSON_PARSER_LIBS` contains all the ugly absolute paths that you will need.


## API Examples

For a complete use case check out [`examples/Prettify.cpp`](examples/Prettify.cpp).

You can implement customized visitors by extending the class `Visitor`.

```c++
json_parser::Object object;
if (object.From("{\"mixed\": [1,2.3,"four"]")) {
  PrettifyVisitor pv;
  object.Accept(pv);
  cout << object.GetResult();
}
```
will print
```json
{
    "mixed": [
        1,
        2.3,
        "four"
    ]
}
```

Try implementing a colorful variant of PrettyVisitor!

## Benchmarking

To run a parsing benchmark on the set of JSON files provided in `data/benchmark` use `make benchmark`.
The command uses `build/examples/checker` as the default program during the benchmark,
to set another one set the `CMD` make variable appropriately.

Here are some examples:
```
make benchmark
make benchmark CMD=build/examples/checker
make benchmark CMD=gnu/checker # after `make gnu/checker`
make benchmark CMD=jq # the popular JSON processor
```

The command will produce an output like the following (time in seconds):
```
file                              size[MB]  time[s]  speed[MB/s]
data/benchmark/canada.json        2.146     .260     8.253
data/benchmark/citm_catalog.json  1.647     .102     16.147
data/benchmark/twitter.json       .602      .054     11.148
```

## Testing

To run a correctness test:
```
make test
make test CMD=build/examples/checker
make test CMD=gnu/checker # after `make gnu/checker`
make test CMD=jq # the popular JSON processor
```

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
