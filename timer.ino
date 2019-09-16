/* Rotary encoder test code
 * Modified from http://www.circuitsathome.com/mcu/programming/reading-rotary-encoder-on-arduino by Oleg
 * Latest mod by Jessica Faruque 7/17/2013
 */
 
#include <LiquidCrystal.h>
#include "pitches.h"

#define ENC_A 6 //these need to be digital input pins
#define ENC_B 7

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int sensorPin = A1;    // select the input pin for the potentiometer
float sensorValue = 0;  // variable to store the value coming from the sensor
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ENC_SWITCH 0 //Switch button to switch between time setter option

#define START 0
#define SET_TIMER 1
#define SUB_TIMER_SET 2
#define SUB_TIMER_CLEAR 3
#define SUB_TIMER_BACK 4
#define COUNTER_START 5
#define SUB_TIMER_SET_COUNTER_START 6
#define SUB_TIMER_SET_RESET 7
#define TIMER_RUNNING 8
#define TIMER_DONE 9
#define SUB_TIMER_SET_TIME 10

int timerCount = 0;
int menuState = 0;
int timer = 0;
static unsigned int counter = 0;
static unsigned int prevCounter = 0;
static unsigned int menuCounter = 0;
static unsigned int prevMenuCounter = 0;
int prevState = -1;

void resetLCD() {
  lcd.noDisplay();
  delay(500);
  // Turn on the display:
  lcd.display();
  lcd.clear();
}

int melody[] = {
  NOTE_D3,NOTE_D3,NOTE_D3,NOTE_G3,NOTE_D4,NOTE_C4,NOTE_B3,NOTE_A3,NOTE_G4,NOTE_D4,NOTE_C4,NOTE_B3,NOTE_A3,NOTE_G4,NOTE_D4,NOTE_C4,NOTE_B3,NOTE_C4,NOTE_A3,0};
 
int noteDurations[] = {
  10,10,10,2,2,10,10,10,2,4,10,10,10,2,4,10,10,10,2,4};


boolean running = true;
int swReading;
int swPrevious = LOW;
boolean updateNeeded = false;

void setup()
{
  /* Setup encoder pins as inputs */
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SWITCH, INPUT);

  digitalWrite(ENC_SWITCH, HIGH);
 
  Serial.begin (9600);
  Serial.println("Start");

  //Screen print
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.display();
  lcd.clear();
  menuState = START;
  updateNeeded = true;
  
  //Music
  // iterate over the notes of the melody:
  /*
  
  */
    
}

void playMusic() {
  for (int thisNote = 0; thisNote < 20; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void trackCounter() {
  static unsigned int counter4x = 0;      //the SparkFun encoders jump by 4 states from detent to detent
  int tmpdata;
  tmpdata = read_encoder();
  if( tmpdata) {
    counter4x += tmpdata;
    counter = counter4x/4;
    menuCounter = counter4x/4;
    
    counter = counter % 60;
    menuCounter = menuCounter % 3;
    if (prevCounter != counter){
      Serial.print("Counter value: ");
      Serial.println(counter);
    }
    
    if (prevMenuCounter != menuCounter){
      Serial.print("Menu Counter value: ");
      Serial.println(menuCounter);
    }
    
    prevCounter = counter;
    prevMenuCounter = menuCounter;

    rotatorMoved();
  }
  
}


void loop()
{


  swReading = digitalRead(ENC_SWITCH);

  if(swReading == LOW && swPrevious == HIGH) {
    running = !running;
  }
  swPrevious = swReading;

  if(!running) {
    switchMenu();
    running = !running;
  }

  trackCounter();
  updateDisplay();
  
}

void printT(String inp) {
  Serial.print("State: ");
  Serial.println(inp);
}

void updateDisplay() {
  if(!updateNeeded)
    return;
   lcd.clear();
  switch(menuState) {
    case START:
       lcd.print("PRESS");
      prevState = menuState;
      break;
    case SUB_TIMER_SET:
      lcd.print("SET TIME");
      break;
    case SUB_TIMER_SET_TIME:
      lcd.print("SET TIME");
      break;
    case SUB_TIMER_CLEAR:
      lcd.print("CLEAR TIMER");
      break;
    case SUB_TIMER_BACK:
      lcd.print("GO BACK");
      break;
    case COUNTER_START:
      lcd.print("Seconds :" + String(counter));
      break;
    case SUB_TIMER_SET_RESET:
      lcd.print("GO BACK");
      break;
    case TIMER_RUNNING:
      lcd.print("Timer - " + String(timerCount));
      break;
    case TIMER_DONE:
      lcd.print("Done");
      break;
    default:
      lcd.print("Waiting");
      break;
  }

  prevState = menuState;
  updateNeeded = false;
}

void resetCounter() {
      counter = 0;
      prevCounter = 0;
}

void resetMenuCounter() {
      menuCounter = 0;
      prevMenuCounter = 0;
}


void switchMenu() {
    
  lcd.clear();
  switch (menuState) {
    case START:
      menuState = SUB_TIMER_SET;
      break;
    case SUB_TIMER_SET:
      menuState = SUB_TIMER_SET_TIME;
      break;
    case SUB_TIMER_CLEAR:
      resetCounter();
      menuState = START;
      break;
    case SUB_TIMER_BACK:
      resetCounter();
      menuState = START;
      break;
    case SUB_TIMER_SET_TIME:
      menuState = COUNTER_START;
      resetCounter();
      break;
    case COUNTER_START:
      menuState = TIMER_RUNNING;
      startTimer();
      break;
    case SUB_TIMER_SET_RESET:
      menuState = START;
      resetCounter();
      break;
    case TIMER_DONE:
      menuState = START;
    default:
      menuState = START;
      break;
  }

  printT("Switching State :" + String(menuState));
  updateNeeded = true;
}

void setMenuState() {
  switch (menuCounter) {
    case 0:
    menuState = SUB_TIMER_SET_TIME;
    break;
    case 1:
    menuState = SUB_TIMER_CLEAR;
    break;
    case 2:
    menuState = SUB_TIMER_BACK;
    break;
    default:
    break;
    
  }
}

void startTimer() {
  
  timerCount = counter;
  for (timerCount; timerCount > 0; timerCount--) {
    updateNeeded = true;
    delay(987);
    updateDisplay();
  }

  menuState = TIMER_DONE;
  updateDisplay();
  playMusic();
}

void rotatorMoved() {
  switch (menuState) {
    case SUB_TIMER_SET:
    case SUB_TIMER_SET_TIME:
    case SUB_TIMER_BACK:
    case SUB_TIMER_CLEAR:
     setMenuState();
     break;
    default:
     break;
  }

  updateNeeded = true;
  printT("Rotator Moved = Update");
  
}

 
/* returns change in encoder state (-1,0,1) */
int read_encoder()
{
  static int enc_states[] = {
    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
  static byte ABab = 0;
  ABab *= 4;                   //shift the old values over 2 bits
  ABab = ABab%16;      //keeps only bits 0-3
  ABab += 2*digitalRead(ENC_A)+digitalRead(ENC_B); //adds enc_a and enc_b values to bits 1 and 0
  return ( enc_states[ABab]);
 
}
