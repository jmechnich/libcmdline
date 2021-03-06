libcmdline: a command line parsing library
==========================================

The libcmdline is a command line parsing library written in c++.
It supports single character short options (e.g. -v, -n, ...), long gnu-style
options (e.g. --verbose, ...) and positional parameters (e.g. list of 
filenames given to a command). 
libcmdline automatically generates a help message, that can be displayed
via the "--help" option.
It is also possible to display a "HTMLified" help message with "--usageHTML"
and an "XML" help message (used for frontend programs) with "--usageXML".

### How to use it?

1. Create some command line arguments (CmdArgs).

CmdArgs represent all the options that can be given to a program on the 
command line. There are several types of command line options:

CmdArgSwitch: an option without a parameter
CmdArgType<TYPE>: an option with a parameter of type TYPE
CmdArgVector<TYPE>: an option that takes a list of parameters of type TYPE
CmdArgThrow<E>: an option that throws an exception E when specified on the
	command line (used for --help).


Examples:

```c++
CmdArgType<int> count('c', "count", "HOW_MANY_FOOS", "select number of foos.");
// <int> -> parameter type given as a c++ template parameter
// 'c' -> short option name
// "count" -> long option name
// "HOW_MANY_FOOS" -> parameter name
// "select ..." -> option description
count.setDefaultValue(42);
```

This creates an optional command line option with a required integer 
parameter (named HOW_MANY_FOOS). On the command line it can be specified in 
the standard fashions:
"-c17", "-c 17", "--count 17" or "--count=17".

If the integer parameter is not given on the command line an error message is
displayed.

If the count option is not specified on the command line, its value is set to
the supplied default value.

The parsed value can be retrieved by "count.value()". 

```c++
CmdArgSwitch verbose('v', "verbose", "display verbose messages");
```

This creates an option without a parameter.

If it is specified on the command line by "-v" or "--verbose", its value
("verbose.value()") is true, otherwise it is false.

A positional parameter can be created by omitting the short and long option
names:

```c++
CmdArgType<std::string> outfile("FILENAME", "specify the output file");
```


By default options are optional. You can change this by setting the 
"CmdArg::isREQ" flag of an option as the last parameter of its constructor:

```c++
CmdArgType<std::string> infile("FILENAME", "specify the input file", CmdArg::isREQ);
```

If a required option is not given on the command line, and error message is 
displayed.

2. Create an argument iterator:
An argument iterator is used to retrieve the strings given to the parser.
If you want to parse strings given on the command line, you should use an 
"ArgvIter" and give it the c-style array containing the command line 
parameters:

```c++
int main(int argc, char** argv)
{
  ...
  ArgvIter iter(--argc, ++argv);
  ...
}
```

It is also possible to get the strings from a file or (more generaly) from an
input stream. You can do this via an "IStreamIter".


3. Create a command line object.

The CmdLine class is responsible for all the parsing work.

```c++
//          /program name\  /  short description  \
CmdLine cmd("testprogram", "this is a test program");
// add long description
cmd.description("This is the powerful test program. You can use it to ...");
```


4. Add the command line option objects the the CmdLine object.

Options are added to the CmdLine object by its "append" method. Calls to
CmdLine::append should be placed in a try/catch block since append throws 
CmdLineErrors if you append an option that uses already used short/long option
names.

```c++
try
{
  cmd.append(&count);
  cmd.append(&verbose);
  cmd.append(&infile);
  cmd.append(&outfile);
}
catch (const CmdLineError& e)
{
  std::cerr << "Error: " << e.str() << std::endl;
  exit(1);
}
```


5. Start the parsing.

```c++
try
{
  cmd.parse(iter);
}
catch (const CmdLineUsageError& e) // --help specified
{
  // display usage message
  cmd.usage()
  exit(0);
}
catch (const CmdLineSyntaxError& e)  // invalid command line
{
  cmd.error() << "invalid command line arguments:" << std::endl
              << e.str() << std::endl;
  exit(-1);
}
catch (const CmdLineUsageHTMLError& e) // --usageHTML specified
{
  cmd.usageHTML(std::cerr);
  exit(0);
}
catch (const CmdLineUsageXMLError& e) // --usageXML specified
{
  cmd.usageXML(std::cerr);
  exit(0);
}
```


6. Process the parsed values.

The parsed value of an option can be retried via the "value()" method.
You can check if an optional option was given with the "CmdArg::given()" 
method.

```c++
std::cout << "count=" << count.value << std::endl;
if (verbose.value()) std::cerr << "verbose mode" << std::endl;
std::cerr << "infile=" << infile.value() << std::endl;
if (outfile.given()) std::cerr << "outfile=" << outfile.value() << std::endl;
```

The whole example code:

```c++
#include <iostream>

#include "ArgvIter.hh"
#include "CmdArgs.hh"
#include "CmdLine.hh"
#include "Compiler.hh"
#include "Errors.hh"

int main(int argc, char** argv)
{
  // options
  CmdArgType<int> count('c', "count", "HOW_MANY_FOOS", "number of foos.");
  count.setDefaultValue(42);	
	
  CmdArgSwitch verbose('v', "verbose", "display verbose messages");

  CmdArgType<std::string> infile("FILENAME", "specify the input file", 
                                 CmdArg::isREQ);

  CmdArgType<std::string> outfile("FILENAME", "specify the output file");

  // argument iterator
  ArgvIter iter(--argc, ++argv);

  // CmdLine object
  CmdLine cmd("testprogram", "this is a test program");
  cmd.description("This is the powerful test program. You can use it to ...");

  // add options
  try
  {	
    cmd.append(&count);
    cmd.append(&verbose);
    cmd.append(&infile);
    cmd.append(&outfile);
  }
  catch (const CmdLineError& e)
  {
    std::cerr << "Error: " << e.str() << std::endl;
    exit(1);
  }	

  // parse and handle exceptions
  try
  {
    cmd.parse(iter);
  }
  catch (const CmdLineUsageError& e) // --help specified
  {
    // display usage message
    cmd.usage()
    exit(0);
  }
  catch (const CmdLineSyntaxError& e)  // invalid command line
  {
    cmd.error() << "invalid command line arguments:" << std::endl
                << e.str() << std::endl;
    exit(-1);
  }
  catch (const CmdLineUsageHTMLError& e) // --usageHTML specified
  {
    cmd.usageHTML(std::cerr);
    exit(0);
  }
  catch (const CmdLineUsageXMLError& e) // --usageXML specified
  {
    cmd.usageXML(std::cerr);
    exit(0);
  }

  // process parsed values
  std::cout << "count=" << count.value << std::endl;
  if (verbose.value()) std::cerr << "verbose mode" << std::endl;
  std::cerr << "infile=" << infile.value() << std::endl;
  if (outfile.given()) std::cerr << "outfile=" << outfile.value() << std::endl;

  return 0;
}
```
