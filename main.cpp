#include <SFML/Graphics.hpp>
#include <iostream>


float dist(float pX, float pY, float tX, float tY) {
    float dX = abs(tX - pX);
    float dY = abs(tY - pY);
    // x2 + y2 = z2
    float z = sqrt((dX * dX) + (dY * dY));
    return z;
}

float map(float num, float firstDist_a, float firstDist_b, float secondDist_a, float secondDist_b) {
    return (num - firstDist_a) * ((secondDist_b - secondDist_a) / (firstDist_b - firstDist_a)) + secondDist_a;
}

class Blob
{
public:

    sf::CircleShape crc;
    sf::Vector2f pos;

    float xVel;
    float yVel;

    float r;

    Blob(float x, float y)
    {
        this->pos = sf::Vector2f(x, y);
        this->xVel = (rand() % 6 - 3) * 10;
        this->yVel = (rand() % 6 - 3) * 10;
        r = 40;
    }

    void show() {
        this->crc.setPosition(this->pos);
        this->crc.setRadius(this->r);
        this->crc.setOutlineColor(sf::Color{ 255, 255, 255, 0 });
        this->crc.setFillColor(sf::Color{ 255, 255, 255, 0 });
        this->crc.setOutlineThickness(2);
        this->crc.setOrigin(this->crc.getRadius(), this->crc.getRadius());
    }

    void update(int sizeX, int sizeY) {
        if (this->xVel == 0)
        {
            this->xVel = (rand() % 6 - 3) * 10;
        }
        if (this->yVel == 0)
        {
            this->yVel = (rand() % 6 - 3) * 10;
        }
        this->pos += sf::Vector2f(this->xVel, this->yVel);

        if (this->pos.x >= sizeX || this->pos.x <= 0)
        {
            this->xVel *= -1;
        }
        if (this->pos.y >= sizeY || this->pos.y <= 0)
        {
            this->yVel *= -1;
        }
    }

    ~Blob()
    {
    }

private:

};


int main() {

    srand(time(0));
	
	int width = 300, height = 300;
	sf::RenderWindow window(sf::VideoMode(width, height), "Metaball Simulation", sf::Style::Close);

    sf::Image image;
    image.create(width, height);
    sf::Texture texture;
    sf::Sprite sprite;

    std::vector<Blob> blobs;
    for (int i = 0; i < 3; i++)
    {
        blobs.push_back(Blob(rand() % width, rand() % height));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear(sf::Color{ 51, 51, 51, 255 });


        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                float sum = 0;

                for (int i = 0; i < blobs.size(); i++)
                {
                    float d = dist(x, y, blobs[i].pos.x, blobs[i].pos.y);
                    float col = 3000 * blobs[i].r / d;
                    col = map(col, 0, 5000, 0, 255);
                    
                    sum += col;
                }

                if (sum >= 255)
                {
                    sum = 255;
                }

                image.setPixel(x, y, sf::Color{ (sf::Uint8)sum, (sf::Uint8)sum, (sf::Uint8)sum, 255 });
            }
        }


        
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        window.draw(sprite);

        for (int i = 0; i < blobs.size(); i++)
        {
            blobs[i].update(window.getSize().x, window.getSize().y);
            blobs[i].show();
            window.draw(blobs[i].crc);
        }
        window.display();
    }

	return 0;
}