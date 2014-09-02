//define needed variables

//button setup
int buttonPin = 2;
int pressCounter = 0;
int state = LOW;
int prevState = LOW;
int lastDebounceTime = 0;
int debounceDelay = 20;

//sensor setup
int pins[] = {A5,A4};
//will be overwritten in setup
int sensor_count = 0;
int calibration_samples = 40;

//arrays for info about sensors
//(offset to make input close to zero
//and threshhold over which input is non-zero

//maximum size of ten, because we'll never need more sensors than that
int offsets[10];
int thresholds[10];
int values[10];


//for each sensor find out what the average 'zero' value and variance is.
//set the offset as the average, and threshold as 3*std deviation.
//threshold is the value above which is zero
void calibrate(int pins[], int *offsets,  int *thresholds){
  //naive variance algorithm taken from http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
  int sensor_count = sizeof(pins);
  //for each of the pins to calibrate
  for (int i=0;i < sensor_count; i++){
    int values[calibration_samples];
    //collect samples
    for (int j=0;j<calibration_samples-1;j++){
      delay(40);
      values[j] = analogRead(pins[i]);
    }
    
    //find the sum and squared sum
    float total = 0;
    float total_square = 0;
    for (int j = 0; j < calibration_samples-1;j++){
      total += values[j];
      total_square += pow(values[j],2);
    }
    //average samples
    int average = total / calibration_samples;
    //find the variance
    int variance = (total_square - pow(total,2)/calibration_samples)/(calibration_samples-1);
    
    //set the offset value, increment the pointer
    offsets[i] = average;
    
    //set the threshhold at three std devations
    //then increment the pointer
    thresholds[i] = 3 * sqrt(variance);  
  }  
}

//checks to see if value is within the threshold, if it is, return 0 otherwise return the value
int is_zero(int value,int threshold){
  if (value < 0){
    return 0;
  }
  if (value < threshold){
    return 0;
  }
  else{
    return value;
  }
}
void setup(){
  //set to highest availible baud rate (250000 not supported?)
  //needed to reduce delay. 
  Serial.begin(115200);
  pinMode(buttonPin,INPUT);

  //resize everything based on the number of sensors we're working with
  sensor_count = sizeof(pins)/sizeof(pins[0]);
 
  //begin the caliration
  calibrate(pins,offsets,thresholds);

  //print settings
  Serial.print("Sensor count: ");
  Serial.print(sensor_count);
  Serial.print("\n\n");
  
  for(int i = 0;i<sensor_count;i++){
    Serial.print("Offset: ");
    Serial.print(offsets[i]);
    Serial.print(" Threshold: ");
    Serial.print(thresholds[i]);
    Serial.print(" \n");
  }
  Serial.print("\n\n");
  //delay(5000);
}


void loop(){
  //get the sensor values
  //offset and threshold found in calibration
  for (int i=0;i<sensor_count;i++){
    values[i] = is_zero(analogRead(pins[i]) - offsets[i],thresholds[i]);
  }

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
  //button press is always last
  for (int i=0;i<sensor_count;i++){
    Serial.print(values[i]);
    Serial.print(",");
  }
  Serial.print(pressCounter % 16);
  Serial.print("\n");

  //delay is going to need to be tweaked in final version
  delay(15);
}

