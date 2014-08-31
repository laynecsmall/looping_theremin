SinOsc s => dac;
s.freq(330);

fun void switcher(){
    SinOsc r => dac;
    while (true){
    r.freq(440);
    2::second => now;
    r.freq(660);
    2::second => now;
    }
}

spork ~ switcher();

while(true){
    1::second => now;
}

