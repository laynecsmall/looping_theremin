#define mux_a 5
#define mux_b 6
#define mux_c 7
#define odd_switch 3
#define mux_out A0

int binary_pins(int num, int pin){
 return (num >> pin) & 1;
}

void set_mux_pins(int num){

  for (int i=mux_a;i<=mux_c;i++){
    int pinstate = binary_pins(num,i-mux_a);
    int pinvalue = i-mux_a;
    
//    Serial.print("Setting: ");
//    Serial.print(num);
//    Serial.print(" Pin value: ");
//    Serial.print(pinvalue);
//    Serial.print("  Pinstate: ");
//    Serial.print(pinstate);
//    Serial.print("\n");

    digitalWrite(i,pinstate);
  }
//  Serial.println("\n");
}

void setup(){
  pinMode(mux_a,OUTPUT);
  pinMode(mux_b,OUTPUT);
  pinMode(mux_c,OUTPUT);
  pinMode(odd_switch,OUTPUT);
  Serial.begin(9600);
  }
  
void loop(){
  for (int i=0;i<5;i++){
    set_mux_pins(i);
    Serial.print(" ");
    Serial.print(i);
    Serial.print(":");
    Serial.print(analogRead(mux_out));
    Serial.print(" ");
  }
  Serial.print("\n");
  delay(500);
}
