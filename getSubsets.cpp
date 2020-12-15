#include "getSubsets.h"
#include "Grouping.h"

//Deprecated: pass to new function
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size)
{
	return Grouping::getSubsets(input, min_size);
}
//Deprecated: pass to new function
listofpairedgroupings getSubsets(const std::vector<Particle>& input, const size_t min_size, const size_t max_size)
{
	return Grouping::getSubsets(input, min_size, max_size);
}