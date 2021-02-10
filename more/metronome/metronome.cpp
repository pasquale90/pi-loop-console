#include <iostream>
#include "metronome.h"
#include <ctime>

//default constructor
metronome::metronome()
{
    rythm.prologue=4;
    rythm.epilogue=0;
    rythm.intonation=1;
    tempo=120;
    std::cout<<"Creating a metronome..Default rythm pattern 4/4 initialized, with tempo set to 120 bpm"<<std::endl;
    //std::cout<<"Now it's about to define tempo aswell!"<<std::endl;
}

metronome::metronome(float bpm){
    tempo=bpm;
    std::cout<<"Metronome defined.."<<std::endl;
}

metronome::~metronome()
{
    std::cout<<"Metronome destroyed";
}

void metronome::start_timing(){
    present = steady_clock::now();
}

void metronome::stop_timing(){
    future = steady_clock::now();
}

void metronome::set_tempo(){
    beep_span = duration_cast<duration<double>>(future-present);
    tempo = 60/beep_span.count();//converts period to bpm
}

void metronome::set_tempo(float bpm){
    tempo=bpm;
}

float metronome::get_tempo(){
    return tempo;
}

void metronome::set_rythm(int a,int b){
    rythm.prologue=a;
    rythm.epilogue=b;
}

time_signature metronome::get_rythm(){
    return rythm;
}
/*
int metronome::measure(){
    return (rythm.rythmpart_a+rythm.rythmpart_b);
}*/

void metronome::run()
{
   std::cout<<"Application running.."<<std::endl;
   std::cout<<"tempo "<<tempo<<", beep_span "<<beep_span.count()<<std::endl;


   int ms_beep=(int) (beep_span.count()*1000);
   present = std::chrono::steady_clock::now();
   future = present + std::chrono::milliseconds(ms_beep);
   auto d=duration_cast<duration<double>>(future-present);
   int counter=0;
   while(d.count() && !pause){
        present = std::chrono::steady_clock::now();
        d=duration_cast<duration<double>>(future-present);
        if(d.count()<0.000001){
                future = present + std::chrono::milliseconds(ms_beep);
                ///std::cout<<"DCOUNT "<<d.count()<<std::endl;
                ///std::cout<<"beep_span.count() "<<beep_span.count()<<std::endl;
                ///std::cout<<"ms_beep "<<ms_beep<<std::endl;
                rythm.intonation=counter%rythm.prologue;
                if (!rythm.intonation){
                   std::cout<<std::endl<<"tick"<<std::endl;
                }else{
                    std::cout<<"tock"<<std::endl;}
                if (counter%15==14){
                    pause_metro();
                }
                ++counter;

        }
   }//stop()?
}

void metronome::pause_metro(){
    pause=true;
    std::cout<<"metronome paused"<<std::endl;
}

