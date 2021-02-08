#include "1PI.h"

std::vector<LoopDiagram> connect1PI(const LoopDiagram& diag, const int num_loops,
    const std::vector<LoopyVertex>& loopyvs, const n0dict& nto0, const n1dict& nto1)
{

    std::vector<Particle> external_particles{ diag.getExterns() };
    std::vector<LoopDiagram> list_of_diagrams;

    /* ALGORITHM - we take a vector of m external particles and return all *specific* 1PI completions at num_loops
    *   > Declare (without loss of generality since this is 1PI) the first external particle to be the starting point of the first loop
    *   > Go through all the 1->n vertices, spawning (n-1) new loops
    *		- If connection fails, skip this vertex. Otherwise, add the result to the output list
    *	> Now consider pairing the first external particle to all other inbound particles in turn and go through the 2->n vertices.
    *		- Repeat the above subalgorithm with the new subgraphs
    *	> Keep going until the first external particle is being grouped with
    *		A) (m-2) of the remaining external particles (i.e. after introducing loops they have somewhere to connect to)
    *	 or B) (max_int-2) others for max_int the largest number of particles that can simultaneously interact at a vertex
    *	> If condition A was met, now check if this could be a loopy vertex (these have been generated in advance in generic form)
    */

    //Take the first particle to be the reference point
    const Particle& loop_start{ external_particles[0] };

    //Collect the other particles in a vector: we can select from these to combine with the reference particle
    const std::vector<Particle> particles_available(external_particles.begin() + 1, external_particles.end());

    //Choose how many more particles are going to combine before spawning the loop
    for (int i{ 0 }; i <= std::min(static_cast<int>(external_particles.size()) - 2, Model::max_legs - 2); ++i) {
        //Get the list of new particles that are going to combine

        SubsetList choices(particles_available, i, i);

        //Add the starting particle to each set
        for (pairedsubset& pg : choices.subsets) {
            pg.first.insert(pg.first.begin(), loop_start);
        }

        //Remove physically identical groupings using identicality indices:

        //Create a set to keep track of "grouping" signatures
        std::set<std::map<int, int>> signature_dictionary;
        //Loop through all combinations until we've considered all options
        int loc{};
        while (loc < choices.subsets.size()) {
            //Set up the signature for this subset
            std::map<int, int> current_signature;
            //Go through all particles that are set to group
            for (const Particle& p : choices.subsets[loc].first) {
                //If the particle has an identicality condition, increase the number of occurrences in the signature
                if (p.hasIdenticality()) {
                    ++current_signature[p.getIdenticality()];
                }
                //If not, then it is a distinguished particle: add the negative of its ID (all IDs are strictly positive)
                else {
                    ++current_signature[-p.getID()];
                }
            }
            //If we haven't seen this signature before, this is a physically distinct grouping, so we leave it be
            if (signature_dictionary.find(current_signature) == signature_dictionary.end()) {
                signature_dictionary.insert(current_signature);
                ++loc;
            }
            //If we have, this grouping is already included, so remove this copy ([[loc]] is now pointing to the next element) 
            else {
                choices.subsets.erase(choices.subsets.begin() + loc);
            }
        }

        //For each choice:
        for (const pairedsubset& pg : choices.subsets) {

            //Set up the list of particles to combine
            const std::vector<Particle>& to_combine{ pg.first };

            //Set up the list of particles that will be left over
            std::vector<Particle> particles_available_after_grouping{ pg.second };

            //Spawn the new particles
            newloopvalues new_particle_data{ spawnLoops(std::vector<Particle> {to_combine}, num_loops) };

            //For each "spawned loop":
            for (std::pair<std::vector<Particle>, LoopyVertex>& new_set : new_particle_data) {
                //Start to create the vertex particle list
                std::vector<Particle> new_vertex{ to_combine };
                //Work out how many loops are left
                int remaining_loops = num_loops - static_cast<int>(new_set.first.size()) + 1;
                if (!new_set.second.is_null) remaining_loops -= new_set.second.num_loops;
                //Set up the new external particle container for the subdiagram
                std::vector<Particle> new_externs{ particles_available_after_grouping };
                //For each particle spawned:
                for (Particle& p : new_set.first) {
                    //Toggle to the antiparticle and add it to the vertex
                    new_vertex.push_back(p.toggleAntiPart());
                    //Toggle back to the particle and add it to the external container
                    new_externs.push_back(p.toggleAntiPart());
                }

                //Create the base diagram and add the distinguished (loopy) vertex
                LoopDiagram base_diagram(external_particles);
                if (new_set.second.is_null) base_diagram.addVertex(Vertex(new_vertex));
                else base_diagram.addLoopyVertex(new_set.second, new_vertex);

                LoopDiagram base_subdiagram(new_externs);
                //Connect the subdiagram

                std::vector<LoopDiagram> subdiagrams{ connectLoopDiagram(base_subdiagram, remaining_loops, loopyvs, nto0, nto1, true) };
                for (LoopDiagram& ld : subdiagrams) {
                    LoopDiagram to_add(base_diagram);
                    to_add.addVertices(ld.getVertices());
                    list_of_diagrams.push_back(to_add);
                }
            }
        }
    }

    if (external_particles.size() <= Model::max_legs) {
        //Get the external particle types
        std::multiset<P> externparttypes;
        for (const Particle& part : external_particles) {
            externparttypes.insert(part.getType());
        }
        //Go through each loopy vertex
        for (const LoopyVertex& lv : loopyvs) {
            //If the vertex produces external particles or adds the wrong number of loops, skip it
            if ((lv.external_particles_out != std::multiset<P>{}) || (lv.num_loops != num_loops)) continue;
            //Otherwise, check that the external particles match and add a diagram with the loopyvertex
            if (lv.external_particles_in == externparttypes) {
                LoopDiagram toadd(external_particles);
                toadd.addLoopyVertex(lv, external_particles);
                list_of_diagrams.push_back(toadd);
            }
        }
    }

    return list_of_diagrams;
}

