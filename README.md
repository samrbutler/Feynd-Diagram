# FeyndDiagram
A C++ program to find all Feynman diagrams for a user-defined model and process.

**This program is very much still in development - features may change and no assurances are made for functionality.**
## Features
* Flexible model definition
* Fast algorithm for tree-level diagram generation
## Features in development
* Generation of loop diagrams
## Future functionality
* Evaluation of diagrams
* Automatic model generation from a Lagrangian
## Usage
Before compiling and running, define a valid particle model in *Model.h* and *Model.cpp*, providing particle names, an antiparticle dictionary, a list of allowable interactions of the theory, and a suitably-defined overloaded operator<<.

The class **Diagram**, defined in *Diagram.h* and *Diagram.cpp*, is provided for creation of an initial process, and the function **connect** is provided to return all possible tree-level completions of this diagram in a vector. 