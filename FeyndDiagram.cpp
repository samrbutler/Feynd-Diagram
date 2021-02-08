#include "FeyndDiagram.h"

int main()
{

	using std::cout, std::endl;
	
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
	/*
	Particle p1(P::phi);
	Particle p2(P::phi);
	Particle p3(P::chi);
	Particle p4(P::chi);
	p1.setIdenticality(0);
	p2.setIdenticality(0);
	p3.setIdenticality(1);
	p4.setIdenticality(1);

	std::vector<std::vector<Particle>> plist{
		{p1,p2,p3,p4},
		{p1,p2,p3},
		{p1,p2,p4},
		{p1,p3,p4},
		{p2,p3,p4}
	};

	std::set<std::map<int, int>> signature_dictionary;
	int loc{};
	while (loc < plist.size()) {
		std::map<int, int> current_mapping;
		for (const Particle& p : plist[loc]) {
			if (p.hasIdenticality()) {
				++current_mapping[p.getIdenticality()];
			}
			else {
				++current_mapping[-p.getID()];
			}
		}
		if (signature_dictionary.find(current_mapping) == signature_dictionary.end()) {
			signature_dictionary.insert(current_mapping);
			++loc;
		}
		else {
			plist.erase(plist.begin() + loc);
		}
	}

	for (auto& l : plist) {
		for (auto& p : l) {
			cout << p << ", ";
		}
		cout << endl;
	}
	*/

}