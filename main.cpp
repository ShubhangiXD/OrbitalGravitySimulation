#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>

//making the planet in sfml
class GravitySource {
    sf::Vector2f pos;
    float gravStrength;
    sf::CircleShape circle;
    sf::CircleShape outline;
    float outlineRadius;

public:
    GravitySource(float posX, float posY, float gravStrength) {
        pos.x = posX;
        pos.y = posY;
        this->gravStrength = gravStrength;

        //initiating the position, colour and size of the planet.
        circle.setPosition(pos);
        circle.setFillColor(sf::Color::White);
        circle.setRadius(20);

        outlineRadius = circle.getRadius() * 5;
        outline.setPosition(pos.x - outlineRadius + circle.getRadius(), pos.y - outlineRadius + circle.getRadius());  // Adjust position based on the desired distance
        outline.setFillColor(sf::Color::Transparent);  // No fill color
        outline.setOutlineThickness(1);  // Set the outline thickness
        outline.setOutlineColor(sf::Color::Yellow);  // Set the outline color
        outline.setRadius(outlineRadius);  // Set the radius larger than the original circle

    }

    float getOutlineRadius() const {
        return outlineRadius;
    }

    void render(sf::RenderWindow& wind) {
        wind.draw(outline);
        wind.draw(circle);
    }

    sf::Vector2f getPosition() {
        return pos;
    }

    float getGravStrength() const {
        return gravStrength;
    }
    int getRadius() {
        int radius = 20;
        return radius;
    }
};

//making the particles in sfml
class Particles {

    //for the particles we do not require the gravity strength and
    //the change in position has to be accounted for using the velocity variables
    sf::Vector2f pos;
    sf::Vector2f velo;
    sf::CircleShape circle;
    std::vector<sf::Vertex> trail;  // Store the trail as a vector of vertices
    bool isAlive;
    bool isCircling;

public:
    Particles(float posX, float posY,float velX, float velY) : isAlive(true), isCircling(false) {
        pos.x = posX;
        pos.y = posY;
        velo.x = velX;
        velo.y = velY;

        circle.setPosition(pos);
        circle.setFillColor(sf::Color::White);
        circle.setRadius(10);
    }

    void render(sf::RenderWindow& wind) {
        circle.setPosition(pos);
        wind.draw(circle);
        for (auto & vertex : trail) {
            vertex.color.a = 64;  // Set a constant alpha value
        }
        if (trail.size() > 1) {
            wind.draw(&trail[0], trail.size(), sf::LinesStrip);
        }
    }

    int getRadius() {
        int radius = 10;
        return radius;
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
        sf::Vector2f trailPos = pos;
        trailPos.x += getRadius();  // Adjust for the center of the particle
        trailPos.y += getRadius();  // Adjust for the center of the particle
        trail.emplace_back(trailPos, sf::Color::White);

        // if (trail.size() > 200) {
        //     trail.erase(trail.begin());
        // }

        if (dist <= circle.getOutlineRadius()) {
            isCircling = true;
        }
        if (isCircling) {
            // Calculate the angle of the velocity vector
            float angle = std::atan2(velo.y, velo.x);

            // Add 90 degrees to the angle to make the particle move in a circle
            // angle += 3.14159f / 8;

            // Calculate the new velocity vector
            float speed = std::sqrt(velo.x * velo.x + velo.y * velo.y);

            float gravForceX = circle.getGravStrength() * invSqrDropOff * normX;
            float gravForceY = circle.getGravStrength() * invSqrDropOff * normY;
            float speedX = speed + gravForceX;
            float speedY = speed + gravForceY;
            velo.x = std::cos(angle) * speedX;
            velo.y = std::sin(angle) * speedY;

            // Check if the particle has reached the GravitySource
            if (dist <= circle.getRadius()) {
                // If it has, set isAlive to false
                isAlive = false;
            }
        }
    }
    void setColour(sf::Color colour) {
        circle.setFillColor(colour);
    }
    bool getIsAlive() const {
        return isAlive;
    }

};

//calculating colour
sf::Color mapValueToColour(float value) {
    if (value < 0.0f) value = 0;
    if (value > 1.0f) value = 1;
    int red = 0, green = 0, blue = 0;
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
    sources.emplace_back(500, 500, 3000);
    // sources.emplace_back(1200, 500, 7000);

    int numParticles = 1;
    std::vector<Particles> particles;
    particles.reserve(numParticles);
    for (int i=0; i < numParticles; i++) {
        particles.emplace_back(600, 700, 5,0.6+(0.1/numParticles)*i);
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
                if (particle.getIsAlive()) {  // Only update and render alive particles
                    particle.updatePhysics(source);
                    particle.render(window);
                }
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