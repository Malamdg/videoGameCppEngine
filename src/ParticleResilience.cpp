#include "Particle.h"
#include "ParticleForceGenerator.h"
#include "Vector3D.h"

class ParticleResilience : public ParticleForceGenerator {
	/* elasticity constant */
	float m_k;

	/*
	class constructor

	@param elasticity, elasticity constant
	*/
	ParticleResilience::ParticleResilience(float elasticity) : m_k(elasticity) {}

	/*
	update the particle's gravity

	@param *particle, the particle to update
	@param duration, frame duration when the gravity applies
	*/
	virtual void updateForce(Particle* particle, float duration) {
		particle->addForce(m_k * (1 / particle->getInverseMass());
	}
};