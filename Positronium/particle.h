#pragma once
#ifndef PARTICLE
#define PARTICLE

#include "constants.h"
#include <iostream>
#include <cmath>

class Particle
{
private:
	double m_mass{};
	double m_charge{};
	double m_angle{};	// in radians

	double m_x{};
	double m_y{};
	double m_acc_x{};
	double m_acc_y{};
	double m_vel_mag{};
	double m_velocity_x{};
	double m_velocity_y{};
	double m_delta_v{};

	double m_potential{};

	double m_cross{}; // particle cross-section
	double m_capture_rad{}; // recombination capture radius
	double m_rate{}; // rate of photon emission


public:
	Particle() = default;

	Particle(const double mass, const double charge, double start_x, double start_y)
	: m_mass{ mass }, m_charge{ charge }{
		m_x = start_x;
		m_y = start_y;
		m_velocity_x = 0;
		m_velocity_y = 0;

		m_angle = atan(start_y / start_x);

	};
	//acessors
	
	const double getCharge()
	{
		return m_charge;
	}

	double setCharge(double charge)
	{
		m_charge = charge;
	}

	const double getX()
	{
		return m_x;
	}

	void setX(double x)
	{
		m_x = x;
	}

	const double getY()
	{
		return m_y;
	}

	void setY(double y)
	{
		m_y = y;
	}

	double getMass()
	{
		return m_mass;
	}

	void setAccX(double acceleration)
	{
		m_acc_x = acceleration;
	}

	double getAccX()
	{
		return m_acc_x;
	}

	void setAccY(double acceleration)
	{
		m_acc_y = acceleration;
	}

	double getAccY()
	{
		return m_acc_y;
	}

	void setVelX(double velocity)
	{
		m_velocity_x = velocity;
	}

	double getVelX()
	{
		return m_velocity_x;
	}

	double getVelY()
	{
		return m_velocity_y;
	}

	void setVelY(double velocity)
	{
		m_velocity_y = velocity;
	}

	double getVelMag()
	{
		return m_vel_mag;
	}



	double getAngle()
	{
		return m_angle;
	}

	//equations
	const double relativePotential(Particle& part2) // calculates relative potential to particle2
	{
		using namespace constants;

		double relPot{ coulombs * m_charge / sqrt(distSquared(part2)) };

		std::cout << "Relative Potential: " << relPot;


		return relPot;
	};

	const double distSquared(Particle& part2)	// calculates distance squared
	{
		double x{ m_x - part2.getX() };
		double y{ m_y - part2.getY() }; 

		return x * x + y * y;
	}

	const double coulombForce(Particle& part2)	// force acting on the particle due to part2
	{
		using namespace constants;

		return coulombs * (m_charge * part2.getCharge())/distSquared(part2);
	}

	void crossSection()	// calculates particle's cross section relative to current velocity
	{
		using namespace constants;

		double vel_mag{ std::hypot(m_velocity_x, m_velocity_x) };	// calculate magnitude of velocity at current timestep
		double photon_freq{ 1 + (vel_mag * vel_mag) / (2.0 * light * light) }; // calculates photon frequency

		double denominator{ vel_mag * photon_freq * photon_freq };	// v^2 * w^2
		double numerator{ fine_struct * fine_struct * fine_struct * bohr_rad * bohr_rad }; // a^3 * a0^2

		double cross_section{ numerator / denominator };	// cross section equation

		m_cross = cross_section;	// set particle's current cross section
	}

	double captureRadius()	// calculates recombination area
	{
		using namespace constants;

		crossSection(); // calculates cross section at current timestep

		m_capture_rad = { std::sqrt(m_cross / pi) };

		//std::cout << "Capture radius is " << m_capture_rad * 1e9 << " nm\n";

		return m_capture_rad;
	}
	
	void rateEmission()
	{
		// since number density = 1, hence flux = velocity
		double vel_mag{ std::hypot(m_velocity_y, m_velocity_x) };
		double rate{ vel_mag * m_cross }; // rate of photon emission

		m_rate = rate;
	}

	double emitProbability(double timestep)
	{
		rateEmission(); // calculates emission rate

		double probability{ 1 - std::exp(-m_rate * timestep) }; // poisson distribution
		std::cout << std::scientific
			<< "Probability of capture: "
			<< probability
			<< std::defaultfloat << '\n';
		return probability;
	}

	auto vel_mag()
	{
		double mag = std::hypot(m_velocity_x, m_velocity_y);
		m_vel_mag = mag;
		return *this;
	}

	double changeInVel(double final)
	{
		m_delta_v = final - m_vel_mag;
		return m_delta_v;
	}

};


#endif