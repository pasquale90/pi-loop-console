#ifndef AUDIOTRACK_H_INCLUDED
#define AUDIOTRACK_H_INCLUDED

class audiotrack:public route{
    audiotrack();

    void fill_track();
    void get_buffer_track();

private:
    std::vector<float> data;
};

#endif
