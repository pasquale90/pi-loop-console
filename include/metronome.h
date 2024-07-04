#ifndef METRONOME_H_INCLUDED
#define METRONOME_H_INCLUDED

#include <chrono>
#include <atomic>
#include <vector>
// #include <functional>

#include "config.h"

#define BPM_MIN 30
#define BPM_MAX 240
#define BPM_MIN_PREF 80
#define BPM_MAX_PREF 130
#define MAX_POTENTIAL_TEMPOS BPM_MAX/BPM_MIN;
#define MIN_BEEP_SPAN  60./BPM_MAX // == 0.25 seconds
#define MAX_BEEP_SPAN  60./BPM_MIN // == 2 seconds
#define RYTHM_INTONATION 0

struct time_signature
{
    int numerator;
    int denominator;
};

class Metronome{
public:
	Metronome();

    // void set_tempo(float);
    // void set_rythm(int,int);

    // void run(); // discard
    void lock();
    void unlock();
    void pause_toggle();
    void pause();
    void unpause();

    void display(); // ?

    void start_timing();
    void stop_timing();
    void tap_tempo();

	void tick_tock(); // threaded
    void alter_tempo();

    void clear();

private:

    Config& cfg = Config::getInstance();

	float tempo; //bpm
	time_signature rythm; // i.e. 4/4 (tick on first beat)
    
    // std::thread metro_thread; //discard
    int num_measures;

	std::chrono::steady_clock::time_point present, future, tap;
    std::chrono::duration<float> beep_span;
    int ms_beep;
    
    void _nullify_tempo();
    void _set_tempo();


	std::atomic<bool> is_initialized,is_paused,is_locked;

    std::vector<std::pair<int,float>> alternative_tempos;
    int current_tempo;
    
};
#endif