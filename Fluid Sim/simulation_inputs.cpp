#include "Simulation.h"

void Simulation::rightClick(sf::Vector2f mouseposition) {

	for (Particle* particle : particleList) {

		sf::Vector2f relativePosition = (mouseposition - particle->position);
		
		float distance = magnitude(relativePosition);

		float effect = distanceContribution(distance, 150, 6);

		particle->velocity += multiply(relativePosition, effect * 1);
	}
}
