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


}