#include "effects.h"

Effects::Effects(){
    buffer_size = 256;
}

void Effects::whiteNoise(float *in)
{
    // Define random generator with Gaussian distribution
    const double mean = 0.0;
    const double stddev = 0.3;
    std::default_random_engine generator;
    std::normal_distribution<double> dist(mean, stddev);

    // Add Gaussian noise
    for (int i=0; i<buffer_size; ++i) {
        in[i] = in[i] + dist(generator);
    }

}

void Effects::effect2(float *in){

}


void Effects::effect3(float *in){
    
}