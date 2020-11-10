#include "FeyndDiagram.h"

int main()
{

	//TEST CODE
	P p1{ P::phi };
	P p2{ P::phi };
	P p3{ P::phi };
	P p4{ P::phi };
		
	LoopDiagram p({ p1,p2, p3,p4 });
	Timer t;
	auto returns{ connect1PI(p,1,Model::loopy_vertices,Model::n_to_0,Model::n_to_1) };
	auto time{ t.elapsed() };
	for (Diagram& ld : returns) {
		std::cout << ld.cleanup() << '\n';
	}
	std::cout << "\n\nCompleted in " << time << " seconds.\n\n";
}