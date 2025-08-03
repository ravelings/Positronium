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
	double m_velocity_x{};
	double m_velocity_y{};

	double m_potential{};

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
	
};


#endif