#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct Zombie {
	
	sf::Sprite zombieSprite;
	float speed;
	//this way ,we can reuse same texture
	Zombie(sf::Texture& tex):zombieSprite(tex){} //only need the reference of the texture
};

struct Bullet {
	//sf::CircleShape bulletShape;
	sf::Sprite bulletSprite;
	sf::Vector2f velocity;

	Bullet(sf::Texture& tex) :bulletSprite(tex) {}
};



class Game{

private:
	sf::RenderWindow window;
	sf::Clock clock;

	//texture
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	float playerSpeed;

	sf::Texture zombTexture;

	sf::Texture bulletTexture;

	sf::Texture gunTexture;
	sf::Sprite gunSprite;

	std::vector<Bullet> bullets;
	float bulletSpeed;

	std::vector<Zombie>zombies;
	sf::Clock spawnClock; // use to give time for zombie to spawn

	//deal with player damage
	int playerHealth;
	sf::Clock DamageClock;// use to give a damage at a time

	//gameClock(difficulty scaling)
	sf::Clock gameClock;
	float spawnInterval;


	//Font
	sf::Font gameFont;
	sf::Text gameText;
	bool isGameOver;

	//score
	int score;
	sf::Text scoreText;

	//health display
	sf::Text healthText;

	//Audio Soundbuffer->Sound sound
	sf::SoundBuffer buffer;
	sf::Sound gunSound;

	//sprite-sheet

	int currentRow;
	int currentFrame;//colum
	sf::Clock animClock;


	//process->update->render
	
	void processEvent();
	void update(float deltaTime);
	void render();

	void handleMovement(float deltaTime);
	void handleAiming();
	void shoot();

	void spawnZombie();
	void reset();//reset game 

	

	

public:
	Game(); //constructor
	void run();



};