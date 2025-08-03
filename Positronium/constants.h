#pragma once
#ifndef CONSTANT
#define CONSTANT

namespace constants
{
	// to 4 s.f in SI units 
	// 
	//standard constant
	
	constexpr double pi{ 3.142 };

	//Electrodynamics
	constexpr double perm{ 1.257e-6 };
	constexpr double coulombs{ 8.988e9 };

	//Quantum
	constexpr double e{ 1.602e-19 };
	constexpr double e_restmass{ 9.109e-31 };
	constexpr double e_radius{ 2.818e-15 };
}

#endif