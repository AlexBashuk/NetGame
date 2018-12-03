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
vector <sf::RectangleShape> walls;
/*sf::Image image;
sf::Texture texture;
sf::Sprite sprite;*/

void set_walls_points(pair <double, double> *a, const sf::RectangleShape &rect)
{
    double x = rect.getPosition().x;
    double y = rect.getPosition().y;
    double w = rect.getSize().x;
    double h = rect.getSize().y;

    a[0] = mp(x, y);
    a[1] = mp(x + w, y);
    a[2] = mp(x + w, y + h);
    a[3] = mp(x, y + h);
}



double turn(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
}

bool check_rect_and_point(pair <double, double> *a, double x, double y)
{
    double t = 0;

    for(int i = 0; i < 4; i++)
    {
        double xr1 = a[i].X;
        double yr1 = a[i].Y;
        double xr2 = a[(i + 1) % 4].X;
        double yr2 = a[(i + 1) % 4].Y;

        if(t == 0)
        {
            if(abs(turn(xr1, yr1, x, y, xr2, yr2)) > 1e-9)
                t = turn(xr1, yr1, x, y, xr2, yr2) / abs(turn(xr1, yr1, x, y, xr2, yr2));
        }
        else
        {
            if(abs(turn(xr1, yr1, x, y, xr2, yr2)) > 1e-9)
            {
                double t0 = turn(xr1, yr1, x, y, xr2, yr2) / abs(turn(xr1, yr1, x, y, xr2, yr2));

                if(abs(t - t0) > 1e-9)
                    return false;
            }
        }
    }

    return true;
}

bool check_two_rect(pair <double, double> *a, pair <double, double> *b)
{
    for(int i = 0; i < 4; i++)
    {
        double xr = b[i].X;
        double yr = b[i].Y;

        if(check_rect_and_point(a, xr, yr))
            return true;
    }

    return false;
}



class Bullet
{
private:
    const double move_speed = 0.7;

    int type;

    double elp;
    double x;
    double y;
    double a = 0;
    double b = 1;

    //sf::Texture texture;
    sf::Clock Clock;
    sf::Sprite sprite;

public:
    bool exist = true;

    sf::RectangleShape body;

private:
    void elp_t()
    {
        elp = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();
    }

    void move(int way)
    {
        //x += way * a * move_speed * elp;
        //y += way * b * move_speed * elp;
        sprite.move(way * a * move_speed * elp, way * b * move_speed * elp);
        body.move(way * a * move_speed * elp, way * b * move_speed * elp);
        x = sprite.getPosition().x;
        y = sprite.getPosition().y;
    }

    void rotate0()
    {
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;

        b = -b;
        sprite.rotate(-360 * ang1 / M_PI);
        body.rotate(-360 * ang1 / M_PI);
    }

    void rotate1()
    {
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;

        a = -a;
        sprite.rotate(180 - 360 * ang1 / M_PI);
        body.rotate(180 - 360 * ang1 / M_PI);
    }

    void action0()
    {
        move(1);

        if(check_walls())
            exist = false;
    }

    void action1()
    {
        move(1);

        if(!check_walls())
            return;

        //cout << x << " " << y << " " << sprite.getPosition().x << " " << sprite.getPosition().y << endl;

        move(-1);
        rotate0();
        move(1);

        //cout << x << " " << y << " " << sprite.getPosition().x << " " << sprite.getPosition().y << endl;

        if(!check_walls())
        {
            //move(-0);
            return;
        }

        move(-1);
        rotate0();
        rotate1();
        move(1);

        if(!check_walls())
        {
            //move(-0);
            return;
        }

        move(-1);
        rotate0();
        move(1);

        if(check_walls())
            exist = false;
        //move(-0);
    }

public:

    Bullet()
    {
        //texture.loadFromFile("Bullets.png");
        sprite.setTexture(texture_bullet_1);
        sprite.setTextureRect(sf::IntRect(81, 125, 36, 40));//81, 116, 123, 163
        sprite.setPosition(300, 600);
        sprite.setOrigin(18, 20);
        sprite.scale(1, 1);
        body.setSize(sf::Vector2f(18, 20));
        body.setPosition(300, 600);
        body.setOrigin(9, 10);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + sprite.getOrigin().x;
        y = sprite.getPosition().y;// + sprite.getOrigin().y;
        //cout << x << " " << y << endl;
        type = 0;
    }

