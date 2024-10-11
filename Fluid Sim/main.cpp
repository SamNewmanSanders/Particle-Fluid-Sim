#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <TGUI/TGUI.hpp>  // Include TGUI main header
#include <TGUI/Backend/SFML-Graphics.hpp> //ChatGPT was too stupid to know this existed

#include <cstdlib>

//Include own files too!
#include "Particle.h"
//helpers.h is already included from simulation
#include "Simulation.h"
   
int main(){

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    int width = 1800;
    int height = 900;

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(width, height), "Fluid Sim");

    window.setFramerateLimit(30); 

    //initialise timer for optimization purposes
    Timer timer;

    // Create a TGUI gui context
    tgui::Gui gui(window);

    //Create a simulation
    Simulation simulation(window, gui, width, height, 4 , 1000); //replace with slider afterwards

    //Create TGUI elements
    auto forceSlider = tgui::Slider::create();

    forceSlider->setPosition(100, 50);  // Position in window
    forceSlider->setSize(200, 20);      // Size of the slider
    forceSlider->setMinimum(0.f);         // Min value
    forceSlider->setMaximum(0.35f);      // Max value
    forceSlider->setStep(0.001f);
    forceSlider->setValue(0.f);         // Initial value
    simulation.forceConstant = forceSlider->getValue();

        forceSlider->onValueChange([&simulation](float newValue) {
        simulation.forceConstant = newValue;
        });

    gui.add(forceSlider);

    auto viscositySlider = tgui::Slider::create();

    viscositySlider->setPosition(100, 120);  // Position in window
    viscositySlider->setSize(200, 20);      // Size of the slider
    viscositySlider->setMinimum(0.f);         // Min value
    viscositySlider->setMaximum(7.5f);      // Max value - avoids crazy chaotic stuff
    viscositySlider->setStep(0.1f);
    viscositySlider->setValue(4.f);         // Initial value
    simulation.viscosity = viscositySlider->getValue();

    viscositySlider->onValueChange([&simulation](float newValue) {
        simulation.viscosity = newValue;
        });

    gui.add(viscositySlider);

    auto gravitySlider = tgui::Slider::create();

    gravitySlider->setPosition(100, 180);  // Position in window
    gravitySlider->setSize(200, 20);      // Size of the slider
    gravitySlider->setMinimum(0.f);         // Min value
    gravitySlider->setMaximum(0.5f);      // Max value
    gravitySlider->setStep(0.01f);
    gravitySlider->setValue(0.f);         // Initial value
    simulation.gravity = gravitySlider->getValue();

    gravitySlider->onValueChange([&simulation](float newValue) {
        simulation.gravity = newValue;
        });

    gui.add(gravitySlider);

    // Create a checkbox
    auto showGrid = tgui::CheckBox::create();
    showGrid->setPosition(100, 250);  // Set position in the window
    showGrid->setText("Show Grid Densities"); // Label for the checkbox
    simulation.showDensities = false;

    // Connect the onCheck/onUncheck signal to control the bool
    showGrid->onCheck([&simulation] {
        simulation.showDensities = true;
        });

    showGrid->onUncheck([&simulation] {
        simulation.showDensities = false;
        });

    // Add the checkbox to the gui
    gui.add(showGrid);

    //Start simulation
    simulation.start();

    std::cout << ("Starting sim") << std::endl;

    //Mouse button presses
    bool LeftisPressed = false;
    bool RightisPressed = false;

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check if the left mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                LeftisPressed = true;
            }
            // Check if the left mouse button is released
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                LeftisPressed = false;
            }
            // Check if the right mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                RightisPressed = true;
            }
            // Check if the right mouse button is released
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                RightisPressed = false;
            }

            // Pass events to TGUI
            gui.handleEvent(event);
        }

        if (RightisPressed) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            simulation.rightClick(vectorIntToFloat(mousePosition));
        }


        simulation.updatePositions();
        //std::cout << "Update Positions" << std::endl;
        //timer.timeSinceLastCall();

        simulation.calculateDensities();
        //std::cout << "Calculate Densities" << std::endl;
        //timer.timeSinceLastCall();

        simulation.updateVelocities();
        //std::cout << "Update Velocities" << std::endl;
        //timer.timeSinceLastCall();

        simulation.display();
        //std::cout << "Display" << std::endl;
        //timer.timeSinceLastCall();
    }

    return 0;
}