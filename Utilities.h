//Utilities.h : 

#pragma once

#include <chrono>
#include <iostream>
#include <vector>

class Timer {

	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:

	Timer() : m_beg(clock_t::now()) {};

	void reset() { m_beg = clock_t::now(); }

	double elapsed() const;
};