    Bullet(int type0, double x0, double y0, double a0, double b0)
    {
        type = type0;
        //texture.loadFromFile("Bullets.png");
        if(type == 0 || type == 1)
            sprite.setTexture(texture_bullet_1);
        else if(type == 2)
            sprite.setTexture(texture_bullet_1);
        sprite.setTextureRect(sf::IntRect(81, 125, 36, 40));//81, 116, 123, 163
        sprite.setPosition(x0, y0);
        sprite.setOrigin(18, 20);
        sprite.scale(1, 1);
        body.setSize(sf::Vector2f(18, 20));
        body.setPosition(x0, y0);
        body.setOrigin(9, 10);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + sprite.getOrigin().x;
        y = sprite.getPosition().y;// + sprite.getOrigin().y;
        //cout << x << " " << y << endl;

        a = a0;
        b = b0;
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;
        sprite.setRotation(180 * ang1 / M_PI - 90);
        body.setRotation(180 * ang1 / M_PI - 90);
    }

    void set_points(pair <double, double> *arr)
    {
        double w = body.getSize().x * body.getScale().x;
        double h = body.getSize().y * body.getScale().y;

        arr[0] = mp(x + a * h / 2 + b * w / 2, y + b * h / 2 - a * w / 2);
        arr[1] = mp(x + a * h / 2 - b * w / 2, y + b * h / 2 + a * w / 2);
        arr[2] = mp(x - a * h / 2 - b * w / 2, y - b * h / 2 + a * w / 2);
        arr[3] = mp(x - a * h / 2 + b * w / 2, y - b * h / 2 - a * w / 2);
    }

    void draw()
    {
        window.draw(sprite);
    }

