#include "Helpers.h"
#include <cstdlib> // For rand()

// Function definitions

float magnitude(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f vectorIntToFloat(sf::Vector2i vectorInt) {
    return sf::Vector2f(static_cast<float>(vectorInt.x), static_cast<float>(vectorInt.y));
}

sf::Vector2i vectorFloatToInt(sf::Vector2f vectorFloat) {
    return sf::Vector2i(static_cast<int>(vectorFloat.x), static_cast<int>(vectorFloat.y));
}

sf::Vector2f randomPosition(int width, int height, float exclusionRadius) {

    // Calculate valid range for x and y, where exclusion radius is the particles radius
    int minX = static_cast<int>(exclusionRadius);
    int maxX = width - static_cast<int>(exclusionRadius) - 1; // -1 for edge cases
    int minY = static_cast<int>(exclusionRadius);
    int maxY = height - static_cast<int>(exclusionRadius) - 1;

    // Ensure that the range is valid
    if (maxX <= minX || maxY <= minY) {
        throw std::runtime_error("Grid is too small for the given exclusion radius.");
    }

    // Generate a random position within the valid range
    int x = minX + std::rand() % (maxX - minX + 1);
    int y = minY + std::rand() % (maxY - minY + 1);

    return vectorIntToFloat(sf::Vector2i(x, y));
}

sf::Vector2f multiply(sf::Vector2f vector, float scalar) {
    return sf::Vector2f(vector.x * scalar, vector.y * scalar);
}


// Function to convert a floating-point value to the nearest grid coordinate
sf::Vector2i toGridCoord(sf::Vector2f floatCoord, int gridSize) {

    // Function to convert a single float value to an integer grid coordinate
    auto convertToGridCoord = [gridSize](float value) -> int {
        return static_cast<int>(std::floor(value / gridSize) * gridSize);
        };

    return sf::Vector2i(convertToGridCoord(floatCoord.x), convertToGridCoord(floatCoord.y));
}