std::vector<LoopDiagram> connectLoopDiagram(LoopDiagram& diag, const int num_loops, const std::vector<LoopyVertex>& loopyvs,
    const n0dict& nto0, const n1dict& nto1, const bool is1PI)
{
    //Get the external particles
    const std::vector<Particle>& externs{ diag.getExterns() };

    //If the loop momenta will not cancel out, return early as no completion will be valid
    if (getLoopSignature(externs).size() != 0) return {};

    //If we are looking to complete a 1PI diagram, we need to check there are actually loop momenta present, if not we have failed, so return early
    if (is1PI) {
        bool loops_exist{ false };
        for (const Particle& p : externs) {
            if (p.getLoops().size() != 0) loops_exist = true;
        }
        if (!loops_exist) return {};
    }

    //Create empty output container
    std::vector<LoopDiagram> returnvec;

    //If this isn't actually a loop process, divert to a slightly modified standard connection algorithm
    if (num_loops == 0) {
        std::vector<LoopDiagram> zero_loop_completions{ connectLoopDiagramZero(diag, nto0, nto1) };
        //Extend these diagrams to loop diagrams and add them to the output
        for (const LoopDiagram& d : zero_loop_completions) returnvec.push_back(d);
        return returnvec;
    }

    //Store the number of external particles
    size_t s{ externs.size() };

    //Get the 1PI completions directly
    std::vector<LoopDiagram> opi_completions{ connect1PI(diag, num_loops, loopyvs, nto0, nto1) };
    returnvec.insert(returnvec.end(), opi_completions.begin(), opi_completions.end());

    //Group up particles in groups of size >= 1
    listofpairedgroups groupinglist{ Grouping(externs,1).possible_groupings };

    //TODO: Add code to check for and remove physical identicality in the grouping list

    //Go through all groupings
    for (pairedgroup& pg : groupinglist) {
        //Get the number of groups in the grouping
        int num_groups{ static_cast<int>(pg.first.size()) };


        //TODO: What on earth have I done here? Sort it out...


        //Set up containers to store all possible vertices, loopy vertices and 1PI completions for a group
        using lprods = std::vector<std::pair<Particle, LoopyVertex>>;
        using opiprods = std::vector<std::pair<std::pair<Particle, LoopDiagram>, int>>;
        std::vector<std::vector<Particle>> tree_grouping_products;
        std::vector<lprods> loopy_grouping_products;
        std::vector<opiprods> opi_grouping_products;

        //Go through each group and fill in the containers
        for (std::vector<Particle>& g : pg.first) {
            //Get the outbound loop signature
            std::vector<int> loopsig{ invertLoopSignature(getLoopSignature(g)) };

            std::vector<P> product_types{ getProducts(g, nto1) };
            std::vector<Particle> product_particles;
            for (P p : product_types) product_particles.push_back(Particle(p, true, loopsig));
            //Get tree-level products using the nto1 dictionary
            tree_grouping_products.push_back(product_particles);

            //Get loopyvertex products using the loopyvertex dictionary
            lprods loopy_products;
            if (g.size() < Model::max_legs) {
                //Go through every possible output product we might expect
                for (P inbound_prod_type : Model::all_particle_types) {
                    //Get all the external particle types
                    std::multiset<P> externparttypes{ inbound_prod_type };
                    for (const Particle& part : g) {
                        externparttypes.insert(part.getType());
                    }
                    //Go through each loopy vertex
                    for (const LoopyVertex& lv : loopyvs) {
                        //If the vertex produces external particles or adds the wrong number of loops, skip it
                        if ((lv.external_particles_out != std::multiset<P>{}) || (lv.num_loops != num_loops)) continue;
                        //Otherwise, check that the external particles match and add to the list
                        if (lv.external_particles_in == externparttypes) {
                            loopy_products.push_back(std::make_pair(Particle(getAntiParticle(inbound_prod_type), true, loopsig), lv));
                        }
                    }
                }
            }
            //Add the possibilities to the list
            loopy_grouping_products.push_back(loopy_products);

            //Get the 1PI completions:
            opiprods opi_products;
            //Go through every external particle
            for (P inbound_prod_type : Model::all_particle_types) {
                for (int nl{}; nl <= num_loops; ++nl) {
                    std::vector<Particle> externs = g;
                    externs.push_back(Particle(getAntiParticle(inbound_prod_type)));
                    //TODO: Change the signature here to pass arguments
                    std::vector<LoopDiagram> opicompletions{ connect1PI(LoopDiagram(externs),nl) };
                    for (LoopDiagram& ld : opicompletions) {
                        opi_products.push_back(std::make_pair(std::make_pair(
                            Particle(getAntiParticle(inbound_prod_type), true, loopsig), ld), nl));
                    }
                }
            }
            opi_grouping_products.push_back(opi_products);
        }

        //Now choose which option we use for each group
        for (int x{}; x < pow(3, num_groups); ++x) {
            int* signature{ new int[num_groups] {} };
            int y = x;
            int index{};
            while (y > 0) {
                signature[index++] = y % 3;
                y /= 3;
            }
            bool is_valid{ true };
            for (int g{}; g < num_groups; ++g) {
                if (signature[g] == 0) {
                    //Use tree-level for this group
                    if (tree_grouping_products[g].size() == 0) {
                        is_valid = false;
                        break;
                    }
                }
                else if (signature[g] == 1) {
                    if (loopy_grouping_products[g].size() == 0) {
                        is_valid = false;
                        break;
                    }
                    //Use loopy-vertex for this group
                }
                else {
                    if (opi_grouping_products[g].size() == 0) {
                        is_valid = false;
                        break;
                    }
                    //Use 1PI competion for this group
                }
            }
            if (!is_valid) continue;


        }



    }




    return returnvec;
}

