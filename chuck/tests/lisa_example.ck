SinOsc s => dac;
s => LiSa loop => dac;

4::second => loop.duration;
now => time start;
now => time end;

fun void pattern1(){
    440 => s.freq;
    1::second => now;
    660 => s.freq;
    1::second => now;
    }


fun void pattern2(){
    880 => s.freq;
    1::second => now;
    330 => s.freq;
    1::second => now;
    }


loop.record(1);
pattern1();
pattern2();
loop.record(0);

s.gain(0);
2::second => now;

//pattern2();
loop.loopStart(0,0::second);
loop.loopEnd(0,2::second);
loop.loopStart(1,2::second);
loop.loopEnd(1,4::second);
loop.play(0,1);
4::second => now;
loop.play(1,1);
2::second => now;
loop.play(0,0);
loop.play(1,0);
1::second => now;

loop.recPos(2::second);
loop.record(1);
s.gain(1);
pattern1();
s.gain(0);
loop.record(0);
loop.play(1,1);

while(true){
    1::second => now;
}

