#include "Connection.h"

//Given a diagram and interaction dictionaries, return a vector of all possible completed tree-level diagrams
std::vector<Diagram> connect(Diagram& diag, const n0dict& nto0, const n1dict& nto1)
{
	/* ALGORITHM
	*	> Check if the external particles can connect directly into a vertex with at least two active particles
	*		> If they can, add the vertex and return the completed diagram
	*	> Check if we have two or fewer particles left without being able to connect them
	*		> If we do, the diagram has not connected, so return an empty vector
	*	> Otherwise, get all possible groupings of the external particles
	*		> For each grouping, get all possible products
	*			> If there are no products, skip this grouping
	*			> If each group does not include at least one active particle, skip this grouping
	*			> Go through all possible products and get the new external particles.
					> Set these new particles active, the other particles passive and make a subdiagram
	*				> Use recursion to connect the subdiagram
	*					> If the subdiagram returned connections, combine the subdiagram into the current diagram and add it to the list
	*					> If the subdiagram fails to connect, go to the next product
	*			> If all products failed to produce a connected diagram, go to the next grouping
	*		> If all groupings failed to connect, the diagram has failed to connect, so return an empty vector
	*/

	//Store the current external vertices of this diagram
	const std::vector<Particle>& externs{ diag.getExterns() };

	//Store the number of external particles
	size_t s{ externs.size() };

	//Create empty return vector to add Diagrams to
	std::vector<Diagram> returnvec;

	//Check to see if we can form a vertex
	if (diag.isVertex(nto0)) {
		//Add a diagram consisting of the external points and a vertex connecting them
		returnvec.push_back(Diagram(externs, Vertex(externs)));
	}

	//If we don't have a vertex but there are now two or fewer particles left, this process has failed, so return an empty vector
	else if (s <= 2) {
		//Return an empty vector
		return {};
	}

	//Construct the initial (un)grouped vertex list
	pairedgrouping input{ std::make_pair(grouping{},externs) };

	//Get all possible groupings
	listofpairedgroupings groupinglist{ getGroupings(input) };

	//For each grouping in the list...
	for (pairedgrouping& grp : groupinglist) {
		//...get a list of possible new products for this group
		listofproducts prodlist{ getNewExterns(grp, nto1) };
		
		//Skip this grouping if it doesn't produce any valid interactions
		if ((prodlist.size() == 0 || prodlist.size() == 1) && (prodlist[0].size() == 0)) continue;

		//Go through each possible interaction product
		for (size_t j{}; j < prodlist.size(); ++j) {
			//Set up the new particle and vertex containers
			std::vector<Particle> particlestoadd;
			std::vector<Vertex> verticestoadd;

			//Go through each of the groups that have been formed
			for (size_t i{}; i < grp.first.size(); ++i) {
				//Set up the new particle information containers for vertex production
				std::vector<Particle> vertexparts{ grp.first[i] };

				//Add the new (anti)particle to the external container and add it to the vertex
				Particle p_to_add(prodlist[j][i], true);
				particlestoadd.push_back(p_to_add);
				vertexparts.push_back(p_to_add.toggleAntiPart());

				//Create and add the vertex to the current diagram
				verticestoadd.push_back(Vertex(vertexparts));
			}

			//Now add the previous external particles to the new container, setting them inactive first
			for (Particle& ungrouped : grp.second) {
				particlestoadd.push_back(ungrouped.setActive(false));
			}

			//At this point we are ready to create a new subdiagram
			Diagram subdiag{ particlestoadd };

			//Initiate a recursion by connecting the subdiagram
			std::vector<Diagram> connectedsds{ connect(subdiag, nto0, nto1) };

			//If we got nothing back, this grouping has failed
			if (connectedsds.size() == 0) continue;

			//If we got diagrams back, for each subdiagram create a new diagram which combines the vertices... 
			//...and external particles of the current diagram with the vertices of the subdiagram
			for (Diagram& sd : connectedsds) {
				Diagram newdiag(diag);
				newdiag.addVertices(verticestoadd);
				newdiag.addVertices(sd.getVertices());

				//And add this new diagram to the return vector
				returnvec.push_back(newdiag);
			}
		}
	}

	//We're out of options so jump back up the chain
	return returnvec;
}