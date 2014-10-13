//sound chain
SinOsc s => Envelope e => dac;
s => LiSa loop => dac;

//we need 16*30 seconds of memory for all availible voices
480::second => loop.duration;
16 => loop.maxVoices;

//parsing variables
string line;
string data[5];

//input state variables
string xy[3];
float x_samples[15];
float y_samples[15];
float x_out,y_out;
int buttons[3];
int current_voice;
loop_voice loop_voices[16];


class loop_voice
{
    0 => int voice;
    0::ms => dur loop_start;
    0::ms => dur loop_duration;

    0 => int play_button_state;
    0 => int rec_button_state;
    0 => int play_state;
    0 => int rec_state;
    0 => int record_count;

    now => time rec_start;
    now => time rec_end;
    
    fun void set_voice(int set_voice){
        set_voice => voice;
        get_loop_start(set_voice) => loop_start;
        loop.loopStart(voice,loop_start);
    }

    fun void record_start(){
        loop.play(voice,0);
        1 => rec_state;
        loop.recPos(loop_start);
        now => rec_start;
        loop.record(1);
        record_count +1 => record_count;

        spork ~ record_timeout(voice, record_count);
        //TODO spawn loop record resetter, to prevent overwriting other loops
    }

    fun void record_end(){
        loop.record(0);
        0 => rec_state;
        now => rec_end;
        rec_end - rec_start => loop_duration;
        loop.loopEnd(voice,loop_start + loop_duration);
    }

    fun void play_start(){
        loop.playPos(voice,loop_start);
        loop.voiceGain(voice,1);
        loop.play(voice,1);
        1 => play_state;
    }

    fun void play_stop(){
        loop.voiceGain(voice,0);
        0 => play_state;
        loop.loop(voice, 0);
    }

    fun dur get_loop_start(int voice){
        return voice*30::second;
    }

}

fun void record_timeout(int voice,int record_count){
    30::second => now;
    //if record hasn't been turned off for this voice after 30 seconds, turn it off
    if (loop_voices[voice].rec_state == 1 && loop_voices[voice].record_count == record_count){
        loop_voices[voice].record_end();
    }
}

//init voice objects
for (0 => int i; i < loop_voices.cap(); i++){
    loop_voice lv;
    lv.set_voice(i);
    lv @=> loop_voices[i];
}

//select the correct serial device
SerialIO serial;
SerialIO.list() @=> string list[];

for(int i; i<list.cap(); i++){
    chout <= i <= ":" <= list[i] <= IO.newline();
}

//open the serial terminal
serial.open(0, SerialIO.B115200, SerialIO.ASCII);



//checks the serial connection for state
fun void serialPoller(){
    while(true){
        serial.onLine() => now;
        serial.getLine() => line;
        
        if (line$Object == null) continue;
    
        //get the x:y,button1,button2,button3 state
        if (RegEx.match("([0-9]+:[0-9]+),([0-9]+),([0-9]+),([0-9]+)", line, data)){
            data[1] => string xy_line;
            //set input state variables

            RegEx.match("([0-9]+):([0-9]+).*", xy_line, xy);

            Std.atoi(data[2]) => buttons[0];
            Std.atoi(data[3]) => buttons[1];
            Std.atoi(data[4]) => buttons[2];
        }
   }
}

fun void handle_buttons(){
    //if we've switched away from the current voice, make sure record is turned off
    if (buttons[1] != current_voice){
        loop_voices[current_voice].record_end();
        buttons[1] => current_voice;
    }

    //toggle play state for current voice
    if (buttons[0] != loop_voices[current_voice].play_button_state){
        if (loop_voices[current_voice].play_state == 1){
            loop_voices[current_voice].play_stop();
            buttons[0] => loop_voices[current_voice].play_button_state;
            <<<"Stopping playback for voice ",current_voice>>>;
        }
        else{
            loop_voices[current_voice].play_start();
            buttons[0] => loop_voices[current_voice].play_button_state;
            <<<"Starting playback for voice ",current_voice>>>;
        }
    }

    //toggle record state for current voice
    if (buttons[2] != loop_voices[current_voice].rec_button_state){
        if (loop_voices[current_voice].rec_state == 1){
            loop_voices[current_voice].record_end();
            buttons[2] => loop_voices[current_voice].rec_button_state;
            <<<"","Stopping recording for voice ", current_voice>>>;
        }
        else{
            loop_voices[current_voice].record_start();
            buttons[2] => loop_voices[current_voice].rec_button_state;
            <<<"","Starting recording for voice", current_voice>>>;
        }
    }
}

fun void updateOutput(){
    while (true){
        for (0 => int i; i < x_samples.cap(); i++){
                //find the proportion of the max our input gives us
                //multiply that proportion with the total freq range
                Std.atof(xy[1]) => x_samples[i];
                Std.atof(xy[2]) => y_samples[i];


                //moving average
                float x,y;
                for (0 => int j; j < x_samples.cap(); j++){
                    x_samples[j] + x => x;
                }
                for (0 => int j; j < y_samples.cap(); j++){
                    y_samples[j] + y => y;
                }
                x/x_samples.cap() => x;
                y/y_samples.cap() => y;
                    
                //stop playing sound unless we've got input
                //TODO this will have to go
                if (x != 0) e.keyOn();
                else e.keyOff();

                1- (x/1024) * 3000 => x_out;

                //gain is 0-1, so no need to set the total range
                 1 - y/1024 => y_out;

                x_out => s.freq;
                y_out => e.gain;

                //toggle voice play/record as needed
                handle_buttons();

                //wait 2 ms
                2::ms => now;
        }
    }
}

spork ~ serialPoller();
spork ~ updateOutput();

while(true){
    1::second => now;
}
