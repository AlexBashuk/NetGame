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

sf::Texture texture_bullets_1;
sf::Texture texture_tank[2];
sf::Texture texture_explode;

const int width = 1920;
const int height = 1200;

//sf::RenderWindow window(sf::VideoMode(width, height), "Tanks");
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

    for(int i = 0; i < 4; i++)
    {
        double xr = a[i].X;
        double yr = a[i].Y;

        if(check_rect_and_point(b, xr, yr))
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

    int damage;

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
        sprite.setTexture(texture_bullets_1);
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
        damage = 5;
    }

    Bullet(int type0, double x0, double y0, double a0, double b0)
    {
        type = type0;
        //texture.loadFromFile("Bullets.png");
        sprite.setTexture(texture_bullets_1);
        if(type == 0)
        {
            //81, 125, 117, 165
            damage = 5;
            sprite.setTextureRect(sf::IntRect(81, 125, 36, 40));
        }
        else if(type == 1)
        {
            //16, 125, 52, 165
            damage = 5;
            sprite.setTextureRect(sf::IntRect(16, 125, 36, 40));
        }
        else if(type == 2)
        {
            damage = 100;
            sprite.setTextureRect(sf::IntRect(81, 125, 36, 40));
        }

        //sprite.setTextureRect(sf::IntRect(81, 125, 36, 40));
        body.setSize(sf::Vector2f(18, 20));
        sprite.setPosition(x0, y0);
        body.setPosition(x0, y0);
        sprite.setOrigin(18, 20);
        body.setOrigin(9, 10);
        sprite.scale(1, 1);
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
        //window.draw(sprite);
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

            if(type == -1)
                action0();
            else if(0 <= type && type < 4)
                action1();

            //if(abs(x - sprite.getPosition().x) >= 1e-3 || abs(y - sprite.getPosition().y) >= 1e-3)
              //  cout << x << " " << y << " " << sprite.getPosition().x << " " << sprite.getPosition().y << endl;
        }
    }

    string data()
    {
        string data_ = "B;";
        data_ += to_string(type);
        data_ += ';';
        data_ += to_string(x);
        data_ += ';';
        data_ += to_string(y);
        data_ += ';';
        data_ += to_string(a);
        data_ += ';';
        data_ += to_string(b);
        data_ += ';';

        return data_;
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

    int hp = 100;
    int num, type;

    double elp;
    double x;
    double y;
    double a = 0;
    double b = 1;

    //sf::Texture texture;
    sf::Clock Clock;
    sf::Sprite sprite;
    sf::RectangleShape cur_hp, max_hp;

    void elp_t()
    {
        elp = Clock.getElapsedTime().asMilliseconds();
        elp = int(1e5 * elp)/1e5;
        Clock.restart();
    }

public:
    bool exist = true;

    sf::RectangleShape body;

    Tank()
    {
        //texture.loadFromFile("Tank.png");
        num = type = 0;
        sprite.setTexture(texture_tank[0]);
        sprite.setPosition(300, 600);
        sprite.setOrigin(256, 256);
        sprite.scale(0.1, 0.1);
        body.setSize(sf::Vector2f(512, 512));
        body.setPosition(300, 600);
        body.setOrigin(256, 256);
        body.scale(0.1, 0.1);
        body.setFillColor(sf::Color::Green);
        cur_hp.setPosition(30, 1085);
        cur_hp.setSize(sf::Vector2f(500, 30));
        cur_hp.setFillColor(sf::Color::Green);
        max_hp.setPosition(30, 1085);
        max_hp.setSize(sf::Vector2f(500, 30));
        max_hp.setFillColor(sf::Color::Red);
        //max_hp.setSize(sf::Vector2f(512 * 0.1, 512 * 0.1);
        //sprite.setColor(sf::Color::Green);
        x = sprite.getPosition().x;// + (sprite.getOrigin().x)/1;
        y = sprite.getPosition().y;// + (sprite.getOrigin().y)/1;
        //cout << x << " " << y << endl;
    }

    Tank(int num0, int type0, double x0, double y0, double a0, double b0, int hp0)
    {
        /*if(type == 0)
            texture.loadFromFile("Tank.png");
        else if(type == 1)
            texture.loadFromFile("Tank2.png");*/
        num = num0;
        type = type0;
        hp = hp0;
        sprite.setTexture(texture_tank[type]);
        body.setSize(sf::Vector2f(512, 512));
        sprite.setOrigin(256, 256);
        body.setOrigin(256, 256);
        sprite.setPosition(x0, y0);
        body.setPosition(x0, y0);
        sprite.scale(0.1, 0.1);
        body.scale(0.1, 0.1);
        body.setFillColor(sf::Color::Green);

        cur_hp.setPosition(30 + type * 960, 1085);
        max_hp.setPosition(30 + type * 960, 1085);
        cur_hp.setSize(sf::Vector2f(500, 30));
        max_hp.setSize(sf::Vector2f(500, 30));
        cur_hp.setFillColor(sf::Color::Green);
        max_hp.setFillColor(sf::Color::Red);

        //x = sprite.getPosition().x;// + (sprite.getOrigin().x)/5;
        //y = sprite.getPosition().y;// + (sprite.getOrigin().y)/5;
        x = x0;
        y = y0;
        a = a0;
        b = b0;
        double ang1 = acos(a);
        double ang2 = asin(b);
        if(ang2 < 0)
            ang1 = -ang1;
        sprite.setRotation(180 * ang1 / M_PI - 90);
        body.setRotation(180 * ang1 / M_PI - 90);
    }

    void hit(int damage)
    {
        hp -= damage;
        cur_hp.setScale(hp/100.0, 1);
        if(hp <= 0)
        {
            sprite.setTexture(texture_explode);
            exist = false;
        }
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
        //window.draw(sprite);
        //window.draw(max_hp);
        //window.draw(cur_hp);
        //window.draw(body);
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
            l = body.getSize().y * body.getScale().y/2 + 15;
            b_x = x + a * l;
            b_y = y + b * l;

            bullets.push_back(Bullet(type, b_x, b_y, a, b));
            time = clock();
        }
    }

    void action(char *s)
    {
        elp_t();

        /*elp = 0;
        for(int i = 1; i <= 4; i++)
            elp = (elp * 256) + s[i];
        elp = elp / 1e5;*/

        if(s[1] == '1')
        {
            rotate_left();
            if(check_walls())
                rotate_right();
        }
        if(s[2] == '1')
        {
            rotate_right();
            if(check_walls())
                rotate_left();
        }
        if(s[3] == '1')
        {
            move(1);//move_forward();
            if(check_walls())
                move(-1);//move_backward();
        }
        if(s[4] == '1')
        {
            move(-1);//move_backward();
            if(check_walls())
                move(1);//move_forward();
        }
        if(s[5] == '1')
            fire();
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

    string data()
    {
        string data_ = "T;";
        data_ += to_string(num);
        data_ += ';';
        data_ += to_string(type);
        data_ += ';';
        data_ += to_string(x);
        data_ += ';';
        data_ += to_string(y);
        data_ += ';';
        data_ += to_string(a);
        data_ += ';';
        data_ += to_string(b);
        data_ += ';';
        data_ += to_string(hp);
        data_ += ';';

        return data_;
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
        tank.hit(bullet.damage);
        bullet.exist = false;
    }
}

