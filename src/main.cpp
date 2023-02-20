#include "tetris.h"
#include "display.h"
#include "menu.h"

Menu m;
Tetris t;

void initBtns(){
    pinMode(IPIN0, INPUT_PULLUP);
    pinMode(IPIN1, INPUT_PULLUP);
    pinMode(IPIN2, INPUT_PULLUP);
    pinMode(IPIN3, INPUT_PULLUP);
}

int score = 0;

void setup() {

    Serial.begin(9600);
    initBtns();
    initDisplay();
    
    m = Menu();
    t = Tetris();
    score = t.play();
};

void loop() {

    /*if(digitalRead(IPIN0)==0){
        display.setRotation(0); 
        score = t.play();
        m.setScore(0, score);
    }*/
}
