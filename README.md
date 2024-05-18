# OrbitalGravitySimulation
This code simulates a simple gravity system using the SFML library. It creates two objects that exert a gravitational pull on a number of particles. The particles move according to the laws of classical mechanics, specifically using Verlet integration. The code also includes a function to map a value to a color, which is used to color the particles based on an index.

![GravitySimulation](https://github.com/ShubhangiXD/OrbitalGravitySimulation/assets/92100787/e6cf3999-93f2-464f-a5fe-0d570af4a551)

## Code Structure
### GravitySource
The so called 'star' declaration.
It has the following attributes: <br>
`pos`: The position of the gravity source as a 2D vector. <br>
`gravStrength`: The strength of the gravity exerted by the source.<br>
`circle`: A circle shape used to represent the source visually. Made using SFML's CircleShape.<br>
<br>
It has the following methods:<br>
`GravitySource(float posX, float posY, float gravStrength)`: Constructor that initializes the position and gravity strength of the source. <br>
`render(sf::RenderWindow& wind)`: Draws the circle representing the source on the window.<br>
`getPosition()`: Returns the position of the source as a 2D vector.<br>
`getGravStrength() const`: Returns the strength of the gravity exerted by the source.<br>
<br><br>

### Particles
It has the following attributes: <br>
`pos`: The position of the particle as a 2D vector.<br>
`velo`: The velocity of the particle as a 2D vector.<br>
`circle`: A circle shape used to represent the particle visually. Made using SFML's CircleShape.<br>
<br>
It has the following methods:<br>
`Particles(float posX, float posY,float velX, float velY)`: Constructor that initializes the position and velocity of the particle.<br>
`render(sf::RenderWindow& wind)`: Draws the circle representing the particle on the window.<br>
`updatePhysics(GravitySource& circle)`: Updates the position and velocity of the particle based on the gravitational pull of the source.<br>
`setColour(sf::Color colour)`: Sets the color of the particle.<br>
<br><br>

### Main Function:

The `main` function creates a window, initializes two gravity sources, and a number of particles.
It then enters a loop that:
Handles window closing events.
Clears the window.
Updates the position and velocity of each particle based on the gravity from each source.
Draws all the gravity sources and particles on the window.
Displays the window contents. <br> <br>
Additional Notes:<br>

The code uses Verlet integration to update the position and velocity of the particles.<br>
The color of each particle is based on its index in the particle list.
<br>
<br>
