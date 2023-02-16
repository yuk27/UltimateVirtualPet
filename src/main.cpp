#include "utilities.h"
#include "tetris.h"
#include "display.h"
//#include "avr8-stub.h"
//#include "app_api.h"

#define IPIN0 8
#define IPIN1 10
#define IPIN2 11
#define IPIN3 9
#define DWAIT 1000;

Tetris t;
int status = -1;
int dvalue = 0;

int xval = 0; 
int waitTime = DWAIT;  // initial 1000ms
unsigned long accel = 200; // decrease 100 ms
unsigned long ntime = 0;
uint8_t counter = 0;
uint8_t id = 0;

uint8_t btn[4] = {1,1,1,1};
uint8_t pbtn[4] = {1,1,1,1};


void initBtns(){
    pinMode(IPIN0, INPUT_PULLUP);
    pinMode(IPIN1, INPUT_PULLUP);
    pinMode(IPIN2, INPUT_PULLUP);
    pinMode(IPIN3, INPUT_PULLUP);
}

void setup() {

    Serial.begin(9600);
    Serial.println();
    initBtns();
    initDisplay();
    t = Tetris();
    t.draw_grid();
    t.drawScreen();
    
    while(true){
        while(status < 2){
            id = random(6);
            t.add_piece(id,xval,0); 

            counter++;
            if(counter > 4){
                counter = 0; 
                
                if(waitTime -= accel >= 0){
                    waitTime -= accel;
                }
            }

            status = 0;
            while (status < 1)
            {
                if (ntime < millis()){
                    status = t.moveY();
                    ntime = millis() + waitTime;
                }
                else{

                    btn[0] = digitalRead(IPIN0);
                    btn[1] = digitalRead(IPIN1);
                    btn[2] = digitalRead(IPIN2);
                    btn[3] = digitalRead(IPIN3);

                    if(btn[0] == 0 && pbtn[0] == 1){
                        t.rotate(id);
                    }

                    if(btn[1] == 0){// && pbtn[1] == 0){
                        t.moveX(1);
                    }
                    
                    if(btn[2] == 0){//&& pbtn[2] == 0){
                        t.moveX(-1);
                    }

                    if(btn[3] == 0){
                        status = t.moveY();
                        ntime = millis() + waitTime;
                    }

                    /*if(btn[3] == 1 && pbtn[3] == 0){
                        
                        status = t.moveY();
                        if(status < 1){
                            status = t.moveY();
                        }
                        
                        ntime = millis() + waitTime;
                    }*/

                    t.drawScreen();

                    pbtn[0] = btn[0];
                    pbtn[1] = btn[1];
                    pbtn[2] = btn[2];
                    pbtn[3] = btn[3];

                }
                t.drawScreen();
            }
        }
        t.draw_gameover();
        waitTime = DWAIT
        status = 0;
    }
}

void loop() {

}