int main()
{
    freopen("input.in", "r", stdin);
    //freopen("output.out", "w", stdout);
    //freopen("input.in", "w", stdout);

    texture_bullets_1.loadFromFile("Bullets.png");
    texture_tank[0].loadFromFile("Tank1.png");
    texture_tank[1].loadFromFile("Tank2.png");
    texture_explode.loadFromFile("Explode.png");

    circle.setPosition(960, 500);
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

    //Tank tank;
    int tanks_num = 0;
    vector <Tank> tanks;
    //tanks.push_back(Tank(0, 0, 300, 600, 0, 1));
    //tanks.push_back(Tank(1, 1, 1220, 600, 0, 1));
    //Bullet bullet;

    sf::TcpListener listener;

    // bind the listener to a port
    if (listener.listen(53000) != sf::Socket::Done)
    {
        cout << "Listen Error\n";
    }

    while(true)
    {
        /*sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            //if(event.type == sf::Event::Resized)
              //  window.sbool exist = true;etSize(event.size.width, event.size.height);
        }*/

        // accept a new connection
        sf::TcpSocket socket;
        if (listener.accept(socket) != sf::Socket::Done)
        {
            cout << "Accept Error\n";
            break;
        }

        size_t bytesRead = 0;
        char buffer [100010] = {0};
        sf::Socket::Status status = socket.receive(buffer, 100010, bytesRead);
        if(status != sf::Socket::Done)
        {
            std::cout << "Error rececive!" << endl;
            continue;
        }
        else
        {
            if(buffer[0] == -1)
            {
                cout << "New tank\n";
                tanks.push_back(Tank(tanks_num, tanks_num, 300, 600, 0, 1, 100));
                string data;
                data += char(tanks_num); //to_string(tanks_num);
                socket.send(data.c_str(), data.length());
                buffer[0] = tanks_num;
                tanks_num++;
                cout << "New tank added\n";

                size_t bytesRead_ = 0;
                char buffer_ [100010] = {0};
                sf::Socket::Status status = socket.receive(buffer_, 100010, bytesRead_);
                if(status != sf::Socket::Done)
                {
                    std::cout << "Error rececive!" << endl;
                    continue;
                }
                else
                {
                    string check_str(buffer_);
                    if(check_str != "Number got")
                        cout << "It's bad\n";
                }
            }
        }
        //string s_buffer(buffer);
        //if(s_buffer.size() >= 6 && s_buffer != "000000")
            //cout << s_buffer << endl;
        tanks[buffer[0]].action(buffer);

        /*for(int i = 0; i < tanks.size(); i++)
            if(tanks[i].exist == true)
                tanks[i].action();*/
        //bullet.action();
        //if(bullets.begin() != bullets.end())
          //  cout << "Hi\n";
        for(auto it = bullets.begin(); it != bullets.end();)
        {
            auto cur_it = it;
            it++;
            cur_it->action();
            for(int i = 0; i < tanks.size(); i++)
                if((cur_it->exist) == true && tanks[i].exist == true)
                    hit_check(tanks[i], *cur_it);
            if((cur_it->exist) == false)
            {
                //cout << "Hi\n";
                bullets.erase(cur_it);
            }
        }

        string data;
        for(int i = 0; i < tanks.size(); i++)
            data += tanks[i].data();
        for(auto it = bullets.begin(); it != bullets.end(); it++)
            data += it->data();
        socket.send(data.c_str(), data.length());

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

        /*window.clear(sf::Color::White);
        window.draw(circle);
        for(int i = 0; i < n; i++)
            window.draw(walls[i]);
        for(int i = 0; i < tanks.size(); i++)
            tanks[i].draw();
        for(auto it = bullets.begin(); it != bullets.end(); it++)
            it->draw();
        //window.draw(sprite);
        //bullet.draw();
        window.display();*/
    }

    return 0;
}














