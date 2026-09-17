#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

enum class GAMESTATE {
	intro,
	playing,
	gameover
};

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

	sf::Texture mapTexture;
	sf::Sprite mapSprite;

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
	

	sf::Text introText;

	//score
	int score;
	sf::Text scoreText;

	//health display
	sf::Text healthText;

	//Audio Soundbuffer->Sound sound
	sf::SoundBuffer buffer;
	sf::Sound gunSound;

	//Music soundTrack
	sf::Music introSoundTrack;
	sf::Music soundTrack;
	sf::Music gameOverSound;

	//sprite-sheet

	int currentRow;
	int currentFrame;//colum
	sf::Clock animClock;

	int mapRow;
	int mapCol;

	//grid for map
	std::vector<std::vector<int>>map;
	int tileSize = 32;

	//Camera view
	sf::View playerCam;
	sf::View uiCam;

	//GAMESTATE
	GAMESTATE currentState;


	//process->update->render
	
	void processEvent();
	void update(float deltaTime);
	void render();

	void handleMovement(float deltaTime);
	void handleAiming();
	void shoot();

	void spawnZombie();
	void reset();//reset game
	void drawMap();
	bool isWallat(sf::Vector2f pos);

	void changeState(GAMESTATE newState);
	

	

public:
	Game(); //constructor
	void run();



};