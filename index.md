# Notes - Aman Desai
Table of Contents:
* [Project 1 - Mary Had a Little Lamb](#project-1---simple-melody)
* [Project 2 - Flute Synthesis](#project-2---instrument-synthesis)
* [Project 3 - Boid Simulation in Allolib](#project-3---personal-choice-project)

## Project 1 - Simple Melody

### Mary Had a Little Lamb
[![Mary Had a Little Lamb Video](https://img.youtube.com/vi/BiB_DIoA_Jw/0.jpg)](https://www.youtube.com/watch?v=BiB_DIoA_Jw) <br/>
(Click the image above to go to the video)<br/>
[GitHub Files](https://github.com/allolib-s24/notes-amanpdesai/tree/main/melody-001)

&nbsp;&nbsp;&nbsp;&nbsp;The above video demo is a short clip of using the allolib_playground library, using the 01_Sine_Env_Piano file to play Mary Had a Little Lamb. It was quite difficult for me as I am not very musically inclined at all. I learned how to play the melody using one voice as that was what was easiest for my skill set. Unfortunately, the recording is super choppy and I no longer have the synthSequence file as the computer I did that project on blew up...

## Project 2 - Instrument Synthesis

### Flute Synthesis
Flute Demo Video<br/>
[![Flute Demo Video](https://img.youtube.com/vi/6v6PHWqZZPY/0.jpg)](https://www.youtube.com/watch?v=6v6PHWqZZPY) <br/>
Flute My Melody<br/>
[![Flute My Melody](https://img.youtube.com/vi/RBjkYZFdzrA/0.jpg)](https://www.youtube.com/watch?v=RBjkYZFdzrA) <br/>
(Click the images above to go to the respective videos)<br/>
[GitHub Files](https://github.com/allolib-s24/notes-amanpdesai/tree/main/AMandSineSynth)

&nbsp;&nbsp;&nbsp;&nbsp;The goal of this project was to explore instrument synthesis techniques. I started by simply looking at all of the instruments in the tutorial/synthesis folder of allolib_playground while doing some research on how each of those respective synthesis techniques works. I chose two synthesis techniques that I found sounded good. Before combining the two I had no plan to simulate a specific instrument. However, for the third project, I wanted a sound that would work well to give me a mix of "unsettling" and "calming" sounds and I thought the flute would be the best for that. Once I combined AM and Sine wave synthesis, I played around with the ADSR values of my new instrument till it produced those "unsettling" and "calming" sounds.<br/>

The first video linked is a simple demo of the instrument I created, playing each of its notes. The second video linked is a simple melody I created myself with my instrument. Those are some of the notes I liked and I will probably use them for project 3.

## Project 3 - Personal Choice Project

## Boid Simulation in Allolib
&nbsp;&nbsp;&nbsp;&nbsp;For my final project, I decided to try and implement a boid simulation in 3D space, integrating with it different boid flocking behaviors, types of boids, and most importantly, sound. The code I started with is from Ryan which sets up an environment and a boid system already. Here is the link to it in case you want to see the original code: [Original Boid Code From Ryan](https://github.com/kr4g/MAT201B-2024-ryan-millett/tree/main/boids). Ryan's code gave me a good starting point as instead of having to fully understand and implement everything about boids from scratch using the Allolib Library, I could start with a boid system that had the basics coded already.<br/><br/>
&nbsp;&nbsp;&nbsp;&nbsp;After doing some looking online for pre-existing boid systems for inspiration, here is one that I really loved and tried to inherit from:
[![Zachhi FlockingBoids Project](https://github.com/Zachhi/FlockingBoids-VS/raw/master/boidsDemo.gif)](https://github.com/Zachhi/FlockingBoids-Windows-Linux/tree/master) <br/>
(^ NOT MY SIMULATION THAT IS AN INSPIRATION FOR MY PROJECT)<br/><br/>
&nbsp;&nbsp;&nbsp;&nbsp;A few things I wanted to try and implement that the code above did really well were the different types of flocking and the predator-prey interactions. I was able to implement three of my own "species" as I like to call them, represented by three different colored boids:<br/>
* Blue Boids: Average moving speed, Average separation, Average cohesion, Average Alignment, Average turn rate.
* Yellow Boids: Fast moving speed, High separation, Average cohesion, High Alignment, High turn rate.
* White Boids: Slow moving speed, Average separation, Average cohesion, Low Alignment, Low turn rate.
Because of a lack of time, I wasn't able to implement a predator-prey interaction but I probably will soon just for fun. If someone wanted to implement their own species, here are the steps I would suggest following:
1. Start by creating a new mesh variable. Follow the general format for the other meshes and give it some shape and form and color.
2. In the setUp() function in the code, there is an if block that essentially, based on what number boid is being created, assigns the type of the boid along with some attributes. Give the boid some custom attributes.
3. In the onDraw() function, include another else if block to g.draw(your mesh).
4. In the onAnimate() function add an else if block to the big if block and give the boid properties some values. (You can use simple float/double values or if you want to use parameters like I did you can create and add your own parameters at the top of the MyApp class. Make sure you also gui.add(parameter) in the onInit() function).
Again super sorry that the code is kind of ugly. I plan to clean it up in future versions.
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;The sounds were the most complicated part of the project by far. Because of the lack of clear documentation in some of the tutorial files in the code, I wasn't really sure how to implement my own sound so a good chunk of my code for the sound is copy pasted snippets that I found were useful through deleting stuff in the tutorials to see how the end result was affected. I started by looking in the tutorials folder where I found the 11_audio_spatialization_scene.cpp file under the tutorials/interaction-sequencing path. This file sets up a virtual environment in which when the space bar is pressed, an instance of a PositionedVoice is created. Using the Q, W, E, A, S, D, Z, X, and C keys, you can navigate the environment and see how the sound changes from different camera view-points. I copied the MyAgent class from this file and tweaked it a bunch. I started by changing the sound it emitted to use the AM and Sine synthesis combination I coded for Project 2. I also gave it some functions that would allow me to move it and set some important values. I also coded some random value generators for floats and doubles which take in a "center" value and a "delta" value and generate a random number from center-delta to center+delta. I will explain where this came in handy the most later.<br/><br/>
Finally, for the actual implementation of the PositionedVoice object: there were two techniques I could use to get this to work.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;1. Oct Tree Implementation:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This method is not limited to using fewer boids and voices as it is less computationally expensive. Using the generateCenters() function, the code generates a list of centers based on how many voices I want to implement (a multiple of 8). At each of those centers, we create a positioned voice that does not move. Using the Oct Tree, we query and get a list of all boids that are in the zone of the voice and average their frequencies. This is the frequency the voice emits. Evey voice at every center does this on every iteration.<br />
&nbsp;&nbsp;&nbsp;&nbsp;2. Direct Boid Voice Implementation<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This method limits us in terms of compute power as we have to keep the number of boids in the simulation lower. Voices are created at the boid positions. Additionally, the voices follow their boid around meaning that every iteration the voice emits the boid's frequency and moves to the boid's position.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;In both cases, we use the random float/double number generators to give the effect of a chorus/group of voices as when I had fixed values, the sound simply sounded like one voice coming from the center of mass of all of the boids (I am not an expert when it comes to sound synthesis so this may not be right but I assumed it was because the sine wave we were outputting was the exact same and the same sine waves layered over one another with very slightly different frequencies made all of the voices sound like one).<br/><br/>
I ended up with four files each using the same external files. Here is what each file is and what it showcases:<br><br>
&nbsp;&nbsp;&nbsp;&nbsp;1. noSoundBoids.cpp <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file is exactly what it seems. It is the boid simulation without any sound aspect to it.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Video Demo](https://youtu.be/Nih4F-f7yKQ) <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Github Link](https://github.com/allolib-s24/notes-amanpdesai/blob/main/boidSimulation/boids/noSoundBoids.cpp)<br>
&nbsp;&nbsp;&nbsp;&nbsp;2. soundTrial0.cpp <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file is a single boid flying around 3D space with random movement. It has one main purpose: to show the effect of the Positioned Voice in 3D space as it flies around. There is only one species of boid and the size of the 3D space is reduced to keep the boid flying closer to the camera.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Video Demo](https://youtu.be/n_irUqfct-Q) <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Github Link](https://github.com/allolib-s24/notes-amanpdesai/blob/main/boidSimulation/boids/soundTrial0.cpp)<br>
&nbsp;&nbsp;&nbsp;&nbsp;3. soundTrial1.cpp <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file showcases the Oct Tree Implementation for the voices of the boids. It has all of the species of boids and the full number of boids. The voice objects are at fixed positions but update their frequencies based on which boids are in their radius.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Video Demo](https://youtu.be/rvoQkzg1v9w) <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Github Link](https://github.com/allolib-s24/notes-amanpdesai/blob/main/boidSimulation/boids/soundTrial1.cpp)<br>
&nbsp;&nbsp;&nbsp;&nbsp;4. soundTrial2.cpp <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file showcases the Direct Boid Voice Implementation for the voices of the boids. It has all of the species of boids and a small number of boids. The voice objects follow their respective boids around the 3D space and emit only one boids frequency.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Video Demo](https://youtu.be/N2ih-cQ36kA) <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[Github Link](https://github.com/allolib-s24/notes-amanpdesai/blob/main/boidSimulation/boids/soundTrial2.cpp)<br><br/>

[GitHub Files](https://github.com/allolib-s24/notes-amanpdesai/tree/main/boidSimulation)
<br/><br/>

&nbsp;&nbsp;&nbsp;&nbsp;Overall I am quite happy with the progress I was able to make on this project and what I was able to accomplish. For future plans and what else I want to implement, implementing a predator into the space would make it look even more fascinating. I also hope to implement more species of boids with different flocking behaviors. Ryans original code had some beginnings to additional elements like food, lifespans, hunger, fear, attention span, and even mutations. Those aspects sound so cool, and I would probably want to implement more of those. I also want to clean up the code as currently, it is horrific to look at. A final goal that I hope to work towards is eventually also creating a virtual space with obstacles and graphics in which the boid simulation fits. Some ideas could be the ocean floor, a fish tank, the sky, a park, etc. Something along those lines, I feel, would really complete the simulation.
