#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>

const unsigned int MAX_ITER = 64;
const double BASE_WIDTH = 4.0;
const double BASE_HEIGHT = 4.0;
const double BASE_ZOOM = 0.5;

enum class State {CALCULATING, DISPLAYING};

class ComplexPlane : public sf::Drawable {
	public:
		ComplexPlane(int pixelWidth, int pixelHeight);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void zoomIn();
		void zoomOut();
		void setCenter(sf::Vector2i mousePixel);
		void setMouseLocation(sf::Vector2i mousePixel);
		void loadText(sf::Text& text);
		void updateRenderer();
	private:
		//functions
		int countIterations(Vector2f coords);
		void iterationsToRBG(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
		sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
		//variables
		sf::VertexArray m_vArray;
		sf::State m_state;
		sf::Vector2f m_mouseLocation;
		sf::Vector2i m_pixel_size;
		sf::Vector2f m_plane_center;
		sf::Vector2f m_plane_size;
		int m_zoomCount;
		double m_aspectRatio;;
}

#endif



