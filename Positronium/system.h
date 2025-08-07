#pragma once
#ifndef SYSTEM

#define SYSTEM
#include <cmath>
#include <array>
#include <random>
#include <cassert>

class System
{
private:

	double m_timestep{ 1e-23 };

	double m_reduced_mass{}; // reduced mass of the system
	double m_bind_energy{ -6.8 }; // n = 1 binding energy in eV.

	double m_sum_pot{};
	double m_apart{};	// distance apart
	std::array<Particle, 2> m_particles{};
	bool m_isOrbit{false};
	double m_elapsed{ 0 }; // in nanoseconds


	double m_system_energy{};	// total energy in electron volts
	double m_energy_kinetic{};
	double m_energy_coulomb{};

	double m_unit_x{};	// unit vector X
	double m_unit_y{};	// unit vector Y

	double m_change_dist{0}; //change in distance per since last timestep
	double m_change_vel{};	// change in velocity per timestep
	int m_emitted{}; // number of emitted photons
	double m_avg_recomb_area{};

	// RNG

	// engine
	std::mt19937_64 m_engine{};
	// define distribution
	std::uniform_real_distribution<double> m_uniform01;


	// when these are multiplied by vector quantity, they that in direction
public:
	System(Particle& part1, Particle& part2)
		: m_engine{ std::random_device{}() }
		, m_uniform01{ 0.0, 1.0 }
	{
		m_particles[0] = part1;
		m_particles[1] = part2;

		// set initial distance apart
		m_apart = distance();

		// calculates initial unit vector
		calculateVector();

		m_reduced_mass = reducedMass();
	}

	// moves particle by 1 timestep
	void initiateTimestep()
	{
		acceleration();	// accelerate 
		velocity();		// update velocity
		updateLocation();// update location
		calculateVector(); // obtains new direction (unit) vector
		photonsEmitted();
		updateTime();	// update time
	}

	void recombAreaTimestep()
	{
		while (true)
		{
			photonsEmitted(); // attempts a capture

			if (!orbitFormed()) { initiateTimestep(); }
			else { break; }
		}

		std::cout << "Orbit formed!";
	}
	void updateTime()
	{
		m_elapsed += m_timestep * 1e9;

		std::cout << "Elapsed: " << m_elapsed << " Ns \n";
	}
	
