#include "SpringParticleParticle.h"
#include "../Particle.h"
#include "ParticleForceGenerator.h"
#include "../Vector3D.h"

/*
class constructor

@param particle, the pointer to the particle where the spring is attached
@param elasticity, the elasticity of the spring
@param lenght, the lenght of the spring
@param C, a coefficient
*/
SpringParticleParticle::SpringParticleParticle(Particle* particle, float elasticity, float lenght, float C)
	:
	m_particle(particle),
	m_k(elasticity),
	m_l0(lenght),
	m_C(C),
	m_z(0),
	m_w(0)
{}

/*
class desctructor
*/
SpringParticleParticle::~SpringParticleParticle() { }

/*
update the particle's force

@param particle, the pointer to the particle to update
@param duration, frame duration when the spring's force applies
*/
void SpringParticleParticle::updateForce(Particle* particle, float duration)
{
	Vector3D direction = (m_particle->getPosition() - particle->getPosition());

	if (particle->getInverseMass() != 0 && !(direction == Vector3D()))
	{

		float invMass = particle->getInverseMass();
		/* l-l0 distance computation */
		float distance = m_l0 - direction.Norm();

		/* Force director vector */
		direction.Normalize();

		/* Velocity of the particle on the particle-fixation axis by projection */
		float velocityProj = particle->getVelocity() * direction;

		/* Calculation of natural frequency */
		m_w = sqrt(invMass * m_k);
		/* Calculation of the depreciation rate */
		m_z = m_C * invMass / 2 * m_w;

		float coeff = (-m_w * m_w * distance) - (2 * m_z * m_w * velocityProj);

		particle->addForce(direction * coeff);
	}
}
