/* GCC */
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
//#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <sstream>

#define X first
#define Y second
#define mp(a, b) make_pair((a), (b))

using namespace std;

int main()
{
    //freopen("input.in", "r", stdin);
    //freopen("output.out", "w", stdout);
    //freopen("input.in", "w", stdout);

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("91.202.128.77", 80);
    if (status != sf::Socket::Done)
    {
        cout << "Error\n";
    }

    string data = "GET /\n\n";
    socket.send(data.c_str(), data.length());

    size_t bytesRead = 0;
    char buffer [10000] = {0};
    status = socket.receive(buffer, 10000, bytesRead);
    if(status != sf::Socket::Done)
    {
        std::cout << "Error rececive!" << endl;
    }
    else
    {
        cout << buffer;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}














