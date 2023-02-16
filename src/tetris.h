#include "display.h"
#include "utilities.h"
#define MATRIX_W 8
#define MATRIX_H 16
#define BOXSIZE 8
#define DWAIT 1000
#define ACCEL 100


class Tetris {

    private:
        int points[4] = {40, 100, 300, 1200}; 
        int score = 0;
        uint8_t rotation = 0;
        uint8_t lx = 4;
        uint8_t ly = 0;
        uint8_t id = 0;
        int8_t matrix[MATRIX_W][MATRIX_H] = {0}; // 0 clean; 1 current; 2 solid
        int8_t current_piece[4][2] = {0};
                                                                                        ///   -Y     
        int8_t positions[7][4][4][2] = { //  Ordered from lowest to highest Y value    /// +X  0  -X
            {                                                                          ///    +Y
                {{-2,0},{-1,0},{0,0},{1,0}}, // I
                {{-1,1},{-1,0},{-1,-1},{-1,-2}},
                {{-2,0},{-1,0},{0,0},{1,0}},
                {{0,1},{0,0},{0,-1},{0,-2}}
            },
            {                   
                {{-1,0},{0,0},{1,0},{1,-1}}, // J
                {{0,1},{0,0},{0,-1},{-1,-1}},
                {{-1,1},{-1,0},{0,0},{1,0}},
                {{0,1},{1,1},{0,0},{0,-1}}
            },
                {
                {{-1,0},{0,0},{1,0},{-1,-1}}, // L
                {{0,1},{0,0},{0,-1},{1,-1}},
                {{1,1},{-1,0},{0,0},{1,0}},
                {{-1,1},{0,1},{0,0},{0,-1}}
            },
            {
                {{-1,0},{0,0},{0,-1},{-1,-1}}, // O
                {{-1,0},{0,0},{0,-1},{-1,-1}},
                {{-1,0},{0,0},{0,-1},{-1,-1}},
                {{-1,0},{0,0},{0,-1},{-1,-1}}
            },
            {
                {{1,0},{0,0},{0,-1},{-1,-1}}, // S
                {{-1,1},{0,0},{-1,0},{0,-1}},
                {{0,1},{1,1},{0,0},{-1,0}},
                {{0,1},{0,0},{1,0},{1,-1}}
            },
            {
                {{-1,0},{0,0},{1,0},{0,-1}}, // T
                {{0,1},{-1,0},{0,0},{0,-1}},
                {{0,1},{-1,0},{0,0},{1,0}},
                {{0,1},{1,0},{0,0},{0,-1}}
            },
            {
                {{0,0},{-1,0},{1,-1},{0,-1}}, // Z
                {{0,1},{0,0},{-1,0},{-1,-1}},
                {{0,1},{-1,1},{1,0},{0,0}},
                {{1,1},{1,0},{0,0},{0,-1}}
            }
        };

        void cleanMatrix(){
            for(uint8_t x = 0; x < MATRIX_W; x++){
                for(uint8_t y = 0; y < MATRIX_H; y--){
                        matrix[x][y] = 0;
                }
            }
        }

        void printcoll(int8_t xv,int8_t yv){
            int8_t x;
            int8_t y;
            String r = "Rotation: [";
            String v = "Collision: [";


            for(uint8_t i = 0; i < 4; i++){
                x = positions[id][rotation][i][0] + lx + xv;
                y = positions[id][rotation][i][1] + ly + yv;

                r += "(" + String(positions[id][rotation][i][0]) + "," + String(positions[id][rotation][i][1]) + ")";
                v += "(" + String(x) + "," + String(y) + ")";
            }
            v += "]"; 
            r += "]"; 
            Serial.println("(lx,ly): ("+String(lx)+"," + String(ly)+")");
            Serial.println("(xv,yv): ("+String(xv)+"," + String(yv)+")");
            Serial.println(r);
            Serial.println(v);
            printMatrix();


        }        

        bool check_colision(uint8_t xv, uint8_t yv){
            //Check collisions 

            int8_t x;
            int8_t y;

            //printcoll(xv,yv);

            for(uint8_t i = 0; i < 4; i++){
                
                x = positions[id][rotation][i][0] + lx + xv;
                y = positions[id][rotation][i][1] + ly + yv;

                if(y >= 0){
                    if(y >= MATRIX_H || x >= MATRIX_W || x < 0 || matrix[x][y] == 2){
                        return 1; // Get out if collision is detected
                    }
                }
            }

            return 0;
        }

