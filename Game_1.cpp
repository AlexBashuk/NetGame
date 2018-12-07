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

    /*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);*/

    /*sf::Socket::Status status = socket.connect("127.0.0.0", 53000);
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
    }*/

    sf::TcpListener listener;

    // bind the listener to a port
    if (listener.listen(53000) != sf::Socket::Done)
    {
        cout << "Listen Error\n";
    }

    // accept a new connection
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done)
    {
        cout << "Listen Accept\n";
    }

    sf::Socket::Status status;
    size_t bytesRead_ = 0;
    char buffer_ [10000] = {0};

    //client.recive(beffer_, 10000, bytesRead_);
    status = client.receive(buffer_, 10000, bytesRead_);
    if(status != sf::Socket::Done)
    {
        std::cout << "Error rececive!" << endl;
    }
    else
    {
        cout << buffer;
    }

    /*while (window.isOpen())
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
    }*/

    return 0;
}














