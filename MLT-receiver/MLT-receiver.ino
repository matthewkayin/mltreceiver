#include <Pixy2.h>

/****mychanges******/
bool bits[8];
int bcount = 0;
/******mychanges***/

int red_sig = 1;
int green_sig = 2;
Pixy2 pixy;

int current = -1;
int count = 0;

/*****mychanges******/
void trans(bool bits[]){
  char ch;
  int num = 0;
  int base = 2;
  for(int i = 7; i >= 0; i--){
    num = num + (bits[i] * (pow(base, (7-i))));
  }
  ch = num;
  Serial.print(ch);
}
/*******mychanges********/

void setup(){

    Serial.begin(9600);
    Serial.print("Starting...\n");

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
    /******my changes*****/
    if(count >= 60){
        bits[bcount] = current;
        //Serial.print(current);  
        count = 0;
    }

    if(bcount == 7){
      trans(bits);
      bcount = 0;
      bits = {};
    }
    /******mychanges***********/
}
