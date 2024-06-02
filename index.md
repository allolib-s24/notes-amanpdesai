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

The above video demo is a short clip of using the allolib_playground library, using the 01_Sine_Env_Piano file to play Mary Had a Little Lamb. It was quite difficult for me as I am not very musically inclined at all. I learned how to play the melody using one voice as that was what was easiest for my skill set. Unfortunately, the recording is super choppy and I no longer have the synthSequence file as the computer I did that project on blew up...

## Project 2 - Instrument Synthesis

### Flute Synthesis
[![Flute Demo Video](https://img.youtube.com/vi/ID_HERE/0.jpg)](https://www.youtube.com/watch?v=ID_HERE) <br/>
[![Flute My Melody](https://img.youtube.com/vi/ID_HERE/0.jpg)](https://www.youtube.com/watch?v=ID_HERE) <br/>
(Click the images above to go to the respective videos)<br/>
[GitHub Files](https://github.com/allolib-s24/notes-amanpdesai/tree/main/AMandSineSynth)

The goal of this project was to explore instrument synthesis techniques. I started by simply looking at all of the instruments in the tutorial/synthesis folder of allolib_playground while doing some research on how each of those respective synthesis techniques works. I chose two synthesis techniques that I found sounded good. Before combining the two I had no plan to simulate a specific instrument. However, for the third project, I wanted a sound that would work well to give me a mix of "unsettling" and "calming" sounds and I thought the flute would be the best for that. Once I combined AM and Sine wave synthesis, I played around with the ADSR values of my new instrument till it produced those "unsettling" and "calming" sounds.<br/>

The first video linked is a simple demo of the instrument I created, playing each of its notes. The second video linked is a simple melody I created myself with my instrument. Those are some of the notes I liked and I will probably use them for project 3.

## Project 3 - Personal Choice Project

## Boid Simulation in Allolib
For my final project, I decided to try and implement a boid simulation in 3D space, integrating with it different boid flocking behaviors, types of boids, and most importantly, sound. The code I started with is from Ryan which sets up an environment and a boid system already. Here is the link to it in case you want to see the original code: (Original Boid Code From Ryan)[https://github.com/kr4g/MAT201B-2024-ryan-millett/tree/main/boids]. Ryan's code gave me a good starting point as instead of having to fully understand and implement everything about boids from scratch using the Allolib Library, I could start with a boid system that had the basics coded already.<br/>
After doing some looking online for pre-existing boid systems for inspiration, here is one that I really loved and tried to inherit from:
[![Zachhi FlockingBoids Project](https://github.com/Zachhi/FlockingBoids-VS/raw/master/boidsDemo.gif)](https://github.com/Zachhi/FlockingBoids-Windows-Linux/tree/master) <br/>
A few things I wanted to try and implement that the code above did really well were the different types of flocking and the predator-prey interactions. I was able to implement three of my own "species" as I like to call them, represented by three different colored boids:<br/>
* Blue Boids: Average moving speed, Average separation, Average cohesion, Average Alignment, Average turn rate.
* Yellow Boids: Fast moving speed, High separation, Average cohesion, High Alignment, High turn rate.
* White Boids: Slow moving speed, Average separation, Average cohesion, Low Alignment, Low turn rate.
