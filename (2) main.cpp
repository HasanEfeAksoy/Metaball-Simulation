#include <SFML/Graphics.hpp>

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

int width = 400, height = 400;


int main() {

    srand(time(0));

    
    sf::RenderWindow window(sf::VideoMode(width, height), "Metaball Simulation", sf::Style::Close);

    sf::Image image;
    image.create(width, height);
    sf::Texture texture;
    sf::Sprite sprite;


    struct Blobs
    {
        int radius = 40;
        int posX = rand() % width;
        int posY = rand() % height;
        
        float velocityX = (rand() % 6 - 3) * 10;
        float velocityY = (rand() % 6 - 3) * 10;

        void update() {
            if (this->velocityX == 0)
            {
                this->velocityX = (rand() % 6 - 3) * 10;
            }
            if (this->velocityY == 0)
            {
                this->velocityY = (rand() % 6 - 3) * 10;
            }
            
            this->posX += this->velocityX;
            this->posY += this->velocityY;

            if (this->posX >= width || this->posX <= 0)
            {
                this->velocityX *= -1;
            }
            if (this->posY >= height || this->posY <= 0)
            {
                this->velocityY *= -1;
            }
        }
    };


    std::vector<Blobs> blobs;

    for (int i = 0; i < 4; i++)
    {
        blobs.push_back(Blobs());
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

        for (int i = 0; i < blobs.size(); i++)
        {
            blobs[i].update();
        }

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                float sum = 0;

                for (int i = 0; i < blobs.size(); i++)
                {
                    float d = dist(x, y, blobs[i].posX, blobs[i].posY);
                    float col = 3000 * blobs[i].radius / d;
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

        window.display();
    }

    return 0;
}