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

/*! @brief Struct to hold the information of rythm 
* @param int numerator - the rythm numerator
* @param int denominator - the rythm denominator
*/
struct time_signature
{
    int numerator;
    int denominator;
};

// @TODO fix metronome. Instead of timing, use loop length to calculate the #buffers per quarter unit

/*! @brief A metronome defined within Looper.*/
class Metronome{
public:
    /*! @brief Class contructor.
     */
	Metronome();

    /*! @brief function to set lock to true, so as to prevent metronome from changing.
     * @return void
    */
    void lock();

    /*! @brief function to set lock to false, so as to enable resetting of the metronome.
     * @return void
    */
    void unlock();
    
    void pause_toggle();

    /*! @brief function to pause the counting of the metronome.
     * @return void
    */
    void pause();

    /*! @brief function to unpause the metronome.
     * @return void
    */
    void unpause();

    // void display();

    /*! @brief function to start timing the first loop.
     * @return void
    */
    void start_timing();
    
    /*! @brief function to stop timing and set the tempo.
     * @return void
    */
    void stop_timing();

    /*! @brief function to read taps from the user for setting the tempo
     * @return void
    */
    void tap_tempo();

    /*! @brief function that counts the time and does the tick-tocking
     * @return int -  the intontation of the tick. 0 if the first of the loop, 1 if first of the measure, 2 otherwise
    */
	int tick_tock(); // threaded

    /*! @brief function that changes the tempo to the next most suitable form.
     * @return void
    */
    void alter_tempo();

    /*! @brief Function used to reset the metronome.(i.e. for when changing session )
     * @return void
    */
    void clear();

private:

    Config& cfg = Config::getInstance();
	float tempo; //bpm
	time_signature rythm; // i.e. 4/4 (tick on first beat)
    int num_measures;
	std::chrono::steady_clock::time_point present, future, tap;
    std::chrono::duration<float> time_elapsed, beep_span;
    int intonation, measure_begin;
    int ms_beep;

	std::atomic<bool> is_initialized,is_paused,is_locked;
    std::vector<std::pair<int,float>> alternative_tempos;
    int current_tempo;

    /*! @brief function to clear all data.
     * @return void
    */
    void _nullify_tempo();

    /*! @brief function used to do all the operations required to calculate the tempo, the num of measures, and so on.
     * @return void
    */
    void _set_tempo();

    
};
#endif