    bool check_walls()
    {
        pair <double, double> arr1[4];
        pair <double, double> arr2[4];
        set_points(arr1);

        //sf::FloatRect bound = sprite.getGlobalBounds();
        //sf::FloatRect bound = body.getGlobalBounds();

        for(int i = 0; i < walls.size(); i++)
        {
            //sf::FloatRect wallbound = walls[i].getGlobalBounds();
            //if(bound.intersects(wallbound))
            set_walls_points(arr2, walls[i]);
            if(check_two_rect(arr1, arr2))
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

            /*if(type == 0)
                action0();
            else if(type == 1)
                action1();*/

            //if(abs(x - sprite.getPosition().x) >= 1e-3 || abs(y - sprite.getPosition().y) >= 1e-3)
              //  cout << x << " " << y << " " << sprite.getPosition().x << " " << sprite.getPosition().y << endl;
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
    sf::Clock Clock;
    sf::Sprite sprite;

    void elp_t()
    {
        elp = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();
    }

public:
    sf::RectangleShape body;

    Tank()
    {
        //texture.loadFromFile("Tank.png");
        sprite.setTexture(texture_tank_1);
        sprite.setPosition(300, 600);
        sprite.setOrigin(256, 256);
        sprite.scale(0.2, 0.2);
        body.setSize(sf::Vector2f(512, 512));
        body.setPosition(300, 600);
        body.setOrigin(256, 256);
        body.scale(0.2, 0.2);
        body.setFillColor(sf::Color::Green);
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
        sprite.setOrigin(256, 256);
        sprite.scale(0.2, 0.2);
        body.setSize(sf::Vector2f(512, 512));
        body.setPosition(300, 600);
        body.setOrigin(256, 256);
        body.scale(0.2, 0.2);
        body.setFillColor(sf::Color::Green);
        x = sprite.getPosition().x;// + (sprite.getOrigin().x)/5;
        y = sprite.getPosition().y;// + (sprite.getOrigin().y)/5;
        a = a0;
        b = b0;
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;
        sprite.setRotation(180 * ang1 / M_PI - 90);
        body.setRotation(180 * ang1 / M_PI - 90);
    }

    void set_points(pair <double, double> *arr)
    {
        double w = body.getSize().x * body.getScale().x;
        double h = body.getSize().y * body.getScale().y;

        arr[0] = mp(x + a * h / 2 + b * w / 2, y + b * h / 2 - a * w / 2);
        arr[1] = mp(x + a * h / 2 - b * w / 2, y + b * h / 2 + a * w / 2);
        arr[2] = mp(x - a * h / 2 - b * w / 2, y - b * h / 2 + a * w / 2);
        arr[3] = mp(x - a * h / 2 + b * w / 2, y - b * h / 2 - a * w / 2);
    }

    void draw()
    {
        window.draw(sprite);
        window.draw(body);
    }

    bool check_walls()
    {
        pair <double, double> arr1[4];
        pair <double, double> arr2[4];
        set_points(arr1);

        //sf::FloatRect spritebound = sprite.getGlobalBounds();

        for(int i = 0; i < walls.size(); i++)
        {
            //sf::FloatRect wallbound = walls[i].getGlobalBounds();
            //if(spritebound.intersects(wallbound))
            set_walls_points(arr2, walls[i]);
            if(check_two_rect(arr1, arr2))
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
        body.rotate(-rotate_speed * elp);
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
        body.rotate(rotate_speed * elp);
    }

    /*void move_forward()
    {
        x += a * move_speed * elp;
        y += b * move_speed * elp;
        sprite.move(a * move_speed * elp, b * move_speed * elp);
        body.move(a * move_speed * elp, b * move_speed * elp);
    }

    void move_backward()
    {
        x -= a * move_speed * elp;
        y -= b * move_speed * elp;
        sprite.move(-a * move_speed * elp, -b * move_speed * elp);
        body.move(-a * move_speed * elp, -b * move_speed * elp);
    }*/

    void move(int way)
    {
        //x += way * a * move_speed * elp;
        //y += way * b * move_speed * elp;
        sprite.move(way * a * move_speed * elp, way * b * move_speed * elp);
        body.move(way * a * move_speed * elp, way * b * move_speed * elp);
        x = sprite.getPosition().x;
        y = sprite.getPosition().y;
    }

    void fire()
    {
        if(clock() - time >= change_time)
        {
            //cout << "Hi\n";

            double b_x, b_y, l;
            l = body.getSize().y * body.getScale().y/2 + 50;
            b_x = x + a * l;
            b_y = y + b * l;

            bullets.push_back(Bullet(1, b_x, b_y, a, b));
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
            move(1);//move_forward();
            if(check_walls())
                move(-1);//move_backward();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            move(-1);//move_backward();
            if(check_walls())
                move(1);//move_forward();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            fire();
    }
};

void hit_check(Tank &tank, Bullet &bullet)
{
    //sf::FloatRect tankbound = tank.sprite.getGlobalBounds();
    //sf::FloatRect bulletbound = bullet.sprite.getGlobalBounds();
    //sf::FloatRect bulletbound = bullet.body.getGlobalBounds();

    //if(tankbound.intersects(bulletbound))

    pair <double, double> a[4];
    pair <double, double> b[4];
    tank.set_points(a);
    bullet.set_points(b);

    if(check_two_rect(a, b))
    {
        bullet.exist = false;
    }
}

int main()
{
    freopen("input.in", "r", stdin);
    //freopen("output.out", "w", stdout);
    //freopen("input.in", "w", stdout);

    texture_bullet_1.loadFromFile("Bullets.png");
    texture_tank_1.loadFromFile("Tank.png");

    circle.setPosition(960, 600);
    circle.setFillColor(sf::Color(200, 150, 100));
    circle.setOrigin(100, 100);

    /*walls[0].setPosition(0, 0);
    walls[1].setPosition(0, 0);
    walls[2].setPosition(0, height - 5);
    walls[3].setPosition(width - 5, 0);
    walls[0].setSize(sf::Vector2f(width, 5));
    walls[1].setSize(sf::Vector2f(5, height));
    walls[2].setSize(sf::Vector2f(width, 5));
    walls[3].setSize(sf::Vector2f(5, height));
    for(int i = 0; i < 4; i++)
        walls[i].setFillColor(sf::Color::Black);*/

    int n;
    cin >> n;
    walls.resize(n);
    for(int i = 0; i < n; i++)
    {
        int a, b, w, h;
        cin >> a >> b >> w >> h;
        walls[i].setPosition(a, b);
        walls[i].setSize(sf::Vector2f(w, h));
        walls[i].setFillColor(sf::Color::Black);
    }

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
            if((cur_it->exist) == true)
                hit_check(tank, *cur_it);
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
        for(int i = 0; i < n; i++)
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














