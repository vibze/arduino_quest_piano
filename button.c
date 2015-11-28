#include "Arduino.h"

const int buttonWait = 500;
const int yes = 1;
const int no = 0;

typedef struct button {
  int pin;
  int state;
  long ignoreFrom;
} Button;


// Button initializer
Button newButton(int pin) {
  Button nb;
  nb.pin = pin;
  nb.state = HIGH;
  nb.ignoreFrom = -1;
  return nb;
}

// Loop button checker
int checkButtons(Button* buttons[], int s, void (*onButtonDown)(Button*), void (*onButtonUp)(Button*)) {
  int i;
  for (i=0; i<s; i++) {
    // If ignore timer on button has run out reset it.
    if (buttons[i]->ignoreFrom != -1 && millis() > buttons[i]->ignoreFrom + buttonWait) {
      buttons[i]->ignoreFrom = -1;
    }

    // If ignore timer isnt set we dont ignore this
    if (digitalRead(buttons[i]->pin) != buttons[i]->state && buttons[i]->ignoreFrom == -1) {
      buttons[i]->state = digitalRead(buttons[i]->pin);

      if (buttons[i]->state == LOW) {
        onButtonDown(buttons[i]);
        buttons[i]->ignoreFrom = millis();
      }
    }
  }
}
