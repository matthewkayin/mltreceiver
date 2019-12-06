
 #include <Pixy2.h>

int red_sig = 1;
int green_sig = 2;
Pixy2 pixy;

int current = -1;
int count = 0;

void setup(){

    Serial.begin(9600);
    //Serial.print("Starting...");

    pixy.init();
}

void loop(){

    uint16_t blocks;
    int now = -1;

    blocks = pixy.ccc.getBlocks();
    if(blocks){

        if(pixy.ccc.blocks[0].m_signature == red_sig){

            if(current != 0){

                current = 0;
                count = 0;
            }
            count++;

        }else if(pixy.ccc.blocks[0].m_signature == green_sig){

            if(current != 1){

                current = 1;
                count = 0;
            }
            count++;
        }

    }else{

        current = -1;
        count = 0;
    }

    if(count >= 60){

        Serial.print(current);
        count = 0;
    }
}