        void rotate(uint8_t id){

            int8_t x;
            int8_t y;

            rotation++;

            if (rotation > 3){
                rotation = 0;
            }
            
            if(check_colision(0,0)){ // Check rotations
                
                if (rotation > 0){
                    rotation--;
                }
                return;
            }

            for(uint8_t i = 0; i < 4; i++){ // Clean previous position, needs to be done before to prevent next clean to remove previous update 
            
                if(current_piece[i][1] >= 0){
                    matrix[current_piece[i][0]][current_piece[i][1]] = 0; 
                }
            }

            for(uint8_t i = 0; i < 4; i++){ // for each piece 

                x = positions[id][rotation][i][0];
                y = positions[id][rotation][i][1];

                current_piece[i][0] = lx + x; //Store rotation plus relative position
                current_piece[i][1] = ly + y;
                
                if(ly + y >= 0){
                    matrix[current_piece[i][0]][current_piece[i][1]] = 1;
                }

            }

        }

        void moveX(uint8_t xm){
            //Serial.println("xm: ");
            if(!check_colision(xm,0)){

                //Serial.println(xm);

                for(uint8_t i = 0; i < 4; i++){ // Clean previous position, needs to be done before to prevent next clean to remove previous update 
                    if(current_piece[i][1] >= 0){
                        matrix[current_piece[i][0]][current_piece[i][1]] = 0; 
                    }
                }

                for(uint8_t i = 0; i < 4; i++){ // for each piece 

                    current_piece[i][0] += xm; //Store rotation plus relative position
                    
                    if(current_piece[i][1] >= 0){
                        matrix[current_piece[i][0]][current_piece[i][1]] = 1;
                    }

                }
                lx += xm;
            }

        }

        void add_piece(uint8_t iid, int8_t fx = 0, int8_t fy = 0){
            id = iid;

            for(int i = 0; i < 4; i++){
                current_piece[i][0] = -1;
                current_piece[i][1] = -1;
            }

            lx = 4+fx; // Store pivot position
            ly = fy; // Store pivot position

            for(uint8_t i = 0; i < 4; i++){

                int8_t x = positions[id][rotation][i][0];
                int8_t y = positions[id][rotation][i][1];

                current_piece[i][0] = 4+x+fx;
                current_piece[i][1] = y+fy;
                
                if(y+fy >= 0){
                    matrix[4+x+fx][y+fy] = 1;
                }
            }   

        }

        bool isLineComplete(uint8_t y){
            for (uint8_t x = 0; x < uint8_t(8); x++){ //Checks if all the values in line Y are != 0
                if (matrix[x][y] < 1){
                    return false;
                }
            }
            return true;
        }

        void remove_line(uint8_t cy){

            for(uint8_t x = 0; x < uint8_t(8); x++){
                for(int8_t y = cy; y > int8_t(0); y--){
                        matrix[x][y] = matrix[x][y-1];
                }
                matrix[x][0] = 0; // Clean first line, as previous first line has been moved down 
            }
        }

        void check_complete_lines(int8_t min_y){
            
            int8_t top = 0; 
            uint8_t ipoints = 0;
            for (int8_t i = min_y; i > top; i--){
                if (isLineComplete(i)){
                    score += points[ipoints]; 
                    remove_line(i);
                    i++;
                    ipoints++;
                    drawScreen();
                }
            }

        }

        uint8_t moveY(){

            uint8_t collision = check_colision(0,1);

            if(collision == 0){ // If didn't colided move and store new positions
                for(uint8_t i = 0; i < 4; i++){

                    int8_t x = current_piece[i][0];
                    int8_t y = current_piece[i][1];

                    if (y+1 >= 0){ // Upload to matrix only if it will visible
                        matrix[x][y+1] = 1; 
                        if(y >= 0){
                            matrix[x][y] = 0;
                        }
                    }

                    current_piece[i][0] = x;
                    current_piece[i][1] = y+1;
                }

                ly += 1;

            }else{ //If collided store previous collision as final
                //fill(y_pos,4,-1);
                //y_pos_index = 0;
                int8_t max_y = -1;            

                for(uint8_t i = 0; i < 4; i++){ // For each piece 
                    int8_t x = current_piece[i][0];
                    int8_t y = current_piece[i][1];

                    if(ly < 1){ //If piece collided at the Y=0 return -2 to trigger game over;
                        return 2;
                    }

                    matrix[x][y] = 2;
                    max_y = y > max_y ? y : max_y;
                }
                check_complete_lines(max_y);
            }
            return collision; 
        }

