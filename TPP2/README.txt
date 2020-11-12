Nikita Tran
CPSC 4140
Assignment 4
November 12, 2020

Inputs: A (move left), D (move right)

Collision handling is in GameEngine.cpp (IsColliding function, called in HandleEvents). I only use box colliders, no circle distance.
Particle system classes are Particle.cpp and ParticleEmitter.cpp, ParticleEmitter is used in GameEngine.cpp

Colliding with the shopping cart pushes it in the direction the player is moving. Colliding with the hand sanitizer triggers particle effect.
The hand sanitizer will be a collectable to restore health which is why it disappears after being collided with. 
If I implement a health bar, I want to use the same green sparkle effect when health is being regained so it looks like the hand sanitizer disappeared into the health bar.

Art sources:
- Player sprites, shopping cart, sparkle particle drawn by me
- Hand sanitizer sprite source: https://clipartpng.com/?3267,green-hand-sanitizer-png-clipart


Makefile is edited from the starter code makefile; "make clean" then "make" then "make run" to compile and run game from command line