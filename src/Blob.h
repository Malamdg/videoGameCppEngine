#pragma once
#include "Particle.h"
#include "Vector3D.h"
#include "Forces/ParticleForceRegistry.h"

class Blob {
private:
	Particle* m_core;
	std::list<Particle*> m_particles;

public:
	Blob(Particle* core = new Particle());
	~Blob();

	Particle* getCore();
	std::list<Particle*> getParticles();
	void merge(Particle* particle);
	void linkParticles(ParticleForceRegistry* forceRegistry);

};