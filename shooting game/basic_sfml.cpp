#include <SFML/Graphics.hpp>

int main() {

	using namespace sf;
	using namespace std;
	RenderWindow window(VideoMode({ 800,600 }), "my game");

	while (window.isOpen()) {
		while (const optional event = window.pollEvent()) {
			if (event->is<Event::Closed>()) {
				window.close();
			}
		}
		window.display();
	}
}

//this the best practice to write sf:: instead of using (using namespace sf)


