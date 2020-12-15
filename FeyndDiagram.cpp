#include "FeyndDiagram.h"

int main()
{

	using std::cout, std::endl;

	//TEST CODE
	P p1{ P::phi };
	P p2{ P::phi };
	P p3{ P::phi };
	P p4{ P::phi };

	std::vector<Particle> p{ p1,p2, p3,p4 };
	double total_time{};
	Timer t;
	for (int d{}; d < 10000; ++d) {
		t.reset();
		Grouping g(p);
		total_time += t.elapsed();
	}
	cout << total_time << endl << endl << endl;

	Grouping g(p);

	for (const auto& pg : g.possible_groupings) {
		const group& grps{ pg.first };
		const std::vector<Particle>& ungrp{ pg.second };

		for (const std::vector<Particle>& grp : grps) {
			for (const Particle& p : grp) {
				cout << p << ", ";
			}
			cout << " | ";
		}
		cout << " X ";
		for (const Particle& p : ungrp) {
			cout << p << ", ";
		}
		cout << endl;
	}

	auto returns{ connect1PI(p,1,Model::loopy_vertices,Model::n_to_0,Model::n_to_1) };

	for (Diagram& ld : returns) {
		std::cout << ld.cleanup() << '\n';
	}


}