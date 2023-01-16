#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

static constexpr std::size_t window_size = 800;

float CalculateX(int i, int j, int k, float theta) {
    return j * sin(theta) * sin(theta) * cos(theta) - k * cos(theta) * sin(theta) * cos(theta) +
           j * cos(theta) * sin(theta) + k * sin(theta) * sin(theta) + i * cos(theta) * cos(theta);
}

float CalculateY(int i, int j, int k, float theta) {
    return j * cos(theta) * cos(theta) + k * sin(theta) * cos(theta) - j * sin(theta) * sin(theta) * sin(theta) +
           k * cos(theta) * sin(theta) * sin(theta) - i * cos(theta) * sin(theta);
}

float CalculateZ(int i, int j, int k, float theta) {
    return k * cos(theta) * cos(theta) - j * sin(theta) * cos(theta) + i * sin(theta);
}

void DrawSides(sf::RenderWindow& window, std::vector<std::array<int, 3>> vector) {
    std::vector<std::array<int, 3>> sides;
    sides.push_back({0, 1, 2});
    sides.push_back({0, 2, 3});
    sides.push_back({0, 3, 4});
    sides.push_back({0, 4, 1});

    auto sort_impl = [&vector](std::array<int, 3> a, std::array<int, 3> b) {
        return (vector[a[0]][2] + vector[a[1]][2] + vector[a[2]][2] >
                vector[b[0]][2] + vector[b[1]][2] + vector[b[2]][2]);
    };

    std::sort(sides.begin(), sides.end(), sort_impl);

    sf::VertexArray triangle(sf::Triangles, 3);

    for (auto side : sides) {
        triangle[0].position = sf::Vector2f(vector[side[0]][0], vector[side[0]][1]);
        triangle[1].position = sf::Vector2f(vector[side[1]][0], vector[side[1]][1]);
        triangle[2].position = sf::Vector2f(vector[side[2]][0], vector[side[2]][1]);

        triangle[0].color = sf::Color(210, 100, 100);
        triangle[1].color = sf::Color(0, 0, 0);
        triangle[2].color = sf::Color(0, 0, 0);

        window.draw(triangle);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Spinning_triangle", sf::Style::Close);
    window.setFramerateLimit(61);

    std::vector<std::array<float, 3>> triangle_points;
    triangle_points.push_back({0, 1 * static_cast<float>(std::sqrt(3)) * 2 / 3, 0});
    triangle_points.push_back({1, -1 * static_cast<float>(std::sqrt(3)) / 3, -1});
    triangle_points.push_back({-1, -1 * static_cast<float>(std::sqrt(3)) / 3, -1});
    triangle_points.push_back({-1, -1 * static_cast<float>(std::sqrt(3)) / 3, 1});
    triangle_points.push_back({1, -1 * static_cast<float>(std::sqrt(3)) / 3, 1});

    std::vector<std::array<int, 3>> points;

    sf::VertexArray lines(sf::LinesStrip, 2);

    float theta = 0.0F;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();

        theta = (std::fmod(theta, (3.14159 * 2))) + 0.02;

        for (auto point : triangle_points) {
            std::array<float, 3> rotated = {CalculateX(point[0], point[1], point[2], theta),
                                            CalculateY(point[0], point[1], point[2], theta),
                                            CalculateZ(point[0], point[1], point[2], theta)};

            int x = (rotated[0]) * 100 + 400;
            int y = (rotated[1]) * 100 + 400;
            int z = (rotated[2]) * 100 + 400;

            points.push_back({x, y, z});
        }

        DrawSides(window, points);

        points.clear();
        window.display();
    }
}
