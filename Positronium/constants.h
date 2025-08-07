#pragma once
#ifndef CONSTANT
#define CONSTANT

namespace constants
{
	// to 4 s.f in SI units 
	// 
	//standard constant
	
	constexpr double pi{ 3.142 };

	// Electrodynamics
	constexpr double perm{ 1.257e-6 };
	constexpr double coulombs{ 8.988e9 };
	constexpr double light{ 2.998e8 };

	// Quantum
	constexpr double e{ 1.602e-19 };
	constexpr double e_restmass{ 9.109e-31 };
	constexpr double e_radius{ 2.818e-15 };

	constexpr double fine_struct{ 7.297e-3 };
	constexpr double bohr_rad{ 5.292e-11 };

	// Simulation Constraints
	constexpr double threshold{ constants::e_radius * 2 }; // minimum distance before collision occurs
	constexpr double m_timestep{ 1e-23 };	// timestep for simulation
}

#endif