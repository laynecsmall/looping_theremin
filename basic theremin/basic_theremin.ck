SinOsc s => Envelope e => dac;
SerialIO serial;
string line;
string stringInts[3];
int data[3];

SerialIO.list() @=> string list[];

for(int i; i<list.cap(); i++){
    chout <= i <= ":" <= list[i] <= IO.newline();
}

serial.open(0, SerialIO.B115200, SerialIO.ASCII);

fun void serialPoller(){
    while(true){
        serial.onLine() => now;
        serial.getLine() => line;
        
        
        if (line$Object == null) continue;
    
        0 => stringInts.size;

        if (RegEx.match("([0-9]+),([0-9]+),([0-9]+)", line, stringInts)){
            for (1 => int i; i< stringInts.cap(); i++){
                Std.atoi(stringInts[i]) => data[i-1];
            }
        }

        if (data[0] != 0){
           1000*(1-data[0]/700.0) => float new_freq;
        
           s.freq(new_freq);
        }

        if (data[1] != 0){
           1-data[1]/700.0 => float new_gain;
        
           e.gain(new_gain);
        }
        else e.gain(0);

   }
}

spork ~ serialPoller();
e.keyOn();
while(true){
    1::second => now;
}
