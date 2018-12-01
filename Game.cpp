/* GCC */
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <queue>
#include <sstream>

#define X first
#define Y second
#define mp(a, b) make_pair((a), (b))
#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

using namespace std;

sf::Texture texture_bullet_1;
sf::Texture texture_tank_1;

const int width = 1920;
const int height = 1200;

sf::RenderWindow window(sf::VideoMode(width, height), "Tanks");
sf::CircleShape circle(100);
vector <sf::RectangleShape> walls(4);
/*sf::Image image;
sf::Texture texture;
sf::Sprite sprite;*/

class Bullet
{
private:
    const double move_speed = 0.9;

    double elp;
    double x;
    double y;
    double a = 0;
    double b = 1;

    //sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock Clock;

    void elp_t()
    {
        elp = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();
    }

public:
    bool exist = true;

    Bullet()
    {
        //texture.loadFromFile("Bullets.png");
        sprite.setTexture(texture_bullet_1);
        sprite.setTextureRect(sf::IntRect(81, 123, 35, 40));//81, 116, 123, 163
        sprite.setPosition(300, 600);
        sprite.setOrigin(17, 19);
        sprite.scale(1, 1);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + sprite.getOrigin().x;
        y = sprite.getPosition().y;// + sprite.getOrigin().y;
        //cout << x << " " << y << endl;
    }

    Bullet(int type, double x0, double y0, double a0, double b0)
    {
        //texture.loadFromFile("Bullets.png");
        if(type == 0)
            sprite.setTexture(texture_bullet_1);
        sprite.setTextureRect(sf::IntRect(81, 123, 35, 40));//81, 116, 123, 163
        sprite.setPosition(x0, y0);
        sprite.setOrigin(17, 19);
        sprite.scale(1, 1);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + sprite.getOrigin().x;
        y = sprite.getPosition().y;// + sprite.getOrigin().y;
        //cout << x << " " << y << endl;

        double ang1 = acos(a0);
        double ang2 = asin(b0);
        if(ang2 < 0)
            ang1 = -ang1;
        sprite.setRotation(180 * ang1 / M_PI - 90);

        a = a0;
        b = b0;
    }

    void draw()
    {
        window.draw(sprite);
    }

    bool check_walls()
    {
        sf::FloatRect spritebound = sprite.getGlobalBounds();

        for(int i = 0; i < walls.size(); i++)
        {
            sf::FloatRect wallbound = walls[i].getGlobalBounds();
            if(spritebound.intersects(wallbound))
            {
                //cout << "Bye\n";
                return true;
            }
        }

        return false;
    }

    void action()
    {
        if(exist)
        {
            elp_t();

            x += a * move_speed * elp;
            y += b * move_speed * elp;
            sprite.move(a * move_speed * elp, b * move_speed * elp);

            if(check_walls())
                exist = false;
        }
    }
};
list <Bullet> bullets;

class Tank
{
private:
    const double rotate_speed = 0.4;
    const double move_speed = 0.6;

    int time = 0;
    int change_time = 100;

    double elp;
    double x;
    double y;
    double a = 0;
    double b = 1;

    //sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock Clock;

    void elp_t()
    {
        elp = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();
    }

public:
    Tank()
    {
        //texture.loadFromFile("Tank.png");
        sprite.setTexture(texture_tank_1);
        sprite.setPosition(300, 600);
        sprite.setOrigin(255, 255);
        sprite.scale(0.2, 0.2);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + (sprite.getOrigin().x)/1;
        y = sprite.getPosition().y;// + (sprite.getOrigin().y)/1;
        //cout << x << " " << y << endl;
    }

    Tank(int type, double x0, double y0, double a0, double b0)
    {
        /*if(type == 0)
            texture.loadFromFile("Tank.png");
        else if(type == 1)
            texture.loadFromFile("Tank2.png");*/
        sprite.setTexture(texture_tank_1);
        sprite.setPosition(x0 - 255, y0 - 255);
        sprite.setOrigin(255, 255);
        sprite.scale(0.2, 0.2);
        x = sprite.getPosition().x;// + (sprite.getOrigin().x)/5;
        y = sprite.getPosition().y;// + (sprite.getOrigin().y)/5;
        a = a0;
        b = b0;

    }

