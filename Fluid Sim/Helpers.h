#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>


// Function declarations

float magnitude(sf::Vector2f vector);

//Converts a vector2i to a vector2f
sf::Vector2f vectorIntToFloat(sf::Vector2i vectorInt);

//Converts a vector2f to a vector2i (loses some info but doesn't really matter)
sf::Vector2i vectorFloatToInt(sf::Vector2f vectorFloat);

//Creates a random position on the screen, with an exclusion radius for the edges
sf::Vector2f randomPosition(int width, int height, float exclusionRadius);

// Changes coordinates to the grid integer coord
sf::Vector2i toGridCoord(sf::Vector2f floatCoord, int multiple);

sf::Vector2f multiply(sf::Vector2f vector, float scalar);


//Hash functions included in the header file as they are small (apparently you can do this)

struct Vector2iHasher {
    std::size_t operator()(const sf::Vector2i& vec) const {
        // A common hash combination approach: bitwise XOR and shifting
        // Combines the hash of x and y coordinates into a single hash value
        return std::hash<int>()(vec.x) ^ (std::hash<int>()(vec.y) << 1);
    }
};

// The equality function I think is sfml defined, will keep this as its useful to know (not used though)

struct Vector2iEquality {
    bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};


#include <iostream>
#include <chrono>

//timer for debuggin and efficiency purposes
class Timer {
public:
  Timer() {
    // Initialize the timer with the current time
    lastCallTime = std::chrono::high_resolution_clock::now();
  }

  void timeSinceLastCall() {
    // Get the current time
    auto now = std::chrono::high_resolution_clock::now();

    // Calculate the time difference in seconds
    std::chrono::duration<double> timeSpan = now - lastCallTime;

    // Update the last call time
    lastCallTime = now;

    // Return the time difference in seconds
    std::cout << timeSpan.count() << "Seconds" << std::endl;
  }

private:
  std::chrono::high_resolution_clock::time_point lastCallTime;
};

