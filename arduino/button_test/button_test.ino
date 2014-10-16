typedef enum BUTTON_PINS{PLAY = 7, VOICE, RECORD};
BUTTON_PINS buttons;

int debounce_delay = 20;
int button_order[] = {buttons = PLAY, buttons = VOICE, buttons = RECORD};
int button_state[] = {LOW,LOW,LOW};
int button_count[] = {0,0,0};
int button_prev_state[] = {LOW,LOW,LOW};
int button_last_debounce[] = {0,0,0};

void read_buttons(){
    for (int i = 0; i<sizeof(button_order)/sizeof(button_order[0]); i++){
            button_prev_state[i] = button_state[i];
            button_state[i] = digitalRead(button_order[i]);
    }

    for (int i = 0; i<sizeof(button_order)/sizeof(button_order[0]); i++){
       if (button_prev_state[i] == LOW && button_state[i] == HIGH &&
           millis() - button_last_debounce[i] > debounce_delay){
                   button_last_debounce[i] = millis();
                   button_count[i]++;
               }
        }
}

void setup(){
  Serial.begin(115200);
  pinMode(buttons = PLAY,INPUT);
  pinMode(buttons = VOICE,INPUT);
  pinMode(buttons = RECORD,INPUT);
}

void loop(){
  read_buttons();
  if (button_count[0] % 2 == 0){
          Serial.print(0);
  }
  else{
          Serial.print(1);
  }

  Serial.print(",");
  Serial.print(button_count[1]);
  Serial.print(",");
  if (button_count[2] % 2 == 0){
          Serial.print(0);
  }
  else{
          Serial.print(1);
  }
  Serial.print("\n");
  delay(100);
}
