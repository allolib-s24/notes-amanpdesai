&nbsp;&nbsp;&nbsp;&nbsp;1. [noSoundBoids.cpp]()<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file is exactly what it seems. It is the boid simulation without any sound aspect to it.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;2. [soundTrial0.cpp]()<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file is a single boid flying around 3D space with random movement. It has one main purpose: to show the effect of the Positioned Voice in 3D space as it flies around. There is only one species of boid and the size of the 3D space is reduced to keep the boid flying closer to the camera.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;3. [soundTrial1.cpp]()<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file showcases the Oct Tree Implementation for the voices of the boids. It has all of the species of boids and the full number of boids. The voice objects are at fixed positions but update their frequencies based on which boids are in their radius.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;4. [soundTrial2.cpp]()<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file showcases the Direct Boid Voice Implementation for the voices of the boids. It has all of the species of boids and a small number of boids. The voice objects follow their respective boids around the 3D space and emit only one boids frequency.<br/><br/>
