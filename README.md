# JSON Parser

This is a library written from scratch that provides a top-down ([recursive descent](https://en.wikipedia.org/wiki/Recursive_descent_parser)) parser for JSON files.

It comprises the C++ API and the following four tools:
- **`jcheck`** checks compliance building the AST.
- **`jmatch`** checks compliance without building the AST.
- **`jcolor`** formats with colors just like [`jq`](https://github.com/jqlang/jq).
- **`jpretty`** formats with 4-space indentation.
- **`jugly`** formats with no white spaces.
- **`gnu/jcheck`** acts like `jcheck` but it's made with Bison and Flex.

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
source enable
```
will install the header files and the shared library.

The Bison+Flex implementation can be compiled with `make gnu/check`.

## Build Upon It

After making sure that the library has been sourced (`source enable`)
you can easily link it to your own tool:

```
g++ YourTool.cpp $(pkg-config --cflags --libs jpaser)
```

## API Examples

For a complete use case check out [`tools/Pretty.cpp`](tools/Pretty.cpp).

You can implement customized visitors by extending the class `Visitor`.

```c++
json_parser::Object object;
if (object.From("{\"mixed\": [1,2.3,"four"]")) {
  PrettyVisitor pv;
  object.Accept(pv);
  cout << pv.GetResult();
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

Try implementing a your variant that does something more special!

## Benchmarking

To run a parsing benchmark on the set of JSON files provided in `data/benchmark` use `make bench`.
The command uses `build/tools/jcheck` as the default program during the benchmark,
to set another one set the `CMD` make variable appropriately.

Here are some examples:
```
make bench
make bench CMD=build/tools/jcheck
make bench CMD=jcheck           # after `source enable`
make bench CMD=gnu/jcheck       # after `make gnu/jcheck`
```
Try comparing it with the popular JSON processor
```
make bench CMD=jq
```

The command will produce an output like the following:
```
file                              size[MB]  time[s]  speed[MB/s]
data/benchmark/canada.json        2.146     .084     25.547
data/benchmark/citm_catalog.json  1.647     .046     35.804
data/benchmark/twitter.json       .602      .021     28.666
```

## Testing

To run a compliance test:
```
make test
make test CMD=build/tools/jcheck
make test CMD=gnu/jcheck            # after `make gnu/jcheck`
make test CMD=jq                    # the popular JSON processor
```

## What's Next

- [ ] Support for the [JSON Lines](https://jsonlines.org/) format

## $cat author.json

```json
{
    "name": "Federico Sossai",
    "country": "Italy",
    "year": 2022,
    "email": "federico.sossai@gmail.com"
}
```