std::vector<LoopDiagram> connectLoopDiagramZero(LoopDiagram& diag, const n0dict& nto0, const n1dict& nto1, bool is1PI)
{
    //Store the current external vertices of this diagram
    const std::vector<Particle>& externs{ diag.getExterns() };

    //Store the number of external particles
    size_t s{ externs.size() };

    //Create empty return vector to add diagrams to
    std::vector<LoopDiagram> returnvec;

    //Check to see if we can form a vertex - we've already checked that the momenta cancel out
    if (diag.isVertex(nto0)) {
        //Add a diagram consisting of the external points and a vertex connecting them
        returnvec.emplace_back(LoopDiagram(externs, Vertex(externs)));
        return returnvec;
    }

    //If we don't have a vertex but there are now two or fewer particles left, this process has failed, so return an empty vector
    else if (s <= 2) return {};

    //Get all possible groupings
    listofpairedgroups groupinglist{ Grouping(externs).possible_groupings };
    //For each grouping in the list...
    for (pairedgroup grp : groupinglist) {
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
                std::vector<int> outbound_signature{ invertLoopSignature(getLoopSignature(vertexparts)) };
                //Add the new (anti)particle to the external container and add it to the vertex
                Particle p_to_add(prodlist[j][i], true, outbound_signature);
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
            LoopDiagram subdiag{ particlestoadd };

            //Initiate a recursion by connecting the subdiagram
            std::vector<LoopDiagram> connectedsds{ connectLoopDiagram(subdiag, 0, Model::loopy_vertices, nto0, nto1, is1PI) };

            //If we got nothing back, this grouping has failed
            if (connectedsds.size() == 0) continue;

            //If we got diagrams back, for each subdiagram create a new diagram which combines the vertices...
            //...and external particles of the current diagram with the vertices of the subdiagram
            for (LoopDiagram& sd : connectedsds) {
                LoopDiagram newdiag(diag);
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