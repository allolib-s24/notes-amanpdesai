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
[![Flute Demo Video](https://img.youtube.com/vi/ID_HERE/0.jpg)](https://www.youtube.com/watch?v=ID_HERE) <br/>
[![Flute My Melody](https://img.youtube.com/vi/ID_HERE/0.jpg)](https://www.youtube.com/watch?v=ID_HERE) <br/>
(Click the images above to go to the respective videos)<br/>
[GitHub Files](https://github.com/allolib-s24/notes-amanpdesai/tree/main/AMandSineSynth)

&nbsp;&nbsp;&nbsp;&nbsp;The goal of this project was to explore instrument synthesis techniques. I started by simply looking at all of the instruments in the tutorial/synthesis folder of allolib_playground while doing some research on how each of those respective synthesis techniques works. I chose two synthesis techniques that I found sounded good. Before combining the two I had no plan to simulate a specific instrument. However, for the third project, I wanted a sound that would work well to give me a mix of "unsettling" and "calming" sounds and I thought the flute would be the best for that. Once I combined AM and Sine wave synthesis, I played around with the ADSR values of my new instrument till it produced those "unsettling" and "calming" sounds.<br/>

The first video linked is a simple demo of the instrument I created, playing each of its notes. The second video linked is a simple melody I created myself with my instrument. Those are some of the notes I liked and I will probably use them for project 3.

## Project 3 - Personal Choice Project

## Boid Simulation in Allolib
&nbsp;&nbsp;&nbsp;&nbsp;For my final project, I decided to try and implement a boid simulation in 3D space, integrating with it different boid flocking behaviors, types of boids, and most importantly, sound. The code I started with is from Ryan which sets up an environment and a boid system already. Here is the link to it in case you want to see the original code: [Original Boid Code From Ryan](https://github.com/kr4g/MAT201B-2024-ryan-millett/tree/main/boids). Ryan's code gave me a good starting point as instead of having to fully understand and implement everything about boids from scratch using the Allolib Library, I could start with a boid system that had the basics coded already.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;After doing some looking online for pre-existing boid systems for inspiration, here is one that I really loved and tried to inherit from:
[![Zachhi FlockingBoids Project](https://github.com/Zachhi/FlockingBoids-VS/raw/master/boidsDemo.gif)](https://github.com/Zachhi/FlockingBoids-Windows-Linux/tree/master) <br/>
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
&nbsp;&nbsp;&nbsp;&nbsp;The sounds were the most complicated part of the project by far. Because of the lack of clear documentation in some of the tutorial files in the code, I wasn't really sure how to implement my own sound so a good chunk of my code for the sound is copy pasted snippets that I found were useful through deleting stuff in the tutorials to see how the end result was affected. I started by looking in the tutorials folder where I found [11_audio_spatialization_scene.cpp](https://github.com/AlloSphere-Research-Group/allolib_playground/blob/master/tutorials/interaction-sequencing/11_audio_spatialization_scene.cpp) under the tutorials/interaction-sequencing path.
