//Timer.cpp : Define the elapsed function for the Timer class

#include "Timer.h"

double Timer::elapsed() const {
	return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}