//define needed variables
int buttonPin = 2;
int pressCounter = 0;
int state = LOW;
int prevState = LOW;

int lastDebounceTime = 0;
int debounceDelay = 20;

void setup(){
  //set to highest availible baud rate (250000 not supported?)
  //needed to reduce delay. 
  Serial.begin(115200);
  pinMode(buttonPin,INPUT);
}

void loop(){
  //get the sensor values
  int l = analogRead(A5);
  int r = analogRead(A4);
  
  //save the previous state
  prevState = state;
  state = digitalRead(buttonPin);
  
  //only increment the count if we're going from low to high
  //also, debouncing
  if (prevState == LOW && state == HIGH && millis() - lastDebounceTime > debounceDelay){ 
    pressCounter++;
    lastDebounceTime = millis();
  }

  //print the output to serial
  Serial.print(l);
  Serial.print(",");
  Serial.print(r);
  Serial.print(",");
  Serial.print(pressCounter % 16);
  Serial.print("\n");

  //delay is going to need to be tweaked in final version
  delay(100);
}

