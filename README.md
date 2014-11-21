# pmcSAT
======

Parallel SAT Solver, awarded with a bronze medal in the 2013 and 2014 [SAT Competitions](http://www.satcompetition.org/) and published on the [FLAIRS-26 Conference](http://www.flairs.com/).

## Implementation

* Sequential mechanism based on [MiniSat](http://minisat.se/) and [Glucose](http://www.labri.fr/perso/lsimon/glucose/).
* Multiple instances of the same solver are launched with different parameter configurations, which cooperate to a certain degree by sharing relevant information when searching for a solution. 
* A more detailed description can be found [here](http://www.aaai.org/ocs/index.php/FLAIRS/FLAIRS13/paper/viewFile/5875/6048).

## Usage

For compiling:

```
./build.sh 
```

For running:
```
./binary/pmcsat_static INPUT_FILE.cnf
```
