#ifndef PRINT_H
#define PRINT_H

#include <Arduino.h>

#define IPIN0 8
#define IPIN1 10
#define IPIN2 11
#define IPIN3 9

void println(String input){
    Serial.println(input);
}

void print(String input){
    Serial.print(input);
}

void print(int8_t input[], int size){
    Serial.print("(");
    for (int i = 0; i < size; i++){
        if(i > 0){
            Serial.print(",");
        }
        Serial.print(input[i]);
    }
    Serial.println(")");
}

void println(int8_t **input, int size){
    Serial.print("(");
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 16; y++){
                Serial.print(input[x][y]);
        }
    }
    Serial.println(")");
}

void print(int8_t **input, int size){
    Serial.print("(");
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 16; y++){
                Serial.print(input[x][y]);
        }
    }
    Serial.print(")");
}

void fill(int8_t *input, int size, int8_t value){
    for(int i = 0; i < size; i++){
        input[i] = value;
    }
}



#endif