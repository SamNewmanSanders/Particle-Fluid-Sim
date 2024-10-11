#include <Simulation.h>
#include <string>

// Constructor definition
Simulation::Simulation(sf::RenderWindow& window, tgui::Gui& gui, int width, int height, float particle_radius, int numparticles)
    : window(window), gui(gui), width(width), height(height), particle_radius(particle_radius), numparticles(numparticles) {}

// Method to start the simulation
void Simulation::start() {
    random_positions();
    std::cout << "Generating random positions" << std::endl;
}

void Simulation::random_positions() {

    for (int i = 0; i < numparticles; i++) {

        Particle* newParticle = new Particle(particle_radius, randomPosition(width, height, particle_radius)); //use default 0 velocity

        newParticle->index = i; //this will not be changed

        searchGrid[toGridCoord(newParticle->position, gridSize)].push_back(newParticle); 

        particleList.push_back(newParticle);
    }
}
        
