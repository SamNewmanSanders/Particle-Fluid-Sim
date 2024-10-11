#include "Simulation.h" // Include the header file
#include "Helpers.h"
#include <cstdlib>       // For rand()
#include <vector>
#include <unordered_map>
#include <iostream>


void Simulation::updatePositions() {
    
    //Two pass algorithm so no particles are double moved (moved to a new grid cell then updated AGAIN)
    
    std::vector<std::pair<Particle*, sf::Vector2i>> particlesToMove; // Temporary storage for particle movements

    //First pass to update the position
    
    // First pass: Update particles but don't move them between grid cells yet
    for (auto& cell : searchGrid) {

        // Iterate over particles in the current cell
        auto& particles = cell.second;

        for (Particle* particle : particles) {

            particle->move(width, height); // Update particle position based on velocity

            sf::Vector2i newGridCell = toGridCoord(particle->position, gridSize);

            // If the particle moved to a new grid cell, store it in particlesToMove
            if (newGridCell != cell.first) {
                particlesToMove.push_back(std::make_pair(particle, newGridCell)); //the pair is the particle reference and the new grid cell
            }
        }
    }


    // Second pass: Remove particles from old cells and move them to new cells
    for (auto& movePair : particlesToMove) {

        Particle* particle = movePair.first;
        sf::Vector2i oldGridCell = toGridCoord(particle->previousPosition, gridSize);
        sf::Vector2i newGridCell = movePair.second;

        // Remove particle from the old grid cell
        auto& oldCellParticles = searchGrid[oldGridCell];
        oldCellParticles.erase(std::remove(oldCellParticles.begin(), oldCellParticles.end(), particle), oldCellParticles.end()); 

        // Add particle to the new grid cell
        searchGrid[newGridCell].push_back(particle);
    }

}

void Simulation::calculateDensities() {

    // Calculate the density for each particle based on its surrounding particles

    for (auto& cell : searchGrid) {

        sf::Vector2i currentCellCoord = cell.first;
        auto& particles = cell.second;

        //Make a vector of the current and surrounding cells to check against other particles
        std::vector<sf::Vector2i> neighbourCoords;
        neighbourCoords.push_back(currentCellCoord);
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, -gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, -gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, -gridSize));

        //iterate over each particle in the cell
        for (Particle* particle : particles) {

            float current_density = 0;

            //iterate over each neighbouring cell
            for (auto Coord : neighbourCoords) {

                std::vector<Particle*> neighbourParticles = searchGrid[Coord];

                //iterate over each particle in the neighbouring cell
                for (Particle* neighbourParticle : neighbourParticles) {

                    auto relativePosition = (neighbourParticle->position - particle->position);

                    float distance = magnitude(relativePosition);

                    // effect takes gridSize into account as the effect should tend to 0 as distance tends to gridSize (won't search any further anyway)
                    // it is a float between 0 and 1 as it is normalised
                    float effect = distanceContribution(distance, gridSize, 5);

                    current_density += effect;
                   
                }
            }

            particle->density = current_density;
        }
    }
}

//Possible performance enhancement by combining this function and the function above somehow
void Simulation::updateVelocities() {

    // Update the particle velocities based on the densities / distances of surrounding particles

    for (auto& cell : searchGrid) {

        sf::Vector2i currentCellCoord = cell.first;
        auto& particles = cell.second;

        //Make a vector of the current and surrounding cells to check against other particles

        std::vector<sf::Vector2i> neighbourCoords;
        neighbourCoords.push_back(currentCellCoord);
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(gridSize, -gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(0, -gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, gridSize));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, 0));
        neighbourCoords.push_back(currentCellCoord + sf::Vector2i(-gridSize, -gridSize));

        //iterate over each particle in the cell
        for (Particle* particle : particles) {

            auto velocityVector = sf::Vector2f(0.f, 0.f);

            //iterate over each neighbouring cell
            for (auto Coord : neighbourCoords) {

                std::vector<Particle*> neighbourParticles = searchGrid[Coord];

                //iterate over each particle in the neighbouring cell
                for (Particle* neighbourParticle : neighbourParticles) {


                        auto relativePosition = (neighbourParticle->position - particle->position);

                        float distance = magnitude(relativePosition);

                        velocityVector += multiply(relativePosition, distanceContributionGradient(distance, gridSize, 5));

                        //also add a viscosity force which also depends on distance

                        auto relativeVelocity = (neighbourParticle->velocity - particle->velocity);

                        velocityVector += multiply(relativeVelocity, viscosity  * distanceContribution(distance, gridSize, 12));
                    
                }
            }

            particle->velocity += multiply(velocityVector, forceConstant);
            
            //add gravity
            particle->velocity += sf::Vector2f(0, gravity);

            float dampingFactor = 0.96; //for troubleshooting
            particle->velocity *= dampingFactor;
        }
    }
}

float Simulation::distanceContribution(int distance, int gridSize, float exponentialConstant) {

    if (distance >= gridSize) {
        return 0.f;
    }

    // Ensure proper floating-point division by casting to float
    float normalisedDistance = static_cast<float>(distance) / static_cast<float>(gridSize);


    // Calculate the function value
    return std::exp(-exponentialConstant * normalisedDistance) * (1 - normalisedDistance); //seemed like a good function fit
}

float Simulation::distanceContributionGradient(int distance, int gridSize, float exponentialConstant) {

    if (distance >= gridSize) {
        return 0;
    }

    // Ensure proper floating-point division by casting to float
    float normalisedDistance = static_cast<float>(distance) / static_cast<float>(gridSize);


    return (exponentialConstant * normalisedDistance - exponentialConstant - 1) * std::exp(-exponentialConstant * normalisedDistance); //derivative

}