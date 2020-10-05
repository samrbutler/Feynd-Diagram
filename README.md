# FeyndDiagram
A C++ program to find all Feynman diagrams for a user-defined model and process.
## Features
* Flexible model definition
* Fast algorithm for tree-level diagram generation
## Future functionality
* Generation of loop diagrams
* Evaluation of diagrams
* Automatic model generation from a Lagrangian
## Usage
The user should define a valid particle model in *Model.h*, providing particle names, an antiparticle dictionary and a list of allowable interactions of the theory. A simple Yukawa-like theory is provided by default.

The class **Diagram**, defined in *Diagram.h* and *Diagram.cpp*, is provided for creation of an initial process, and the function **connect** is provided to display all possible completions of this diagram on the terminal. 