    void draw()
    {
        window.draw(sprite);
    }

    bool check_walls()
    {
        sf::FloatRect spritebound = sprite.getGlobalBounds();

        for(int i = 0; i < walls.size(); i++)
        {
            sf::FloatRect wallbound = walls[i].getGlobalBounds();
            if(spritebound.intersects(wallbound))
                return true;
        }

        return false;
    }

    void rotate_left()
    {
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;

        double angle = ang1 - (rotate_speed * elp/180) * M_PI;
        a = cos(angle);
        b = sin(angle);
        sprite.rotate(-rotate_speed * elp);
    }

    void rotate_right()
    {
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;

        double angle = ang1 + (rotate_speed * elp/180) * M_PI;
        a = cos(angle);
        b = sin(angle);
        sprite.rotate(rotate_speed * elp);
    }

    void move_forward()
    {
        x += a * move_speed * elp;
        y += b * move_speed * elp;
        sprite.move(a * move_speed * elp, b * move_speed * elp);
    }

    void move_backward()
    {
        x -= a * move_speed * elp;
        y -= b * move_speed * elp;
        sprite.move(-a * move_speed * elp, -b * move_speed * elp);
    }

    void fire()
    {
        if(clock() - time >= change_time)
        {
            //cout << "Hi\n";
            bullets.push_back(Bullet(0, x, y, a, b));
            time = clock();
        }
    }

    void action()
    {
        elp_t();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rotate_left();
            if(check_walls())
                rotate_right();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rotate_right();
            if(check_walls())
                rotate_left();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            move_forward();
            if(check_walls())
                move_backward();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            move_backward();
            if(check_walls())
                move_forward();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            fire();
    }
};

int main()
{
    //freopen("input.in", "r", stdin);
    //freopen("output.out", "w", stdout);
    //freopen("input.in", "w", stdout);

    texture_bullet_1.loadFromFile("Bullets.png");
    texture_tank_1.loadFromFile("Tank.png");

    circle.setPosition(900, 600);
    circle.setFillColor(sf::Color(200, 150, 100));
    circle.setOrigin(100, 100);

    walls[0].setPosition(0, 0);
    walls[1].setPosition(0, 0);
    walls[2].setPosition(0, height - 5);
    walls[3].setPosition(width - 5, 0);
    walls[0].setSize(sf::Vector2f(width, 5));
    walls[1].setSize(sf::Vector2f(5, height));
    walls[2].setSize(sf::Vector2f(width, 5));
    walls[3].setSize(sf::Vector2f(5, height));
    for(int i = 0; i < 4; i++)
        walls[i].setFillColor(sf::Color::Black);

    /*image.loadFromFile("Tank.png");
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(300, 600);
    sprite.setOrigin(255, 255);
    sprite.scale(0.2, 0.2);*/

    //cout << cos(45) << " " << cos(M_PI/)

    Tank tank;
    //Bullet bullet;

    while(window.isOpen())
    {

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            //if(event.type == sf::Event::Resized)
              //  window.sbool exist = true;etSize(event.size.width, event.size.height);
        }

        tank.action();
        //bullet.action();
        //if(bullets.begin() != bullets.end())
          //  cout << "Hi\n";
        for(auto it = bullets.begin(); it != bullets.end();)
        {
            auto cur_it = it;
            it++;
            cur_it->action();
            if((cur_it->exist) == false)
            {
                //cout << "Hi\n";
                bullets.erase(cur_it);
            }
        }

        /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            //sprite.move(-1, 0);
            //sprite.rotate(1);
            tank.rotate_left();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            //sprite.move(1, 0);
            //sprite.rotate(-1);
            tank.rotate_right();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            //sprite.move(0, -1);
            tank.move_forward();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            //sprite.move(0, 1);
            tank.move_backward();
        }*/

        window.clear(sf::Color::White);
        window.draw(circle);
        for(int i = 0; i < 4; i++)
            window.draw(walls[i]);
        for(auto it = bullets.begin(); it != bullets.end(); it++)
            it->draw();
        //window.draw(sprite);
        tank.draw();
        //bullet.draw();
        window.display();
    }

    return 0;
}














