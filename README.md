# Laser++ 

**Laser++** is a C++ implementation of the [Laser](https://research.vu.nl/en/publications/expressive-stream-reasoning-with-laser) a stream reasoning engine 
based on the [LARS logic framework](http://www.kr.tuwien.ac.at/research/projects/dhsr/pub/2015/bdef2015-aaai.pdf)   

### Prerequisites
Laser++ requires:
* C++ compiler compliant with C++17 standard (tested on GCC 7.3.0 and 
Clang 6.0.0)
* GTest suite (tested on version 1.8.0)
* CMake (minimum version: 3.7, tested on 3.10)

Optional tools:
* gprof (for profiling)
* gdb (for debugging)
* python (for generating benchmark input)

## Getting Started

Interact with Laser++ by using the `laser.sh` script. 

Build Laser++ by executing:
```
./laser.sh b
```

Run all test cases by executing:
```
./laser.sh t 
```

See all options by running:
```
./laser.sh h 
```

### Quick start with Sample reasoning application

The Laser++ source-code includes **Simpleapp**, a Laser++-based application that 
is meant as a demo. It can be used to test the Laser++ reasoning
engine on programs and input streams provided in text files. To run this 
application first compile Laser++ as well as simpleapp by running `./laser.sh b`,
then execute:
```
cmake-build-release/simpleapp CHASE_ALGORITHM PROGRAM_PATH INPUT_PATH OUTPUT_PATH
```
The three arguments represent the paths to the respective input and output files.
The Chase Algorithm parameter can be either *S* for Skolem Chase, or *R* for 
Restricted Chase.
The source code of *Simpleapp* can be found in *app-simple/main.cpp*, as well as
the sub directories *include* and *src* of the *app-simple* directory. Some 
sample programs and inputs can be found in *app-simple/samples/*.

Example execution:
```
cmake-build-release/simpleapp R app_simple/samples/bicycle.laser app_simple/samples/bicycle.stream output.txt
```

**Program**

Programs are composed of one or more rules, separated by new-line character '\n'.

Each rule is written as: `head := body`. Both *head* and *body* are formulas. 

The simplest type of formula is an Atom, which is composed of a predicate and
some variables, e.g.: `a(X, Y, X)`. In this atom, *a* is the predicate, while 
*X*, *Y*, and *Z* are variables.

More complex formulas can be represented by adding a unary operator in front of another
formula, or a binary operator between two formulas. The result is an extended
formula. These operators are written as: 
```
Diamond: [D]
Box: [B]
Time Window: [$, size]
Tuple Window: [#, size]
Time reference: [@, time_variable]
Conjunction: &&
```
Please note that the head of a Laser++ rule is limited to only atomic and time 
reference formulas.
Example program:
```
u(X) := [#, 1][D]f(X)
f(Y) := [$, 5] [B] e(Y, Y)
a(T, Z) := [@, T] b(Z)
[@, T]c(S) := [@, T] d(S)
i(V, W) := g(W) && h(V)
```

**Input Stream**

The file containing the input stream needs to start with a line containing two 
numbers: the first and last time-point in the stream.
Each line after has the following format: `time-point : facts`. Facts are grounded
atoms separated by commas. Time-points should be in order.
Example input stream:
```1 14 "
1 : q(1), p(1), e(a4, b6)
2 : p(2), a(3, 1, 8)
3 : 
4 : p(41), q(1) 
4 : p(8)
5 : a(4, 8, 12)
12 : a(56, 99, 0), p(1)
```

### Project organization
The code of Laser++ can be found in directories **include** (header files) and **src** 
(C++ source files). The project is divided in 4 modules (*core*, *rule*, *forumula*, 
*util*). Each module can be found in a dedicated sub-directory of *include*
and *src* respectively. The sub-directory *example* contains implementations of 
*rule::RuleReader* and *core::IOManager* interfaces. 

The directory **test** contains contains some test cases which can be found in 
*test/testcase/*. The directory also contains and application that runs some 
hardcoded programs on hard-coded input. The source-code of this application can 
be found in *test/main.cpp*.

The directory **app-bench** contains a Laser++-based application. This 
application is used for benchmarking the performance of the Laser++ reasoning
engine. It can also be used for profiling.  

### Laser++ modules:

**Core** - handles input-output as well as execution of the Laser++ program. 
References modules: Rule, Util.

**Rule** - parses laser programs and evaluates the rules on the input facts
provided by the stream. References modules: Formula, Util.

**Formula** - Evaluates the different formulas found in the program and stores
the result. References module Util.

**Util** - Contains classes useful in other modules such as exceptions, timeline.
Instances of the Grounding class present in this module are used as the basic 
block of data throughout the Laser++ reasoning engine. It is used to represent
such data as input facts, output conclusions or store annotated groudings of 
formulas present in the program being evaluated. 

### Implementing reasoner applications using the  Laser++ engine

Laser++ is a reasoning engine that can be used to implement systems capable of
real-time semantic inference on stream of data. 

In order to implement a reasoning application, the user need only to implement the 
`laser::core::IOManager` and `laser::rule::RuleReader` interfaces found in 
*include/core/io_manager.h* and *include/rule/rule_reader.h*. This 
implementation should reflect the application specific needs regarding reading
and parsing of the input stream, as well as formatting and writing of the output
stream. An example of such an implementation can be found in the folder 
*include/example* and *src/example*.  

Once the *IOManager* and *RuleReader* interfaces are implemented, 
an application can use the Laser++
reasoning engine by constructing an instance of the `laser::core::Reasoner` class
found in *include/core/reasoner.h* and *src/core/reasoner.cpp* respectively.
An example application can be found in *app-sample/main.cpp*

### Debuging

Laser++ comes with a testing application. 
The source code for this application can be found in the directory 
*test/* and can be executed by running:
```
./laser.sh r 
```

The test application runs a series of hardcoded Laser programs on hardcoded input.
This is useful for debuging, as a specific Laser program can be run in GDB. 
Simply edit the code in *test/main.cpp*, build and execute:
```
./laser.sh d 
```

### Benchmarking 

Benchmarking requires an input file from which the stream will be read. 
This file can be generated by using `gen.py` script:
```
python gen.py
```

Benchmark by running:
```
./laser.sh bench test_id end_time_of_stream number_of_facts_per_timepoint window_size chase_algorithm=(S/R) stream_file_path output_file_path 
```

The parameter test_id is used to select the Laser program to benchmark. 
At the moment these programs are hardcoded.
For details on each test, see *app-bench/main.cpp*
Also see *bench.sh* script for examples.

The Chase Algorithm parameter can be either *S* for Skolem Chase, or *R* for 
Restricted Chase.
Input streams can be generated using *gen.py* script.
The parameter *output_file_path* is optional. 
In case it is missing, no output will be written. 

Example use:
```
./laser.sh bench p1 1000 1000 0 O ~/stream_100M_1_2.txt  
```
