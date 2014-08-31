void setup(){
  Serial.begin(9600);
  pinMode(2,INPUT);
}

void loop(){
  if (digitalRead(2) == HIGH){
    Serial.println("BUTTON HIGH");
  }
  else{
    Serial.println("Button low");
  }
  delay(1000);
}
