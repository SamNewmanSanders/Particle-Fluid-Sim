// Particle.cpp
#include "Particle.h"

Particle::Particle(int radius, sf::Vector2f initial_pos, sf::Vector2f initial_vel)
    : position(initial_pos), velocity(initial_vel), radius(radius) {
    shape.setFillColor(sf::Color::White);
    shape.setPosition(initial_pos);
    shape.setOrigin(radius, radius);
    shape.setRadius(radius);
}

void Particle::move(int width, int height) {
    previousPosition = position;

    // Update position based on velocity
    position += velocity;

    // Check for boundary collisions
    if (position.x < 0) {
        position.x = 0; // Keep within left boundary
        velocity.x = -velocity.x; // Reverse velocity on hit
    }
    else if (position.x > width) {
        position.x = width; // Keep within right boundary
        velocity.x = -velocity.x; // Reverse velocity on hit
    }

    if (position.y < 0) {
        position.y = 0; // Keep within top boundary
        velocity.y = -velocity.y; // Reverse velocity on hit
    }
    else if (position.y > height) {
        position.y = height; // Keep within bottom boundary
        velocity.y = -velocity.y; // Reverse velocity on hit
    }

    // Update the shape's colour
    setColour();

    // Update the shape's position
    shape.setPosition(position);
}

void Particle::setColour() {

    //Makes it redder if the ball is travelling faster
    int red = std::max(0, std::min(255, 30 * static_cast<int>(magnitude(velocity))));

    sf::Color colour(red, 255 - red, 147 - red/2); //for aesthetics

    shape.setFillColor(colour);
}