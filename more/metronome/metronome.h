/*
Abstract Class Metronome
Funcionallity:
*/
#ifndef METRONOME_H
#define METRONOME_H

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::seconds;

struct time_signature//fix logic
{
    int prologue;
    int epilogue;
    int intonation;
};

//Abstract
class metronome
{
public:
    metronome();
    metronome(float bpm);
    ~metronome();//destructor
    //duration<double> get_bpm();//returns beeps per sec (bpm)
    steady_clock::time_point present;
    steady_clock::time_point future;
    duration<double> beep_span;
    //duration<double> time_span;
    void start_timing();
    void stop_timing();

    void set_tempo(float bpm);
    void set_tempo();
    float get_tempo();

    void set_rythm(int a,int b);
    time_signature get_rythm();
    void pause_metro();

    //virtual void trigger(void)=0;
    //virtual float to_bpm(duration<double> time_span)=0;
    //virtual high_resolution_clock::time_point beep(void)=0;
    void run();//runs the metronome USAGE:


private:
    //float bpm;
    float tempo;//beats per second
    time_signature rythm;//rythm part_a & rythmpart_b eg. for rythm 4/4.
    int measure;//is consisted of prologue+epilogue segments, measured in beeps as a manner of time period metric,
                                                                //also potentially convertable to #samples metric
                                                                //#THIS WILL BE MOVED TO LOOPER CLASS
    bool pause=false;
};

#endif
