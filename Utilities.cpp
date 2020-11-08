//Utilities.cpp : 

#include "Utilities.h"

#include <chrono>
#include <iostream>
#include <vector>

double Timer::elapsed() const {
	return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}