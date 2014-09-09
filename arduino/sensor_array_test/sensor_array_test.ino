void setup(){
  Serial.begin(9600);
}

void loop(){

  Serial.print(analogRead(A5));
    Serial.print(",");
  Serial.print(analogRead(A4));
    Serial.print(",");
  Serial.print(analogRead(A3));
    Serial.print(",");
  Serial.print(analogRead(A2));
    Serial.print(",");
  Serial.print(analogRead(A1));  
  Serial.print("\n");
  delay(500);
}
