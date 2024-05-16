#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>

//making the planet in sfml
class GravitySource {
    sf::Vector2f pos;
    float gravStrength;
    sf::CircleShape circle;

public:
    GravitySource(float posX, float posY, float gravStrength) {
        pos.x = posX;
        pos.y = posY;
        this->gravStrength = gravStrength;

        //initiating the position, colour and size of the planet.
        circle.setPosition(pos);
        circle.setFillColor(sf::Color::White);
        circle.setRadius(10);
    }

    void render(sf::RenderWindow& wind) {
        wind.draw(circle);
    }

    sf::Vector2f getPosition() {
        return pos;
    }

    float getGravStrength() const {
        return gravStrength;
    }
};

//making the particles in sfml
class Particles {

    //for the particles we do not require the gravity strength and
    //the change in position has to be accounted for using the velocity variables
    sf::Vector2f pos;
    sf::Vector2f velo;
    sf::CircleShape circle;

public:
    Particles(float posX, float posY,float velX, float velY) {
        pos.x = posX;
        pos.y = posY;
        velo.x = velX;
        velo.y = velY;

        circle.setPosition(pos);
        circle.setFillColor(sf::Color::White);
        circle.setRadius(5);
    }

    void render(sf::RenderWindow& wind) {
        circle.setPosition(pos);
        wind.draw(circle);
    }

    //can change the number of references for this.
    //GravitySource &{reference}
    void updatePhysics(GravitySource& circle) {

        //calculating the distance between the two points using pythagorean theorem.
        //Euclidean is another form of pythagorean, just use this.
        float distX = circle.getPosition().x - pos.x;
        float distY = circle.getPosition().y - pos.y;
        float dist = std::sqrt(distX*distX + distY*distY);

        //normalising distance vector means dividing X and Y by the dist,
        //but division is slower than multiplication, so we find inverse
        //and multiply twice.
        float invDist = 1.0f/dist;
        float normX = invDist * distX;
        float normY = invDist * distY;

        //using inverse square law
        float  invSqrDropOff = invDist * invDist;

        //adding the dist to the velocity which means VERLET INTEGRATIONNN
        float accX = normX * circle.getGravStrength() * invSqrDropOff;
        float accY = normY * circle.getGravStrength() * invSqrDropOff;
        velo.x += accX;
        velo.y += accY;

        pos.x += velo.x;
        pos.y += velo.y;
    }
    void setColour(sf::Color colour) {
        circle.setFillColor(colour);
    }
};

//calculating colour
sf::Color mapValueToColour(float value) {
    if (value < 0.0f) value = 0;
    if (value > 1.0f) value = 1;
    int red = 0, green = 0, blue = 0;
    //purple peach gradient
    // if (value < 0.5) {
    //     red = 128 + (255 - 128) * 2 * value;
    //     green = 0 + (170 - 0) * 2 * value;
    //     blue = 128;  // Stays constant for purple-pink transition
    // } else {
    //     red = 255;  // Stays constant for pink
    //     green = 170;  // Stays constant for pink
    //     blue = 128 + (200 - 128) * (value - 0.5);
    // }
    if (value < 0.5) {
        // Transition from blue to green
        blue = 255;  // Keep blue at its maximum
        green = 255 * value;  // Increase green gradually
        red = 0;  // Keep red at 0 for pure blue-green blend
    } else {
        // Transition from green to teal (slightly blueish green)
        blue = 255 * value;  // Increase blue gradually
        green = 255 * (1.0f - value);  // Decrease green gradually
        red = 0;  // Keep red at 0 for pure blue-green blend
    }
    return sf::Color(red, green, blue);
}

int main()
{
    //setting up a basic window for a graphical application, handling window closing events,
    //and a loop for handling animation and rendering.
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity Simulation");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources;
    sources.emplace_back(500, 500, 7000);
    sources.emplace_back(1200, 500, 7000);

    int numParticles = 2000;
    std::vector<Particles> particles;
    particles.reserve(numParticles);
    for (int i=0; i < numParticles; i++) {
        particles.emplace_back(600, 700, 4,0.1+(0.1/numParticles)*i);
        float val = (float)i / (float)numParticles;
        sf::Color col = mapValueToColour(val);
        particles[i].setColour(col);
    }

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        window.clear();
        for (auto & source : sources) {
            for(auto & particle : particles) {
                particle.updatePhysics(source);
            }
        }

        for (auto & source : sources) {
            source.render(window);
        }
        for (auto & particle : particles) {
            particle.render(window);
        }

        window.display();
    }
    return 0;
}