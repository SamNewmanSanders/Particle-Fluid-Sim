#include "Simulation.h" // Include the header file

void Simulation::display() {

    window.clear();

    if (showDensities) {
        for (auto& cell : searchGrid) {

            sf::Vector2i currentCellCoord = cell.first;
            auto& particles = cell.second;
            int numParticles = particles.size();

            sf::RectangleShape gridSquare;
            gridSquare.setPosition(vectorIntToFloat(currentCellCoord));
            gridSquare.setSize(sf::Vector2f(gridSize, gridSize));
            
            auto colour = sf::Color(255,255,255, std::min(numParticles*8, 255));

            gridSquare.setFillColor(colour);

            window.draw(gridSquare);
        }
    }

    for (Particle* particle : particleList) {


        window.draw(particle->shape);
    }

    gui.draw();
    window.display();

}