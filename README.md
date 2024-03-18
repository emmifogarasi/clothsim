This was a programming assignment for a unit in BA Computer Animation Technical Arts at Bournemouth University in 2021. 

I have created a cloth simulation based on Spring Theory, where the user can define several variables, such as gravity, wind, and constraints. This simulation also demonstrates collision between cloth and a solid object. 
My research included looking at tutorials of Nature of Code on Youtube, where Spring Theory is explained. (https://www.youtube.com/watch?v=cluKQOY92Dw). According to this video, in springs, a so called "bob" hangs from an origin point, otherwise called "anchor".
The rest length between the "bob" and "anchor" is the length at which the bob does not move - if the bob is pulled further away or pushed closer to the anchor, it will move back to its original position. The more you push the "bob" away from its original position, the more force will be applied to push it back to its original position. 

I have used David Baraff's research paper called "Large Steps in Cloth Simulation" to understand the theory behind the mathematics of Spring Theory. I have also used Mosegaard's cloth simulation coding tutorial to kickstart this project (https://viscomp.alexandra.dk/?p=147).

I am planning to improve this project by improving the ball collision and the lighting. I also want to allow the user to move the cloth around in the environment. 
Currently, if the iteration is set too low, the cloth simulation is most likely to fall - I will implement a limit here, however currently, this can be fixed with the "reset" button. 

![image](https://github.com/emmifogarasi/clothsim/assets/163855278/1a31b8e7-b6de-4fee-a532-67af4c2c2c23)

