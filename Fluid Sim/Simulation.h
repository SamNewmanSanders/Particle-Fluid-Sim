#pragma once

#include <vector>
#include <string>
#include <algorithm> //for std::clamp

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>  // Include TGUI main header
#include <TGUI/Backend/SFML-Graphics.hpp> //ChatGPT was too stupid to know this existed

#include "Particle.h" // Include own files
#include <Helpers.h>


class Simulation {
public:

    float forceConstant; //responsible for how much particles push each other. Public so gui can easily control
    float viscosity;
    bool showDensities;
    float gravity;

    // Constructor
    Simulation(sf::RenderWindow& window,tgui::Gui& gui, int width, int height, float particle_radius, int numparticles);

    // Starts the simulation and the variants
    void start();
    void random_positions();

    // Updates the simulation
    void updatePositions();
    void calculateDensities();
    void updateVelocities(); //called from the update function, responsible for the actual fluid sim logic!

    float distanceContribution(int distance, int gridSize, float exponentialConstant); //helper function of sorts with the smoothing radius function
    float distanceContributionGradient(int distance, int gridSize, float exponentialConstant); //so the particle knows the greatest rate of change of density from a single particle
    // note that due to the exponential nature of the function, they are almost identical. Keeping them seperate incase I want to change the function

    //Displays everything
    void display();

    //Input handling
    void rightClick(sf::Vector2f mouseposition);


private:

    // Display stuff
    int width;
    int height;
    int gridSize = 50;
    sf::RenderWindow& window; //window and gui must be references
    tgui::Gui& gui;

    int numparticles;
    float particle_radius;

    std::unordered_map< sf::Vector2i, std::vector<Particle*>, Vector2iHasher> searchGrid; //particle reference grid

    std::vector<Particle*> particleList; //to loop over during drawing / non grid based methods
};


