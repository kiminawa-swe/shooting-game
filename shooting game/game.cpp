#include "game.h"
#include <cmath>
#include < cstdlib >
#include<algorithm>//to use remove_if
#include <iostream>
//constructor

Game::Game() :window(sf::VideoMode({ 800, 600 }), "my game"),playerSprite(playerTexture),gameText(gameFont){

    if (!playerTexture.loadFromFile("Hunting_Rifle_12x.png")) {
        std::cout << "failed to load the image";
    }

    if (!gameFont.openFromFile("freedom-font/Freedom-10eM.ttf")) {
        std::cout << "failed to load ";
    }
    
   
    playerSprite.setTexture(playerTexture, true); //initially in constructor we use playerSprite(playerTexture),playerTexture is in 0x0, so then we need to refresh after loaded true texture

    srand(static_cast<unsigned>(time(0))); // to make random number spread more evenly

    playerSprite.setPosition({ 400.f, 300.f });
    playerSprite.setScale({ 0.25f, 0.25f });

    sf::FloatRect bound = playerSprite.getLocalBounds();
    playerSprite.setOrigin({ 400.f, (bound.size.y / 2)-80.f});

    playerSpeed = 250.f;
    bulletSpeed = 500.f;
    playerHealth = 100.f;//initiliaze player health

    //Font and text

    
    gameText.setCharacterSize(50);
    gameText.setString("GAME OVER");
    gameText.setFillColor(sf::Color::Red);
    gameText.setPosition({ 300.f,250.f });
    isGameOver = false;
    //
}

//process->update->render 
void Game::run() {

    while (window.isOpen()) {
    
        float deltaTime = clock.restart().asSeconds();
        processEvent();
        update(deltaTime);
        render();
    }
    



}

void Game::processEvent() {

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (event->is<sf::Event::MouseButtonPressed>()) {
            //handle shooting 
            shoot();


        }
    }


}

void Game::update(float deltaTime) {
    //base case
    if (isGameOver) { return; }

    handleMovement(deltaTime);
    handleAiming();

    for (auto& b : bullets) {
        b.bulletShape.move(b.velocity*deltaTime); //updated: times with deltaTime

    }

    //zombie will be spawn after 2 seconds, use: sf::Clock

    if (spawnClock.getElapsedTime().asSeconds() > 2.f) {
        spawnZombie();
        
        spawnClock.restart();
    }

    

    for (auto& z : zombies) {
        //zombies movement here
        sf::Vector2f zombPos = z.zombShape.getPosition();
        sf::Vector2f playerPos = playerSprite.getPosition();

        sf::Vector2f direction = playerPos - zombPos;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0.f) {
            direction /= length;

        }
       

        

        z.zombShape.move(z.speed * direction*deltaTime);

    }

    //we need to handle collision that occur (bullet collide with zombies)

    for (int i = 0;i < bullets.size();i++) {
        for (int j = 0;j < zombies.size();j++) {
            //get the position first

            sf::Vector2f bulletPos = bullets[i].bulletShape.getPosition();
            sf::Vector2f zombiePos = zombies[j].zombShape.getPosition();

            //to calculate distance

            float dx = bulletPos.x - zombiePos.x;
            float dy = bulletPos.y - zombiePos.y;

            float distance = sqrt(dx * dx + dy * dy);

            //find the sum of radius of zombie and bullet 
            float collisionRange = bullets[i].bulletShape.getRadius() + zombies[j].zombShape.getRadius();

            //if the distance < collision range, it mean collision occur

            if (distance < collisionRange) {
                bullets.erase(bullets.begin()+i);  //syntax: vec.begin() +i 
                zombies.erase(zombies.begin()+j);

                i--; //as the item in the vector will slide to left ,we move backward 

                break; // no more searching using the same bullet to others zombies



            }



        }
    }

    //clean-up of bullets throw away (not hitting target and fly out of nowhere)
    //using lambda , as declaring others function inside a function is prohibited
    auto isOffScreen = [](const Bullet& b) {


        sf::Vector2f pos = b.bulletShape.getPosition();

        return pos.x < 0 || pos.x>800 || pos.y < 0 || pos.y>600;

        };

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), isOffScreen), bullets.end());

    //player damage

    for (auto& d : zombies) {
        sf::Vector2f zombiePos = d.zombShape.getPosition();
        sf::Vector2f playerPos = playerSprite.getPosition();

        sf::Vector2f direction = playerPos - zombiePos;//?
        float dx = playerPos.x - zombiePos.x;
        float dy = playerPos.y - zombiePos.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        float collisionRange = d.zombShape.getRadius() + 20.f; 

        if (distance < collisionRange) {
            //collision with player occur , and damage given 1 at a time
            if (DamageClock.getElapsedTime().asSeconds() > 1.f) {
                playerHealth -= 10;
                DamageClock.restart();

                std::cout << "current player health-> " << playerHealth <<"\n";

                if (playerHealth <= 0) {
                    isGameOver = true;
                }
                


            }

        }
       

    }



    

}

void Game::render() {

    if (!isGameOver) {
    window.clear();
    
    
    for (auto& b : bullets) {
        window.draw(b.bulletShape);
    }

    for (auto& z : zombies) {
        window.draw(z.zombShape);
    }

    
    window.draw(playerSprite);

    }
    else {
        window.draw(gameText);
    }
    
    window.display();


}



//handling function

void Game::handleMovement(float deltaTime) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        playerSprite.move({ 0.f,-playerSpeed * deltaTime });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        playerSprite.move({ 0.f,playerSpeed * deltaTime });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        playerSprite.move({ playerSpeed * deltaTime,0.f });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        playerSprite.move({ -playerSpeed * deltaTime,0.f });

}

void Game::handleAiming() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f playerPos = playerSprite.getPosition();
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

    //angle 

    float dy = mouseWorldPos.y - playerPos.y;
    float dx = mouseWorldPos.x - playerPos.x;

    float angle = std::atan2(dy, dx) * 180.f / 3.14159f;

    //set angle to sprite
    playerSprite.setRotation(sf::degrees(angle));
}

void Game::shoot() {

    Bullet b;

    b.bulletShape.setFillColor(sf::Color::Yellow);
    b.bulletShape.setRadius(4.f);
    b.bulletShape.setPosition(playerSprite.getPosition());

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);
    sf::Vector2f direction = mousePos - playerSprite.getPosition();

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);//theorem

    if (length != 0.f) {
        direction /= length; //direction divide length to get exact value 
        //direction is the" target coordinate "
    }


    b.velocity = bulletSpeed * direction; //doesnt require delta time? line 65,

    bullets.push_back(b);



}


void Game::spawnZombie() {
    Zombie z;

    z.zombShape.setRadius(10.f);
    z.zombShape.setFillColor(sf::Color::Green);
    z.speed = 10.f;

    //creating random position of zombies
    // using edge , then we determine the position

    int edge = rand() % 4; // creating random edge from 0-3

    sf::Vector2f pos;

    //top bottom left right

    if (edge == 0) {
        pos = { (float)(rand() % 800),0.f }; //top
    }
    else if (edge == 1) {
        pos = { 800.f,(float)(rand() % 600) };//right
    }
    else if (edge == 2) { 
        pos ={ (float)(rand() % 800), 600.f }; //bottom

    }
    else if(edge==3) {
        pos = {0.f,(float)(rand()%600)}; //left
    }



    z.zombShape.setPosition(pos);

    zombies.push_back(z);
}