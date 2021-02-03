// author: stf

void setup() {
        Serial.begin(115200);
        Serial.print("\n\n");
  
        Serial.print("datatype      size\n");
        Serial.print("int           "); Serial.println(sizeof(int));
        Serial.print("int           "); Serial.println(sizeof(float));
        Serial.print("int           "); Serial.println(sizeof(char));
        Serial.print("int           "); Serial.println(sizeof(uint16_t));
        Serial.print("int           "); Serial.println(sizeof(bool));
}

void loop() {

}
