#include "ChaosGame.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <algorithm>

const float PI = 3.14159265358979323846f;

ChaosGame::ChaosGame(int pixelWidth, int pixelHeight) {
    m_pixel_size = { pixelWidth, pixelHeight };
    m_sides = 0;
    m_ratio = 0.0f;
    m_lastVertexIndex = -1;
    m_pointsDrawn = 0;
    
    std::random_device rd;
    m_randGen.seed(rd());
    
    m_vArray.setPrimitiveType(sf::Points);

    setPolygon(3);
}

void ChaosGame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray);
}

float ChaosGame::getRatio(int sides) {
    switch (sides) {
        case 3:  return 0.5f;
        case 4:  return 0.5f;
        case 5:  return 0.618f;
        case 6:  return 2.0f / 3.0f;
        case 7:  return 0.692f;
        case 8:  return 0.707f;
        case 9:  return 0.742f;
        case 10: return 0.764f;
        default: return 0.5f;
    }
}

void ChaosGame::generateVertices(int sides) {
    m_vertices.clear();
    if (sides < 3) return;

    float centerX = m_pixel_size.x / 2.0f;
    float centerY = m_pixel_size.y / 2.0f;
    float radius = std::min(centerX, centerY) * 0.9f; 

    float angleIncrement = 2.0f * PI / sides;

    float initialAngle = PI / 2.0f; 

    for (int i = 0; i < sides; ++i) {
        float angle = initialAngle + i * angleIncrement;
        float x = centerX + radius * std::cos(angle);
        float y = centerY - radius * std::sin(angle);
        m_vertices.push_back({x, y});
    }
}

void ChaosGame::setPolygon(int sides) {
    if (sides < 3 || sides > 10) return;
    
    m_sides = sides;
    m_ratio = getRatio(sides);
    generateVertices(sides);
    
    m_vArray.clear();
    m_pointsDrawn = 0;
    m_lastVertexIndex = -1; 
    
    m_currentPoint = {0.0f, 0.0f};
    for(const auto& v : m_vertices) {
        m_currentPoint.x += v.x;
        m_currentPoint.y += v.y;
    }
    m_currentPoint.x /= sides;
    m_currentPoint.y /= sides;
    
    for(const auto& v : m_vertices) {
        sf::Vertex vertex;
        vertex.position = v;
        vertex.color = sf::Color::Red; 
        m_vArray.append(vertex);
    }
    
    sf::Vertex initialPoint;
    initialPoint.position = m_currentPoint;
    initialPoint.color = sf::Color::Blue; 
    m_vArray.append(initialPoint);
    m_pointsDrawn++;
}

void ChaosGame::runIteration() {
    if (m_sides < 3) return;

    std::uniform_int_distribution<> distrib(0, m_sides - 1);
    int chosenIndex = -1;

    if (m_sides >= 4) {
        do {
            chosenIndex = distrib(m_randGen);
        } while (chosenIndex == m_lastVertexIndex);
    } else {
        chosenIndex = distrib(m_randGen);
    }
    
    m_lastVertexIndex = chosenIndex;
    sf::Vector2f chosenVertex = m_vertices[chosenIndex];

    
    sf::Vector2f vectorToVertex = chosenVertex - m_currentPoint;
    sf::Vector2f moveVector = m_ratio * vectorToVertex;
    sf::Vector2f newPoint = m_currentPoint + moveVector;

    m_currentPoint = newPoint;

    sf::Vertex newVertex;
    newVertex.position = m_currentPoint;
    newVertex.color = sf::Color::Green;
    m_vArray.append(newVertex);
    m_pointsDrawn++;
}

void ChaosGame::updateRenderer() {
    for (unsigned int i = 0; i < ITERATIONS_PER_FRAME; ++i) {
        runIteration();
    }
}

void ChaosGame::loadText(sf::Text& text) {
    std::ostringstream txt;

    txt << "Chaos Game Fractal Generator\n";
    txt << "Polygon: " << m_sides << " sides\n";
    txt << "Ratio (r): " << m_ratio << "\n";
    txt << "Total Points Drawn: " << m_pointsDrawn << "\n";
    txt << "Press 3, 4, 5, 6, 7, 8, 9, 0 (for 10) to change polygon\n";
    txt << "Rule: No repeating vertex for sides >= 4";

    text.setString(txt.str());
}
