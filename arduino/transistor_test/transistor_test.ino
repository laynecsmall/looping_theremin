void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  
}

void loop(){
  if (digitalRead(2) == HIGH)
  {
    digitalWrite(3, HIGH);
    Serial.println("button high");
  }
  else{
    digitalWrite(3,LOW);
    Serial.println("button LOW");
  }
  delay(100);
}
