#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Zombie {
	sf::CircleShape zombShape;
	float speed;
};

struct Bullet {
	sf::CircleShape bulletShape;
	sf::Vector2f velocity;
};


class Game{

private:
	sf::RenderWindow window;
	sf::Clock clock;

	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	//sf::Texture playerTexture;
	float playerSpeed;

	std::vector<Bullet> bullets;
	float bulletSpeed;

	std::vector<Zombie>zombies;
	sf::Clock spawnClock; // use to give time for zombie to spawn

	//deal with player damage
	float playerHealth;
	sf::Clock DamageClock;// use to give a damage at a time


	//process->update->render
	
	void processEvent();
	void update(float deltaTime);
	void render();

	void handleMovement(float deltaTime);
	void handleAiming();
	void shoot();

	void spawnZombie();

public:
	Game(); //constructor
	void run();



};