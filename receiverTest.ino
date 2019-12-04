int count = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  /*while(!Serial){
    ;
  }*/
}

void loop() {
  
  String buffer = "1010101";
  if(Serial){
    for(int i = 0; i < buffer.length(); i++){
      Serial.write(buffer[i]);
    }
  
    Serial.write('\n');
  
  }
}
