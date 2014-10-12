#define sensor_count 8
#define read_mux A5
int ctrl_mux[] = {2,3,4};

//arrays for info about sensors
//(offset to make input close to zero
//and threshhold over which input is non-zero
int offsets[sensor_count];
int thresholds[sensor_count];
int sensor_values[sensor_count];

int calibration_samples = 40;

//sensors are grouped like this
int y_sensor[] = {7,2,1,0};
int x_sensor[] = {3,6,5,4};

//for each sensor find out what the average 'zero' value and variance is.
//set the offset as the average, and threshold as 3*std deviation.
//threshold is the value above which is zero
void calibrate(int *offsets,  int *thresholds){
  //naive variance algorithm taken from http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
  
  //for each of the pins to calibrate
  for (int i=0;i < sensor_count; i++){
    int values[calibration_samples];
    //collect samples
    for (int j=0;j<calibration_samples-1;j++){
      delay(10);
      
      //set the mux to read from the correct sensor
      set_mux(i);
      values[j] = analogRead(read_mux);
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

//sets the mux pins to read from sensor number given
void set_mux(int sensor){
    for (int j = 0; j < sizeof(ctrl_mux)/sizeof(ctrl_mux[0]); j++){
            digitalWrite(ctrl_mux[j], binary_pins(sensor,j));
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

void check_sensors(int *sensor_values,int  *offsets,int  *thresholds){
    for (int i = 0; i < sensor_count; i++){
            set_mux(i);
            sensor_values[i] = is_zero(analogRead(read_mux) - offsets[i], thresholds[i]);
    }
}


//helper function, returns if a particular pin 
//should be on or off, to represent a given 
//binary number
int binary_pins(int num, int pin){
 return (num >> pin) & 1;
}

void setup(){
    Serial.begin(115200);

    //set mux set pins to output mode
    for (int i = 0; i < sensor_count; i++){
            pinMode(ctrl_mux[i],OUTPUT);
    }

    calibrate(offsets,thresholds);
    
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


//TODO update this
void loop(){
    check_sensors(sensor_values, offsets, thresholds);
    int x_val = 0;
    int y_val = 0;

    for (int i = 0; i< sizeof(x_sensor)/sizeof(x_sensor[0]); i++){
            if (sensor_values[x_sensor[i]] != 0){
                    x_val = sensor_values[x_sensor[i]]; 
            }
    }

    for (int i = 0; i< sizeof(y_sensor)/sizeof(y_sensor[0]); i++){
            if (sensor_values[y_sensor[i]] != 0){
                    y_val = sensor_values[y_sensor[i]]; 
            }
    }

    Serial.print(x_val);
    Serial.print(":");
    Serial.print(y_val);
    Serial.print("\n");

    delay(500);
}


