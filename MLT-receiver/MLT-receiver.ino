
  
#include <Pixy2.h>

int red_sig = 1;
int green_sig = 2;
bool is_looping = false;
int counter = 0;
int current = -1;
// This is the main Pixy object 
Pixy2 pixy;

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
  
  pixy.init();
}

void loop()
{ 
  uint16_t blocks; 
  // grab blocks!
  while(1){
    int now = -1;
    
    blocks = pixy.ccc.getBlocks();
    if(blocks){
      counter += blocks;
      
        if(pixy.ccc.blocks[0].m_signature == red_sig){
          //Serial.print(1);
          now = 0;
          is_looping = true;
          }
        else if(pixy.ccc.blocks[0].m_signature == green_sig){
          //Serial.print(0);
          now = 1;
          is_looping = true;
        }
        if(now != current){
          Serial.print(now);
          current = now;
        }
         
    }else{
      if(counter != 0){
        is_looping = false;
      
        //Serial.print("blocks= ");
        //Serial.println(counter);
        counter = 0;
      }
     }
  }
}
