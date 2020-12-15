#pragma once

inline int numBitsSet(int n) {
	if (n == 0) return 0;
	else return (n & 1) + numBitsSet(n >> 1);
}