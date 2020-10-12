/*Loops.cpp : Code relevant to the implementation of loops
*    Function definitions
*/


#include "Loops.h"

#include "Diagram.h"
#include "Model.h"
#include "Interactions.h"





std::vector<Diagram> getLoopTopologies(const std::vector<Particle>& legs, const int num_loops) {

	//Get all subsets of the legs of minimum size 1
	listofpairedgroupings subsets{ getSubsets(legs,1) };

	//Go through every returned subset
	for (pairedgrouping& grp : subsets) {

		std::vector<std::multiset<P>> listofproducts;

		//Loop over all interactions in the loop dictionary
		for (const std::pair<std::multiset<P>, std::multiset<P>>& interaction : Model::LOOPDICT) {
			//If the current group matches this interaction, the group is valid, so we add the products to our list
			if (vec2multiset(grp.first[0]) == interaction.first) {
				listofproducts.push_back(interaction.first);
			}
		}
		//If we didn't get anything back, go to the next grouping
		if (listofproducts.size() == 0) continue;
	
		//We now need to check how many loops we can from directly at this vertex
		// >> Any more complicated topologies will be captured by the rest of the algorithm
		for (std::multiset<P>& products : listofproducts) {

		}
		/* TODO
		*	> Given how many loops can coexist at this vertex, go through each case and add the relevant loops
		*	> Recursively call the function with the appropriate number of loops to generate the
		*	  remainder of each topology
		*/
	}

	return std::vector<Diagram>();
}
