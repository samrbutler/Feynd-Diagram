/*Loops.cpp : Code relevant to the implementation of loops
*    Function definitions
*/


#include "Loops.h"

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"





std::vector<LoopVertex> getLoopTopologies(const std::vector<Particle>& legs,const int num_loops)
{
	listofpairedgroupings subsets{ getSubsets(legs) };

	/*	TO-DO:
	*		> get all the groupings of external vertices
	*		> extend code to generate m->n dictionaries for all (m+n)<max_{vertices} (num_legs)
	*		> go through the dictionaries and get a new list of particles
	*		> can we figure out (at least an upper bound) how many loops what we have just done will introduce?
	*/


	return std::vector<LoopVertex>();
}
