# Laser++ 

**Laser++** is a C++ implementation of the [Laser stream reasoning engine](https://research.vu.nl/en/publications/expressive-stream-reasoning-with-laser) 
based on the [LARS logic framework](http://www.kr.tuwien.ac.at/research/projects/dhsr/pub/2015/bdef2015-aaai.pdf)   


### Prerequisites
Laser++ requires:
* C++ compiler compliant with C++17 standard (tested on GCC 7.3.0 and 
Clang 6.0.0 were)
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

Run test application by executing:
```
./laser.sh r 
```

See all options by running:
```
./laser.sh h 
```

### Benchmarking 

Benchmarking requires an input file from which the stream will be read. 
This file can be generated by using `gen.py` script. Edit this script as you see
fit before running it.  

Benchmark by running:
```
./laser.sh bench test_name end_time_of_stream number_of_facts_per_timepoint window_size stream_file_path [output_file_path] 
```

The parameter test_name can be selected from this list:
[*atom*, *diamond*, *box*, *conjunction*, *tref*, *multiatom*, *multiall*, 
*atom_window*]. 
For details on each test, see *benchapp/main.cpp*

The parameter *output_file_path* is optional. In case it is missing, no output
will be written. 

Example use:
```
./laser.sh bench atom 1000000 100 0 ~/stream_file.txt
```

### Project organization
The code of Laser++ can be found in directories **include** (header files) and **src** 
(C++ source files). The project is divided in 5 modules (*formula*, *io*, *program*, 
*rule*, *util*). Each module can be found in a dedicated sub-directory of *include*
and *src* respectively.

The directory **test** contains a test application that uses the Laser++ reasoning 
engine to implement a simple reasoner. It also contains some test cases that
use this application to test the reasoning engine. The testcases can be found 
in *test/testcase/*

The directory **benchapp** contains a different Laser-based application. This 
application is used for benchmarking the performance of the Laser++ reasoning
engine. It can also be used for profiling.  


### Using Laser++

Laser++ is a reasoning engine that can be used to implement systems capable of
real-time semantic inference on stream of data. 

In order to implement a reasoning application, the user need only to implement the 
`laser::io::IOManager` interface found in *include/io/io_manager.h*. This 
implementation should reflect the application specific needs regarding reading
and parsing of the input stream, as well as formatting and writing of the output
stream. An example of such an application can be found in the folder *test* and
*benchapp*

By default Laser++ rules follow a simple language, but an application specific
language can be defined by implementing the `laser::rule::RuleReader` interface
found in *include/io/rule_reader.h*. 
An example of such an implementation can be found in *src/rule/default_rule_reader.cpp*
and its respective header file: *include/rule/default_rule_reader.h*. 

Once the *IOManager* interface is implemented, a application can use the Laser++
reasoning engine by constructing an instance of the `laser::program::Program` class
found in *include/program/program.h* and *src/program/program.cpp* respectively.
Examples can be found in *test/main.cpp* and *benchapp/main.cpp*


