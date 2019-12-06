#include <Pixy2.h>
#inlcude <math.h>

bool bits[8];
int bcount = 0;

int red_sig = 1;
int green_sig = 2;

bool recieve = false;
bool ready = false;
char out_buffer[256];
int out_size = 0;
int chunk_size = 0;
unsigned long before_time;
unsigned long elapsed;
unsigned long timeout;

int current = -1;
int count = 0;

Pixy2 pixy;

int trans(bool bits[]){
  int num = 0;
  int base = 2;
  for(){
    num = num + (bits[i] * pow(base, (7-1)));
  }
  return num;
}

void steup(){
  Serial.begin(9600);
  Serial.print("Starting...\n");
  
  pix.init();
  before_time = millis();
}

void loop(){
  uinit16_t blocks;
  int now = -1;
  elapsed = millis() - before_time();
  blocks = pixy.ccc.getBlocks();
  
  if(pixy.ccc.blocks[0].m_signature == red_sig){
    if(current != 0){
      current = 0;
      count = 0;
      recieve = true;
    }
    count++;
  }else if(pixy.ccc.blocks[0].m_signature == green_sig){
    if(current != 1){
      current = 1;
      count = 0;
      recieve = true;
    }
    count++;
  }else{
    current = -1;
    count = 0;
  }
  
  if(count >= 60){
    bits[bcount] = current;
    bcount++;
    count = 0;
  }
  
  if(elapsed >= timeout){
    int bitcount = (8*out_size) + bcount;
    chunk_size = bitcount/7;
    if{bcount !=0){
      for(int i = 0; i < 7;i++){
        if(bits[i] == 0 || bits[i] == 1){
        {
          //do nothing
        }else{
          bits[i] = 0;
        }
        }
      }
      out_buffer[out_size] = trans(bits);
      out_size++;
    }
    ready = true;
  }
  if(recieve){
    before_time = millis();
    ready = true;
    out_size = 0;
    chunk_size = 0;
    ready = false;
    
    if(bcount == 7){
      bcount = 0;
      out_buffer[out_size] = trans(bits);
      out_size++;
    }
  }
  if(ready){
    Serial.print(out_size, chunk_size, out_buffer);
    recieve = false;
  }
}
