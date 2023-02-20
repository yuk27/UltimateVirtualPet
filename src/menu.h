#include "display.h"
#include "utilities.h"

class Menu {
    public:

        Menu(){            
            display.clearDisplay();
            display.setTextWrap(true);
            display.setTextColor(SSD1306_WHITE); // Draw white text
        }

        void write(const char *text, uint8_t rotation, bool centered = false, uint8_t size = 1, uint8_t cx = 0, uint8_t cy = 0){

            if(!centered){
                if(rotation != 0){
                    display.setRotation(rotation);
                }
                display.setTextSize(size);      // Normal 1:1 pixel scale
                display.setCursor(cx, cy);     // Start at top-left corner
                display.cp437(true);         // Use full 256 char 'Code Page 437' font
                display.write(text);
                display.display();

                if(rotation != 0){
                    display.setRotation(0);
                }
            }
            else{
                center_draw(text, size);
            }
        }

        void setScore(int id, int score){

            int highScore = EEPROM.read(id);
            String s = String(score);
            const char *scoreText = s.c_str();
            String hs = String(highScore);

            if (score > highScore){
                write("High score!", 3, false, 1, 0, 8);
                EEPROM.write(0, score);
            }else{
                write("Score: ", 3, true);
            }

            write(scoreText, 3,true,1);

        }

        private:
            void center_draw(const char *text, uint8_t size = 1) {
                int16_t x1;
                int16_t y1;
                uint16_t width;
                uint16_t height;
                display.setTextSize(size); 
                display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
                display.clearDisplay(); // clear display

                display.setCursor((SCREEN_HEIGHT - height) / 2, (SCREEN_WIDTH - width) / 2);
                display.println(text); // text to display
                display.display();
                
            }
};
