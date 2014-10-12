//set up led pin and order lookup
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


void setup(){                
  //iterate through LED_ORDER, set all leds to output
  for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
          pinMode(LED_ORDER[i], OUTPUT);
  }
}

//turn all leds on
void set_all_high(){
  for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
          digitalWrite(LED_ORDER[i],HIGH);
  }

}


//turn all leds off
void set_all_low(){
  for (int i = 0; i < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); i++){
          digitalWrite(LED_ORDER[i],LOW);
  }
}

//helper function, returns if a particular pin 
//should be on or off, to represent a given 
//binary number
int binary_pins(int num, int pin){
 return (num >> pin) & 1;
}

void loop(){
 set_all_low();
 delay(1000);
 set_all_high();
 delay(1000);

 //for all possible numbers represented by 4 leds
 for (int i = 0; i < 16; i++){
    //iterate throuhg all our leds setting their state according to
    //binary_pins(number to display, which pin)
    for (int j = 0; j < sizeof(LED_ORDER)/sizeof(LED_ORDER[0]); j++){
            int pin_state = binary_pins(i,j);
            digitalWrite(LED_ORDER[j],pin_state);
    }

    delay(500);
 }
}