        void printMatrix(){

            Serial.println("");
            Serial.println("");
            Serial.println("    7 6 5 4 3 2 1 0");

            for (uint8_t y = 0; y < MATRIX_H; y++){
                
                Serial.print(y);
                
                if(y < 10){
                    Serial.print(' '); 
                }

                Serial.print(" [");
                for (int8_t x = 7; x >= 0; x--){
                    
                    if (x < 7){
                        Serial.print(" ");
                    }
                    Serial.print(matrix[x][y]);
                }
                Serial.println("]");
            }
            Serial.println("");
            Serial.println("");
            Serial.println("------------------");
        }

        void drawScreen(){

            display.clearDisplay();

            for(uint8_t x = 0; x < MATRIX_W; x++){
                for(uint8_t y = 0; y < MATRIX_H; y++){
                    if(matrix[x][y] > 0){
                        drawBox(y*BOXSIZE,x*BOXSIZE,true);
                    }
                }
            }
            draw_grid();
            display.display();
        }

        void drawBox(uint8_t x, uint8_t y, bool fill){
            drawBox(x,y,BOXSIZE, fill);
        };

        void drawBox(uint8_t x, uint8_t y, uint8_t size, bool fill){
            if (!fill){
                display.drawRect(x, y, size, size, SSD1306_WHITE);
            }
            else{
                display.fillRect(x, y, size, size, SSD1306_WHITE);
            }
            
        };

        void draw_grid(){
            for (uint8_t y = 0; y < SCREEN_HEIGHT; y+=BOXSIZE){
                for (uint8_t x = 0; x < SCREEN_WIDTH; x+=BOXSIZE){
                    drawBox(x,y,false);
                }
            }
        }

        void draw_gameover(){
            
            for(uint8_t x = 0; x < 4; x++){
                display.clearDisplay();
                draw_grid();
                display.display();
                delay(25);
                display.clearDisplay();
                display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
                display.display();
                delay(50);
            }
                        
            lx = 4;
            ly = 0;

            clearMatrix();
            rotation = 0;
        }
   
        void clearMatrix(){

            for(uint8_t x = 0; x < MATRIX_W; x++){
                for(uint8_t y = 0; y < MATRIX_H; y++){
                        matrix[x][y] = 0;
                    }
            };
        }
   
    public:
        void start_game(){
            uint8_t status = 0;
            int waitTime = DWAIT;
            unsigned long ntime = 0;
            uint8_t btn[4] = {1,1,1,1};
            uint8_t pbtn[4] = {1,1,1,1};    
            uint8_t piecesCounter = 0;

            draw_grid();

            while(status < 2){

                id = random(6);
                add_piece(id,0,0); 
                piecesCounter++;
                if(piecesCounter > 4){
                    piecesCounter = 0; 
                    
                    if((waitTime -= ACCEL) >= 0){
                        waitTime -= ACCEL;
                    }
                }

                status = 0;
                while (status < 1)
                {
                    if (ntime < millis()){
                        status = moveY();
                        ntime = millis() + waitTime;
                    }
                    else{

                        btn[0] = digitalRead(IPIN0);
                        btn[1] = digitalRead(IPIN1);
                        btn[2] = digitalRead(IPIN2);
                        btn[3] = digitalRead(IPIN3);

                        if(btn[0] == 0 && pbtn[0] == 1){
                            rotate(id);
                        }

                        if(btn[1] == 0){
                            moveX(1);
                        }
                        
                        if(btn[2] == 0){
                            moveX(-1);
                        }

                        if(btn[3] == 0){
                            status = moveY();
                            ntime = millis() + waitTime;
                        }
                        drawScreen();

                        pbtn[0] = btn[0];
                        pbtn[1] = btn[1];
                        pbtn[2] = btn[2];
                        pbtn[3] = btn[3];

                    }
                    drawScreen();
                }
            }
            draw_gameover();
        }

};

