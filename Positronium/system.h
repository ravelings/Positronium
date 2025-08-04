#pragma once
#ifndef SYSTEM

#define SYSTEM
#include <cmath>
#include <array>

class System
{
private:

	double m_timestep{ 1e-23 };

	double m_sum_pot{};
	double m_apart{};	// distance apart
	std::array<Particle, 2> m_particles{};
	bool is_circular{};
	double m_elapsed{ 0 }; // in nanoseconds

	double m_unit_x{};	// unit vector X
	double m_unit_y{};	// unit vector Y

	double m_change_dist{0}; //change in distance per since last timestep

	// when these are multiplied by vector quantity, they that in direction
public:
	System(Particle& part1, Particle& part2)
	{
		m_particles[0] = part1;
		m_particles[1] = part2;

		// set initial distance apart
		m_apart = distance();

		// calculates initial unit vector
		calculateVector();
	}

	void updateTime()
	{
		m_elapsed += m_timestep * 1e9;

		std::cout << "Elapsed: " << m_elapsed << " Ns \n";
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

		std::cout << "a1 is " << a1 << " m/s\n";

		double a2{ -part2.coulombForce(part1) / part2.getMass() }; // a for part2

		std::cout << "a2 is " << a2 << " m/s\n";

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

		// velocity 1 magnitude
		double v1_mag{ sqrt(v1_x * v1_x + v1_y * v1_y) };
		
		std::cout << "Velocity 1 = " << v1_mag << "m/s \n";

		// velocity particle 2

		double v2_x{ part2.getVelX() + part2.getAccX() * m_timestep };
		double v2_y{ part2.getVelY() + part2.getAccY() * m_timestep };

		//velocity 2 magnitude
		double v2_mag{ sqrt(v2_x * v2_x + v2_y * v2_y) };

		std::cout << "Velocity 2 = " << v2_mag << "m/s \n";

		// set velocity 1
		part1.setVelX(v1_x);
		part1.setVelY(v1_y);
		
		// set velocity 2
		part2.setVelX(v2_x);
		part2.setVelY(v2_y);
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

		m_change_dist = m_apart - new_apart; // new_apart < m_apart
		m_apart = new_apart;

		std::cout
			<< "Separation = " << new_apart << " m, "
			<< "Δr = " << m_change_dist << " m\n";
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

	const bool detectCollision()
	{
		if (m_apart < constants::threshold)
			return true;
		else
			return false;
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
};

#endif