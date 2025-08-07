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

	while (!system.detectCollision())
	{
		std::cout << "Step " << i << "\n\n";
		// accelerate particle
		system.initiateTimestep();

		std::cout << "Distance Apart: " << system.getApart() * 1e9 << "Nm \n";

		if (system.getisOrbit())
		{
			break;
		}
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