# device-lab2

## What voltage level do you need to power your display?
5V

## What voltage level do you need to power the display backlight?
3.5V

## What was one mistake you made when wiring up the display? How did you fix it?
It worked for me the first time.

## What line of code do you need to change to make it flash your name instead of "Hello World"?
lcd.print("Sunny Ladkani!");

## Include a copy of your Lowly Multimeter code in your lab write-up.
[Multimeter Code](https://github.com/sl2883/device-lab2/blob/master/Multimeter.ino)

## How would you change the code to make the song play twice as fast?
int noteDuration = 1000 / noteDurations[thisNote];
to
int noteDuration = 1000 / noteDurations[thisNote]/2;

## What song is playing?
I think it's the star wars



