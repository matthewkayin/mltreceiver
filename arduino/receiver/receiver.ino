#include <Pixy2.h>
#include <math.h>

int red_sig = 1;
int green_sig = 2;
Pixy2 pixy;

int current = -1;
int count = 0;

bool is_receiving = false;
bool is_broadcasting = false;

bool bits[8];
int bcount = 0;
int gbitcount = 0;

char out_buffer[256];
int out_size = 0;
int chunk_size = 0;

unsigned long before_time;
const unsigned long TIMEOUT = 2000;

void set_current(int value);
void add_block(bool value);
int binary_to_int(bool* bytes);
void send_message();
void broadcast_message();

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

        //Serial.print(blocks);
        if(pixy.ccc.blocks[0].m_signature == red_sig){

            set_current(0);
            count++;

        }else if(pixy.ccc.blocks[0].m_signature == green_sig){

            set_current(1);
            count++;
        }
        //Serial.print("\n");

    }else{

        set_current(-1);
    }

    if(is_receiving && current == -1){

        unsigned long elapsed = millis() - before_time;
        if(elapsed >= TIMEOUT){

            send_message();
        }
    }

    if(is_broadcasting){

        broadcast_message();
    }
}

void set_current(int value){

    if(current != -1 && value == -1){

        before_time = millis();

    }else if(!is_receiving && value != -1){

        out_size = 0;
        chunk_size = 0;
        is_receiving = true;
        is_broadcasting = false;
    }

    if(current != value){

        if(current != -1){

            bool value = false;
            if(current == 1){
              
                value = true;  
            }

            while(count >= 4){

                add_block(value);
                count -= 4;
            }

            while(count >= 3){

                add_block(value);
                count -= 3;
            }
        }

        count = 0;
        current = value;
    }
}

void add_block(bool value){

    bits[bcount] = value;
    bcount++;

    if(bcount == 8){

        bcount = 0;
        out_buffer[out_size] = binary_to_byte(bits);
        out_size++;
    }

    gbitcount++;
}

char binary_to_byte(bool* bits){

    int byte_num = 0;

    for(int i = 0; i < 8; i++){

        int index = 7 - i;
        byte_num += pow(2, i) * (int)bits[i];
    }

    byte_num -= 128;

    return (char)byte_num;
}

void send_message(){

    /*Serial.print("gbc is " );
    Serial.print(gbitcount);
    Serial.print("\n");
    Serial.print("os is " );
    Serial.print(out_size);
    Serial.print("\n");
    Serial.print("bc is " );
    Serial.print(bcount);
    Serial.print("\n");*/
    gbitcount = 0;
    int bitcount = (8 * out_size) + bcount;
    /*Serial.print("bitcount is ");
    Serial.print(bitcount);
    Serial.print("\n");*/
    chunk_size = bitcount / 7;
    if(bcount != 0){

        for(int i = bcount + 1; i < 8; i++){

            bits[i] = 0;
        }

        out_buffer[out_size] = binary_to_byte(bits);
        out_size++;
    }

    is_receiving = false;
    is_broadcasting = true;
}

void broadcast_message(){

    Serial.print((uint8_t)out_size);
    Serial.print((uint8_t)chunk_size);
    for(int i = 0; i < out_size; i++){

        Serial.print(out_buffer[i]);
    }
    Serial.print("\n");
}