	double reducedMass()	// calculates the reduced mass
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };
		if (part1.getMass() == part2.getMass()) { return part1.getMass() / 2; }
		else { return 0; }
	}
	// calculations
	void sumPotential() // calculates the sum of potential relative to each particle's position
	{
		m_sum_pot = m_particles[0].relativePotential(m_particles[1]) + m_particles[1].relativePotential(m_particles[0]);
	}

	void acceleration()	// calculates current acceleration due to force
	{
		auto& part1{ m_particles[0] };
		auto& part2 {m_particles[1]};

		double a1{ part1.coulombForce(part2) / part1.getMass() }; // a for part1

		//std::cout << "a1 is " << a1 << " m/s\n";

		double a2{ -part2.coulombForce(part1) / part2.getMass() }; // a for part2

		//std::cout << "a2 is " << a2 << " m/s\n";

		// set acceleration for particle 1
		part1.setAccX(a1 * m_unit_x);	// multiply by unit vector to set direction of acceleration
		part1.setAccY(a1 * m_unit_y);

		// set acceleration for particle 2
		part2.setAccX(a2 * m_unit_x);
		part2.setAccY(a2 * m_unit_y);
	}

	void velocity()	// calculates current velocity due to acceleration
	{	
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		// velocity particle 1

		double v1_x{ part1.getVelX() + part1.getAccX() * m_timestep};	// accelerated for 1 timestep
		double v1_y{ part1.getVelY() + part1.getAccY() * m_timestep };

		// velocity 1 new magnitude
		double v1_mag{ sqrt(v1_x * v1_x + v1_y * v1_y) };

		part1.changeInVel(v1_mag); // calculates change in velocity 1
		
		//std::cout << "Velocity 1 = " << v1_mag << "m/s \n";

		// velocity particle 2

		double v2_x{ part2.getVelX() + part2.getAccX() * m_timestep };
		double v2_y{ part2.getVelY() + part2.getAccY() * m_timestep };

		//velocity 2 new magnitude
		double v2_mag{ sqrt(v2_x * v2_x + v2_y * v2_y) };

		part2.changeInVel(v2_mag); // calculates change in velocity 2

		//std::cout << "Velocity 2 = " << v2_mag << "m/s \n";

		// set velocity 1
		part1.setVelX(v1_x);
		part1.setVelY(v1_y);
		
		// set velocity 2
		part2.setVelX(v2_x);
		part2.setVelY(v2_y);

		totalKineticEnergy();
	}

	void totalKineticEnergy()	// kinetic energy relative to COM
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		double vel_rel{ (part1.getVelMag() + part2.getVelMag())/2 }; // velocity relative to COM

		m_energy_kinetic = { 1 / 2 * m_reduced_mass * vel_rel * vel_rel };
	}

	void updateLocation()
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };
		
		//update particle 1
		part1.setX(part1.getX() + part1.getVelX() * m_timestep);
		part1.setY(part1.getY() + part1.getVelY() * m_timestep);

		//update particle 2
		part2.setX(part2.getX() + part2.getVelX() * m_timestep);
		part2.setY(part2.getY() + part2.getVelY() * m_timestep);

		//calculate distance apart

		double new_apart{ distance() };

		//m_change_dist = m_apart - new_apart; // new_apart < m_apart
		m_apart = new_apart;

		//std::cout
			//<< "Separation = " << new_apart << " m, "
			//<< "dr = " << m_change_dist << " m\n";
	}

	void calculateVector()
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		double dist_x{ part1.getX() - part2.getX() };
		double dist_y{ part1.getY() - part2.getY() };

		double magnitude{ std::hypot(dist_x, dist_y) };

		// set unit vector x, y
		m_unit_x = dist_x / magnitude;
		m_unit_y = dist_y / magnitude;
	}


	const double distance()	// calculates current distance squared
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		double dist_x{ part1.getX() - part2.getX() };
		double dist_y{ part1.getY() - part2.getY() };

		return std::hypot(dist_x, dist_y);	// returns distance (position vector magnitude)
	}

	// calculates probability of capture per timestep once within recombination area
	// then calculates the energy loss in event of capture
	void photonsEmitted()	
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		double probability_capture{ part1.emitProbability(m_timestep)};

		// set event occurence probability
		double event{ m_uniform01(m_engine) + m_uniform01(m_engine)};

		if (event < probability_capture)
		{
			photonEmission_EnergyLoss();	// to calculate total energy loss
			return;
		}

		else
		{
			std::cout << "No emission for timestep.\n";
			return;
		}
	}

	void photonEmission_EnergyLoss()	// calculates the system energy after emission
	{
		auto& part1{ m_particles[0] };
		auto& part2{ m_particles[1] };

		// returns relative velocity (must be positive) of the two particles relative to COM.
		double vel_rel{ std::abs( part1.vel_mag().getVelMag() - part2.vel_mag().getVelMag()) };

		double rel_KE{ 1 / 2 * m_reduced_mass * vel_rel * vel_rel };
		double rel_KE_eV{ rel_KE / constants::e };

		double photon_eV{ rel_KE_eV - m_bind_energy };	// calculates photon energy

		m_system_energy -= photon_eV;	// removes it from system energy

		if (m_system_energy < m_bind_energy)
		{
			std::cout << "Positronium formed.\n";
			m_isOrbit = true;
		}
		
	}

	const bool detectCollision()
	{
		if (m_apart < constants::threshold)
			return true;
		else
			return false;
	}

	bool isInRecombArea()	// checks whether two particles are close enough for recombination event
	{
		double radius1{ m_particles[0].captureRadius()};
		double radius2{ m_particles[1].captureRadius() };

		// checks if particles are within capture radius.
		if (m_apart <= (radius1 + radius2) / 2) { return true; }	
		else { return false; }
	}

	bool orbitFormed()
	{
		if (m_system_energy < m_bind_energy) { return true; }
		else { return false; }
	}
	//accessors

	double getPotential()
	{
		return m_sum_pot;
	}

	double getTime() // in nanoseconds
	{
		return m_elapsed / 1e9;
	}

	double getApart()
	{
		return m_apart;
	}

	double getChange()
	{
		return m_change_dist;
	}

	double getisOrbit()
	{
		return m_isOrbit;
	}
};

#endif