#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

enum class GAMESTATE {
	intro,
	playing,
	gameover
};

class Zombie {
protected:
	
	sf::Sprite zombSprite;
	float zombSpeed;
	int zombHealth;
	
public:
	//this way ,we can reuse same texture for different sprite
	Zombie(sf::Texture& tex, float speed, int health) :zombSprite(tex), zombSpeed(speed), zombHealth(health) {} //only need the reference of the texture

	virtual ~Zombie(){} //destructor

	sf::Sprite& getSprite(){ return zombSprite;}
	float getSpeed() const { return zombSpeed; }
	void takeDamage(int amount) { zombHealth -= amount;}
	bool isDead() const { return zombHealth <= 0; }

	virtual void updateAnimation(float deltaTime,float directionX){} //by default: this does nothing

	

	
};

class NormalZombie :public Zombie {
public:
	NormalZombie(sf::Texture& tex) :Zombie(tex,80.f, 1) {};

};

class FastZombie :public Zombie {
private:
	
	int currentRow; //38
	int currentFrame=0; //frame 8
	sf::Clock animClock;

public:
	FastZombie(sf::Texture& tex) :Zombie(tex, 100.f, 3) {};

	void updateAnimation(float deltaTime,float directionX) override;

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
	sf::Texture fastZombieTexture;

	sf::Texture bulletTexture;

	sf::Texture gunTexture;
	sf::Sprite gunSprite;

	sf::Texture mapTexture;
	sf::Sprite mapSprite;

	std::vector<Bullet> bullets;
	float bulletSpeed;

	std::vector<std::shared_ptr<Zombie>>zombies;
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