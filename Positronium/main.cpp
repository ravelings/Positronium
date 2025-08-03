#include "particle.h"
#include "constants.h"
#include "system.h"
#include <iostream>
#include <array>
#include <limits>

void simulation(System& system)
{
	constexpr double threshold{ constants::e_radius * 2 };

	int i{ 0 };
	
	//while (std::abs(system.getApart()) < threshold)
	while (i < 188)
	{
		std::cout << "Step " << i << "\n\n";
		// accelerate particle
		system.acceleration();	// accelerate
		system.velocity();		// update velocity
		system.updateLocation();// update location
		system.updateTime();	// update time

		std::cout << "Distance Apart: " << system.getChange() * 1e9 << "Nm \n";

		i++;
	}

	std::cout << "Lifetime: " << system.getTime() * 1e9 << " Ns \n";

}



int main()
{

	using namespace constants;

	const double r20{ e_radius * 20 }; // 20x electron radius

	// Particle(mass, charge, x, y)

	Particle electron(e_restmass, -e, r20, 0);
	Particle positron(e_restmass, e, -r20, 0);

	System system1(electron, positron);

	simulation(system1);

	return 0;

}