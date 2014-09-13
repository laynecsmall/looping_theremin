int binary_pins(int num, int pin){
 return (num >> pin) & 1;
}
void setup(){
  Serial.begin(9600);
}

void loop(){
  for (int i=0;i<8;i++){
    Serial.println(binary_pins(5,i));
  }
  Serial.println("\n");
  delay(5000);
}
