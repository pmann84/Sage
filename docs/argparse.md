# `sage::argparse`

## Overview
This is a Python style command line argument parser in C++. It is part of this header only library so you only need to include with `#include <sage/argparse/argparse.hpp>`

## Functionality
* Supports positional arguments that can consume a (currently fixed) number of items
* Supports optional arguments that can consume a (currently fixed) number of items
* Optional help argument always added to the parser in the form `-h, --help`

## Behaviour
* Arguments must be input on the command line in the order that they are added to the parser
* Optional arguments can be input in any order around positional arguments, but positional args must be in order relative to themselves
* Optional arguments specified more than once on input are overwritten

## Examples
### Simple Positional Argument Example
Note here we have specified the name and description for the parser, but these can be left blank. If blank the parser name will be filled out with the executable name on parsing command line input.
```c++
int main(int argc, char *argv[])
{
    auto parser = argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").help("Positional foo argument help.");
    parser.parse_args(argc, argv);
    auto foo = parser.get<std::string>("foo");
    return 0;
}
```
### Simple Optional Argument Example
```c++
int main(int argc, char *argv[])
{
    auto parser = argparse::argument_parser();
    parser.add_argument({"-f", "-foo"}).help("Optional foo argument help.");
    parser.parse_args(argc, argv);
    auto foo = parser.get<std::string>("foo");
    return 0;
}
```
### Simple Positional and Optional Argument Example
```c++
int main(int argc, char *argv[])
{
    auto parser = argparse::argument_parser();
    parser.add_argument("foo").help("Positional foo argument help.");
    parser.add_argument({"-b", "-bar"}).help("Optional bar argument help.");
    parser.parse_args(argc, argv);
    auto foo = parser.get<std::string>("foo");
    auto bar = parser.get<std::string>("bar");
    return 0;
}
```

### num_args for Positional and Optional Arguments Example
Note in the example below we can get all arguments in a vector by correctly specifying the template to the `get` function.
```c++
int main(int argc, char *argv[])
{
    auto parser = argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").help("foo argument help.");
    parser.add_argument("bar").num_args(3).help("bar argument help.");
    parser.add_argument({"-b", "--baz"}).help("baz argument help.");
    parser.add_argument({"-g", "--goo"}).num_args(3).help("baz argument help.");
    parser.parse_args(argc, argv);
    auto foo = parser.get<std::string>("foo");
    auto bar = parser.get<std::vector<std::string>>("bar");
    return 0;
}
```

### Example Help output
When passing the help flag on a program, it gives output like the following. This is for a named parser with description and one positional argument
```text
Usage MyParser [-h] foo

    Program description

Positional Arguments:
foo: foo argument help.

Optional Arguments:
-h, --help: Show this help message and exit.
```
For a more complicated set of arguments like the `num_args` example above it would look something like
```text
Usage MyParser [-h] [-b B] [-g G G G] foo bar bar bar

    Program description

Positional Arguments:
foo: foo argument help.
bar: bar argument help.

Optional Arguments:
-h, --help: Show this help message and exit.
-b, --baz: baz argument help.
-g, --goo: goo argument help.
```

## Future Work
Currently this is just an MVP feature set argument parser, but below is a non-exhaustive list of things that I would like to get to at some point, more things will probably get added as and when I use this library.
* Add support for inputting negative numbers as arguments
* Refactor the output of the help string to be spaced better (taking into account all argument lengths etc)
* Implement the ability to construct nested parsers for git style command line interfaces
* Implement num_args = *, ? and + for positional and optional arguments (in progress)
* Default argument values (for optional arguments and for positional args specified with num args *, ? or +)
* Add logging, off by default, but ability to see what args have been parsed as they are parsed
* Handle multiple arguments of the same names being added (positional and optional)
* The code works, but its not overly pretty, tidy it up
