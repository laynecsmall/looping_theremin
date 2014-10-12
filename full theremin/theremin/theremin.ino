//LED array setup
typedef enum LED_PINS{LED_1 = 10, LED_2 = 11, LED_3 = 12, LED_4 = 13, LED_5 = A2, LED_6 = A1};
LED_PINS ledPins;

//we need to be able to look up which pin comes first etc
int LED_ORDER[] = {
   ledPins = LED_2,
   ledPins = LED_3,
   ledPins = LED_4,
   ledPins = LED_5};
//these two are special leds, so get their own define
#define RECORD_LED 10
#define PLAY_LED A1

//button setup
typedef enum BUTTON_PINS{PLAY = 7, VOICE, RECORD};
BUTTON_PINS buttons;

int debounce_delay = 20;
int button_order[] = {buttons = PLAY, buttons = VOICE, buttons = RECORD};
int button_state[] = {LOW,LOW,LOW};
int voice_button_count = 0;
int play_button_count[16];
int record_button_count[16];
int button_prev_state[] = {LOW,LOW,LOW};
int button_last_debounce[] = {0,0,0};

//get check the buttons, and set relevant variables appropriately
void read_buttons(){
    //update the button state, and push old state to button_prev_state
    for (int i = 0; i<sizeof(button_order)/sizeof(button_order[0]); i++){
            button_prev_state[i] = button_state[i];
            button_state[i] = digitalRead(button_order[i]);
    }

    //if the button push is after the debounce period:
    for (int i = 0; i<sizeof(button_order)/sizeof(button_order[0]); i++){
       if (button_prev_state[i] == LOW && button_state[i] == HIGH &&
           millis() - button_last_debounce[i] > debounce_delay){
                   button_last_debounce[i] = millis();
                   //we're after the debounce, update the right voice set's
                   //button status
                   switch(i){
                           case 0:
                                   play_button_count[voice_button_count % 16]++;
                                   break;
                           case 1:
                                   voice_button_count++;
                                   break;
                           case 2:
                                   record_button_count[voice_button_count % 16]++;
                                   break;
                   }
               }
        }
}


//sensor setup
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

//update the sensor values
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

void set_play_led(int state){
        digitalWrite(PLAY_LED, state);

}
void set_rec_led(int state){
        digitalWrite(RECORD_LED, state);

}

//turn all leds on
void set_all_leds_high(){
  for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
          digitalWrite(LED_ORDER[i],HIGH);
  }

}

//turn all leds off
void set_all_leds_low(){
  for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
          digitalWrite(LED_ORDER[i],LOW);
  }
}

void set_leds(int number){
    //iterate throuhg all our leds setting their state according to
    //binary_pins(number to display, which pin)
    for (int j = 0; j < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); j++){
            int pin_state = binary_pins(number,j);
            digitalWrite(LED_ORDER[j],pin_state);
    }
}

void setup(){
    Serial.begin(115200);
    //LED setup
    //iterate through LED_ORDER, set all leds to output
    for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
            pinMode(LED_ORDER[i], OUTPUT);
    }

    //set mux set pins to output mode
    for (int i = 0; i < sensor_count; i++){
            pinMode(ctrl_mux[i],OUTPUT);
    }

    set_all_leds_high();
    calibrate(offsets,thresholds);
    set_all_leds_low();
     
    //button setup
    pinMode(buttons = PLAY,INPUT);
    pinMode(buttons = VOICE,INPUT);
    pinMode(buttons = RECORD,INPUT);
    
    //print calibrated settings
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
    check_sensors(sensor_values, offsets, thresholds);
    int x_val = 0;
    int y_val = 0;

    //figure out which array the value in question belongs to
    //if two sensors are active, only pay attention to the last one
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

    //get the button state

    int play = 0;
    int rec = 0;
    read_buttons();
    //get the button state for this voice
    if (play_button_count[voice_button_count % 16] % 2 == 0){
            play = 0;
            set_play_led(0);
    }
    else{
            play = 1;
            set_play_led(1);
    }
  
    if (record_button_count[voice_button_count % 16] % 2 == 0){
          rec = 0;
          set_rec_led(0);
    }
    else{
          rec = 1;
          set_rec_led(1);
    }
    //set the leds appropriately
    set_leds(voice_button_count % 16);

    //print the sensor values
    Serial.print(x_val);
    Serial.print(":");
    Serial.print(y_val);
    Serial.print(",");
    //print the play button state
    Serial.print(play);
    Serial.print(",");
    //print the voice select number
    Serial.print(voice_button_count % 16);
    Serial.print(",");
    //print the record button state
    Serial.print(rec);
    Serial.print("\n");

    delay(15);
}

