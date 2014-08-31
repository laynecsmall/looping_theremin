
fun void newSound(){
    <<<"Starting new shred">>>;
    SinOsc a => Envelope ee =>  dac;
    ee.keyOn();
    ee.gain(1);

    a.freq(700);
}

1::second => now;
spork ~ newSound();

while(true){
    1::second => now;
}
