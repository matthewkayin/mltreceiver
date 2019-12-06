void setup(){

    Serial.begin(9600);
    delay(1000);
}

void loop(){

    //if(Serial.available() == 1){

        //int ack = Serial.read();
    char* message = (char*)"Hello friends\n";
    Serial.write(message, 14);
    //}
}
