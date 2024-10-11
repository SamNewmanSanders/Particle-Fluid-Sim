#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "Helpers.h"

class Particle {
public:

    int index; //to make sure particles arent checked against each other twice

    int radius;
    sf::Vector2f position;
    sf::Vector2f previousPosition;

    sf::Vector2f velocity;
    sf::CircleShape shape;

    float density;

    Particle(int radius, sf::Vector2f initial_pos, sf::Vector2f initial_vel = sf::Vector2f(0.f, 0.f));

    void move(int width, int height);

    void setColour(); //sets the color based on the particles velocity
};
