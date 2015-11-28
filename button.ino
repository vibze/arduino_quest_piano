#include "button.c"
#include "BY8001.h"
#include <SoftwareSerial.h>

Button C = newButton(9);
Button D = newButton(10);     // the number of the pushbutton pin
Button E = newButton(11);
Button F = newButton(8); 
const int ledPin =  13;      // the number of the LED pin

const Button target[4] = {C, C, C, D};
int input[4] = {0};
boolean playing = false;

SoftwareSerial mp3Serial(2, 3);
BY8001 mp3;
 
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(C.pin, INPUT);
  pinMode(D.pin, INPUT);
  pinMode(E.pin, INPUT);
  //pinMode(F.pin, INPUT);
  
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);

  mp3Serial.begin(9600);
  mp3.setup(mp3Serial);

  delay(1000);

  int by8001_VolumeSetting = 20; //  Set volume here to 15 (0-30 range) if not already set
  if ((int)mp3.getVolumeSetting() != by8001_VolumeSetting) mp3.setVolume(by8001_VolumeSetting);
  mp3.stopPlayback();
}

void loop() {
  Button* buttons[4] = {&C, &D, &E, &F};
  checkButtons(buttons, 4, onButtonDown, onButtonUp);

  if (validate() && playing == false) {
    play();
  }
}

void addNote(int note) {
  for(int i=1; i<sizeof(input)/sizeof(int); i++) {
    input[i-1] = input[i];
  } 
  input[sizeof(input)/sizeof(int)-1] = note;
  
  for(int i=0; i<sizeof(input)/sizeof(int); i++) {
    Serial.print(input[i]);
    Serial.print(" - ");
  }
  Serial.print("\n");
}

bool validate() {
  for(int i=0; i<sizeof(target)/sizeof(Button); i++) {
    if (input[i] != target[i].pin) {
      return false;
    }
  }

  return true;
}

boolean pinOn = false;
void onButtonDown(Button* button) {
  addNote(button->pin);
}

void onButtonUp(Button* button) {
}

void play() {
  playing = true;
  mp3.setLoopPlaybackMode(3);  // set playback mode to Random (Shuffle)
  mp3.play();
  digitalWrite(ledPin, HIGH);
  Serial.println("\nLoop playback mode...\n\n  playing tracks at random");
}
