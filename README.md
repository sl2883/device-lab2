#### device-lab2

##### What voltage level do you need to power your display?
5V

##### What voltage level do you need to power the display backlight?
3.5V

##### What was one mistake you made when wiring up the display? How did you fix it?
It worked for me the first time. But, I struggled with using the rotatory encoder for my timer part. I wanted to use the button part of the rotator to work as a state changer. I was expecting the loop option will get the state of the button as pressed or released, but that didn't work. After pressing the button, I was continuously getting the HIGH value in the digitalRead. 
I used a switch button instead, which also had the same problem. After using a logic in the code to detect state switch, I was able to emulate the pressed/released option correctly in the code.

##### What line of code do you need to change to make it flash your name instead of "Hello World"?
lcd.print("Sunny Ladkani!");

##### Include a copy of your Lowly Multimeter code in your lab write-up.
[Multimeter Code](https://github.com/sl2883/device-lab2/blob/master/Multimeter.ino)

##### How would you change the code to make the song play twice as fast?
int noteDuration = 1000 / noteDurations[thisNote];
to
int noteDuration = 1000 / noteDurations[thisNote]/2;

##### What song is playing?
I think it's the star wars

##### Make a short video showing how your timer works, and what happens when time is up!
[Timer Video](https://youtu.be/M6u6lQ4u8rU)

The timer works as a state machine. 
* To start, you press the button at the centre of the breadboard.
* You are taken to the "main menu" state where you can select the following -
  * Set Timer
  * Clear Timer
  * Go Back
* When you press the button for "Set Timer", you are taken to a new state where you can use the rotatory encoder to input time. * On next press, you are given the option to start timer or go back. 
* On press, the timer starts and when it finishes, the "Star wars" music plays.

##### Post a link to the completed lab report your GitHub repo.
[Timer Code](https://github.com/sl2883/device-lab2/blob/master/timer.ino)



