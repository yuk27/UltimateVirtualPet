//#include "utilities.h"
#include "tetris.h"
#include "display.h"
//#include "avr8-stub.h"
//#include "app_api.h"

Tetris t;

void initBtns(){
    pinMode(IPIN0, INPUT_PULLUP);
    pinMode(IPIN1, INPUT_PULLUP);
    pinMode(IPIN2, INPUT_PULLUP);
    pinMode(IPIN3, INPUT_PULLUP);
}

void setup() {

    Serial.begin(9600);
    initBtns();
    initDisplay();
    t = Tetris();
    t.start_game();
}

void loop() {

}
