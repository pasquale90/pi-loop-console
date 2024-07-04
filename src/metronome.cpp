#include "metronome.h"

#include <iostream>
#include <limits>
#include <cmath>

//default constructor
Metronome::Metronome()
{
    is_initialized.store(false);
    is_paused.store(false);
    is_locked.store(false);
    ms_beep = 0;
    num_measures = 1;
    tap = std::chrono::steady_clock::time_point::min();

    tempo = cfg.tempo;
    rythm.numerator = cfg.rythm_numerator;
    rythm.denominator = cfg.rythm_denominator;

    display();
}

void Metronome::lock(){
    is_locked.store(true);
}

void Metronome::unlock(){
    is_locked.store(false);
}

void Metronome::pause_toggle(){
    is_paused.store(!is_paused.load());
}

void Metronome::pause(){
    is_paused.store(true);
}

void Metronome::unpause(){
    is_paused.store(false);
}

void Metronome::display(){
    std::cout<<"Metronome configuration:\n"
            <<"rythm is set to : "
            << rythm.numerator
            <<"/"
            <<rythm.denominator
            <<"tempo is set to "
            << tempo<< " bpm."
            <<"\nTempo list : [ ";
            for (auto t:alternative_tempos)
                std::cout<<t.second<<"("<<t.first<<")";
            std::cout<<" ]";
            std::cout
            <<"\nis initialized "<<is_initialized.load()
            <<"\nis paused "<<is_paused.load()
            <<"\nms_beep "<<ms_beep;
            std::cout<<std::endl;
}

void Metronome::start_timing(){
    
    _nullify_tempo();
    
    tap = std::chrono::steady_clock::time_point::min();
    present = std::chrono::steady_clock::now();
    
    lock();
}

void Metronome::stop_timing(){
    
    future = std::chrono::steady_clock::now();

    auto time_span = std::chrono::duration_cast< std::chrono::duration<double> >(future-present);
    beep_span = time_span / (double)rythm.numerator;
    _set_tempo();

}

void Metronome::alter_tempo(){
// Notes: Bugged. When used, it corrupts the timing of tick-tocks. 
// For now, it should be avoided..
// (..unless it is used along with the pause/unpause operations for synchronizing the tick-tocks).
//@TODO fix altering to be synchronised with the current beep transition.
    ++current_tempo;
    current_tempo %= alternative_tempos.size();
    num_measures = alternative_tempos.at(current_tempo).first;
    tempo = alternative_tempos.at(current_tempo).second; 
    ms_beep=(int) (beep_span.count()*1000) / num_measures;

    // update cfg
    cfg.tempo = tempo;
}

void Metronome::tick_tock()
{

    if (is_initialized.load() && !is_paused.load())
    {
        present = std::chrono::steady_clock::now();
        time_elapsed=std::chrono::duration_cast< std::chrono::duration<float> >(future-present);
        if( time_elapsed.count() < std::numeric_limits<float>::min() ){
                future = present + std::chrono::milliseconds(ms_beep);

                if (intonation == (int)RYTHM_INTONATION){
                    if (measure_begin == (int)RYTHM_INTONATION){
                        std::cout<<std::endl<<"tick (first)"<<std::endl;
                    }else{
                        std::cout<<std::endl<<"tick"<<std::endl;
                    }
                }
                else{
                    std::cout<<"tock"<<std::endl;
                }
                ++intonation;
                ++measure_begin;
                intonation %= rythm.numerator;
                measure_begin %= (rythm.numerator*num_measures);
        } 
    } else if (!is_initialized.load()){
        intonation=1;
        measure_begin = 1;
    } else if (is_paused.load()){
        intonation=0;
        measure_begin = 0;
    }

    // for debugging
    // display();
}

void Metronome::tap_tempo(){
    
    if (!is_locked.load()){

        _nullify_tempo();

        beep_span = std::chrono::duration_cast< std::chrono::duration<double> >(std::chrono::steady_clock::now() - tap);

        if (beep_span.count() >= (float)MIN_BEEP_SPAN && 
            beep_span.count() <= (float)MAX_BEEP_SPAN)
            {        
                _set_tempo();
                unpause();
            }
          
        tap = std::chrono::steady_clock::now();
    }
}

void Metronome::clear(){
    _nullify_tempo();
    tempo = cfg.tempo;
    rythm.numerator = cfg.rythm_numerator;
    rythm.denominator = cfg.rythm_denominator;
}

void Metronome::_nullify_tempo(){
    if (alternative_tempos.size())
        alternative_tempos.clear();
    tempo = 0;
    current_tempo = 0;
    beep_span = std::chrono::duration<double, std::micro>{0.0};
    num_measures = 1;
    is_initialized.store(false);
}

void Metronome::_set_tempo(){
                                                            // calc potential tempos
    float temp = 0;
    std::cout<<"Potential tempos  = [ ";
    while(temp < BPM_MAX){
        temp = ((float)num_measures * 60.)/beep_span.count(); //converts period to bpm
        
        if (temp>BPM_MIN && temp<BPM_MAX){
            std::cout<<temp<<"("<<num_measures<<") ";

            alternative_tempos.push_back(std::make_pair(num_measures,temp));
        }
<<<<<<< HEAD

=======
        
>>>>>>> rework-latest
        num_measures *= 2;

    }
    std::cout<<" ]"<<std::endl;
                                                        // check if there are potential tempos ..
    if (!alternative_tempos.size()){                        // .. if there are not ...
        tempo = 0;
        return;
    }else{                                                  // ,if there are ...
                                                            // ... choose one within a preferable range
        for (int i=0; i< alternative_tempos.size(); ++i){
            float t = alternative_tempos.at(i).second;
            if(t>BPM_MIN_PREF && t<BPM_MAX_PREF){
                current_tempo = i;
                num_measures = alternative_tempos.at(i).first;
                tempo = t;
                break;
            }
        }
        if (tempo <= std::numeric_limits<float>::epsilon()){        // if none of the prefered are chosen, choose the first one from the rest of them ..
            current_tempo = 0;
            num_measures = alternative_tempos.at(current_tempo).first;
            tempo = alternative_tempos.at(current_tempo).second;
        }
        if (tempo <= std::numeric_limits<float>::epsilon()){        // if none of the prefered are chosen ..
            is_initialized.store(false);
            return;
        }
    }
    
    // update_cfg
    cfg.tempo = tempo;
    cfg.rythm_numerator = rythm.numerator;
    cfg.rythm_denominator = rythm.denominator;

    ms_beep=(int) (beep_span.count()*1000) / num_measures;
    present = std::chrono::steady_clock::now();
    future = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms_beep);
    is_initialized.store(true);
}
