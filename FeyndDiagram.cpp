#include "FeyndDiagram.h"

int main()
{

	using std::cout, std::endl;
	/*
	//TEST CODE
	P p1{ P::phi };
	P p2{ P::phi };
	P p3{ P::phi };
	P p4{ P::phi };

	LoopDiagram ldb({ p1, p2, p3, p4 });

	auto returns{ connect1PI(ldb,1,Model::loopy_vertices,Model::n_to_0,Model::n_to_1) };

	for (Diagram& ld : returns) {
		std::cout << ld.cleanup() << '\n';
	}
	*/
	
	Particle p1(P::phi);
	Particle p2(P::phi);
	Particle p3(P::chi);
	Particle p4(P::chi);
	Particle p5(P::phi);
	Particle p6(P::phi);
	Particle p7(P::chi);
	Particle p8(P::chi);
	p1.setIdenticality(0);
	p2.setIdenticality(0);
	p3.setIdenticality(1);
	p4.setIdenticality(1);
	p5.setIdenticality(0);
	p6.setIdenticality(0);
	p7.setIdenticality(1);
	p8.setIdenticality(1);

	std::vector<std::vector<std::vector<Particle>>> glist{
		{{p1,p2},{p3,p4}},
		{{p7,p8},{p5,p6}}
	};

	std::set<std::multiset<std::map<int, int>>> signature_dictionary;
	//Loop through all groupings
	int loc{};
	while (loc < glist.size()) {
		//Consider the current grouping
		std::multiset<std::map<int, int>> current_grouping_signature;
		//Go through each group in the grouping
		for (const auto& particle_group : glist[loc]) {
			//Set up the signature for this group
			std::map<int, int> current_group_signature;
			//Populate the signature as per similar code above
			for (const Particle& p : particle_group) {
				if (p.hasIdenticality()) {
					++current_group_signature[p.getIdenticality()];
				}
				else {
					++current_group_signature[-p.getID()];
				}
			}
			//Add this signature to the grouping multiset
			current_grouping_signature.insert(current_group_signature);
		}
		//Check if we need to remove this grouping because it is a duplicate
		if (signature_dictionary.find(current_grouping_signature) == signature_dictionary.end()) {
			signature_dictionary.insert(current_grouping_signature);
			++loc;
		}
		else {
			glist.erase(glist.begin() + loc);
		}
	}
	for (auto& g : glist) {
		for (auto& v : g) {
			for (auto& p : v) {
				cout << p << ", ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}

}