#define sensor_count 8
#define read_mux A5
#define history 20
int ctrl_mux[] = {2,3,4};
int count;

int sensor_values[sensor_count];



//sets the mux pins to read from sensor number given
void set_mux(int sensor){
    for (int j = 0; j < sizeof(ctrl_mux)/sizeof(ctrl_mux[0]); j++){
            digitalWrite(ctrl_mux[j], binary_pins(sensor,j));
    }
}

void check_sensors(int sensor_values[]){
    for (int i = 0; i < sensor_count; i++){
            set_mux(i);

            sensor_values[i] = analogRead(read_mux);
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
	Serial.println("Beginning");

    //set mux set pins to output mode
    for (int i = 0; i < sensor_count; i++){
            pinMode(ctrl_mux[i],OUTPUT);
    }
	Serial.print("Starting main loop");
}


//TODO update this
void loop(){
    check_sensors(sensor_values);

	for (int i = 0; i < sensor_count; i++){
			Serial.print(sensor_values[i]);
			Serial.print(", ");
	}

	Serial.print("\n");

    delay(200);
}


