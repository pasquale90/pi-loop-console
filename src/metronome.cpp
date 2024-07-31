#include "metronome.h"

#include <iostream>
#include <limits>
#include <cmath>

Metronome::Metronome()
{
    is_initialized.store(false);
    is_paused.store(false);
    is_locked.store(false);

    num_measures = 1;
    tap = std::chrono::steady_clock::time_point::min();

    tempo = cfg.tempo;
    rythm.numerator = cfg.rythm_numerator;
    rythm.denominator = cfg.rythm_denominator;

    buffers_per_quarter = buffer_counter = total_buffers = 0;
    // display();
}

void Metronome::lock(){
    is_locked.store(true);
}

void Metronome::unlock(){
    is_locked.store(false);
}

void Metronome::pause(){
    is_paused.store(true);
    sync();
}

void Metronome::unpause(){
    is_paused.store(false);
}

void Metronome::sync(){
    buffer_counter = 0;
}

void Metronome::start_timing(){
    
    _nullify_tempo();
    
    tap = std::chrono::steady_clock::time_point::min();
    present = std::chrono::steady_clock::now();
    
    lock();
}

void Metronome::stop_timing(int loop_length){
    
    total_buffers = loop_length/BUFFER_SIZE;

    // calculate quarter span
    auto time_span = std::chrono::duration_cast< std::chrono::duration<float> >(std::chrono::steady_clock::now()-present);
    beep_span = time_span / (float)rythm.numerator;

    // calculate estimated samples
    std::chrono::duration<float, std::milli> time_span_ms = time_span;
    int estimated_samples = time_span_ms.count()*SAMPLE_RATE/1000;

    // Check if timer and looper are aligned. They must have a small difference.
    int buffThres = BUFFER_MISMATCH_THRESHOLD;
    if (abs(estimated_samples-loop_length) > buffThres*BUFFER_SIZE)
        std::cout<<"Loop length mismatch. Estimated samples are "<<estimated_samples<<" are != "<<loop_length<<" by a margin greater than "<<buffThres<<" buffers, equals to "<< BUFFER_SIZE*1000*buffThres / SAMPLE_RATE <<" ms. Check your audio settings : \n>sampling rate is set to "<<SAMPLE_RATE<<"\nbuffer size is set to "<<BUFFER_SIZE<<std::endl;
    // else
    //     std::cout<<"Estimated samples are "<<estimated_samples<<" are almost equal to loop length "<<loop_length<<" by a margin less than "<<buffThres<<" buffers, equals to "<< BUFFER_SIZE*1000*buffThres / SAMPLE_RATE <<" ms. Almost equal!"<<std::endl;

    _set_metro();

}

void Metronome::alter_tempo(){

    // int prev_num_measures = num_measures;
    
    ++current_tempo;
    current_tempo %= alternative_tempos.size();
    num_measures = alternative_tempos.at(current_tempo).first;
    tempo = alternative_tempos.at(current_tempo).second; 

    buffers_per_quarter = total_buffers/(rythm.numerator*num_measures); //floor
    int buffer_offset=total_buffers%(rythm.numerator*num_measures);
    actual_buffers = total_buffers - buffer_offset;

    // update cfg
    cfg.tempo = tempo;

    std::cout<<"Tempo set to "<<tempo<<" ( num measures = "<<num_measures<<")"<<std::endl;
}


int Metronome::tick_tock()
{
    int state = -1;
    if (is_initialized.load() && !is_paused.load())
    {
        if (buffer_counter%buffers_per_quarter == 0){
            
                if (buffer_counter == (int)RYTHM_INTONATION){
                    state = 0;
                }else if (buffer_counter< actual_buffers){

                    int intonation = buffer_counter/buffers_per_quarter;

                    if (intonation%rythm.numerator == 0){
                        state = 1;
                    }
                    else{
                        state = 2;
                    }
                }
         }
        ++buffer_counter;
        buffer_counter %= total_buffers;
    }
    return state;
}

void Metronome::tap_tempo(){
    
    if (!is_locked.load()){

        _nullify_tempo();

        beep_span = std::chrono::duration_cast< std::chrono::duration<float> >(std::chrono::steady_clock::now() - tap);

        if (beep_span.count() >= (float)MIN_BEEP_SPAN && 
            beep_span.count() <= (float)MAX_BEEP_SPAN)
            {       
                total_buffers = (rythm.numerator*beep_span.count())*SAMPLE_RATE/BUFFER_SIZE;
                _set_metro();
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
    beep_span = std::chrono::duration<float, std::micro>{0.0};
    num_measures = 1;
    is_initialized.store(false);
}

void Metronome::_set_metro(){
    
    _calculate_tempos(); // calc potential tempos
    
    if (tempo){

        buffers_per_quarter = total_buffers/(rythm.numerator*num_measures); //floor
        int buffer_offset=total_buffers%(rythm.numerator*num_measures);
        actual_buffers = total_buffers-buffer_offset;

        is_initialized.store(true);

        // update_cfg
        cfg.tempo = tempo;
        cfg.rythm_numerator = rythm.numerator;
        cfg.rythm_denominator = rythm.denominator;

        std::cout<<"Tempo set to "<<tempo<<" ( num measures = "<<num_measures<<")"<<std::endl;    
    }
}

void Metronome::_calculate_tempos(){

    float temp = 0;
    // std::cout<<"Potential tempos  = [ ";
    while(temp < BPM_MAX){
        temp = ((float)num_measures * 60.)/beep_span.count(); //converts period to bpm
        
        if (temp>BPM_MIN && temp<BPM_MAX){
            // std::cout<<temp<<"("<<num_measures<<") ";

            alternative_tempos.push_back(std::make_pair(num_measures,temp));
        }
        num_measures *= 2;
    }
    // std::cout<<" ]"<<std::endl;

                                                        // check if there are potential tempos ..
    if (!alternative_tempos.size()){                        // .. if there are not ...
        tempo = 0;
        return;
    }else{                                                  // ,if there are ...
                                                            // ... choose one within a preferable range
        for (int i=alternative_tempos.size()-1; i>= 0; --i){    // from higher to lower 
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
}
