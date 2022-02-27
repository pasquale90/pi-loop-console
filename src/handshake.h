#ifndef HANDSHAKE_H_INCLUDED
#define HANDSHAKE_H_INCLUDED

#include <stdint.h>
#include <string>
//######################################################################################################################
//FORWARD DECLARATIONS
typedef struct _jack_port jack_port_t;
typedef struct _jack_client jack_client_t;
typedef uint32_t jack_nframes_t;
#ifdef __cplusplus
extern "C" {
#endif
jack_nframes_t jack_get_sample_rate(jack_client_t *);
jack_nframes_t 	jack_get_buffer_size (jack_client_t *);
float jack_cpu_load(jack_client_t *client);
#ifdef __cplusplus
}
#endif

//#####################################################################################################################
//######################################################################################################################

class handshake{
public:
    handshake(const char* clientname);
    ~handshake();

    /*encapsulation methods*/
    jack_client_t* get_client() { return client; }


    /*track operations*/
    jack_port_t* add_input_port(jack_port_t *_port,const char *inport_name);//virtual - midi and audio comes from different input ports
    jack_port_t* add_output_port(jack_port_t *_port,const char *outport_name);
    void disconnect_port(jack_port_t *_port);

    void name_port(jack_port_t *_port,const char* name);

    /*IO handshake operations*/
    void connect_input_device(int input_device,jack_port_t *_port);
    void connect_output_device(int output_device,jack_port_t *_port);


    /*controlling operations*/
    void activate();
    void disconnect_client();
    void prevent_failure();//{jack_on_shutdown(client,handshake::jack_shutdown,0);};
    //void (handshake::*sd)(void *arg);

    void callback(int (*process)(jack_nframes_t nframes, void *arg),void *arg);

    /*info control operations*/
    void info_control();
    uint32_t get_sample_rate() { return jack_get_sample_rate (client); }
    uint32_t get_buffer_size() { return jack_get_buffer_size (client); }
    float get_cpu_load(){ return jack_cpu_load(client); }
    /*const char* get_cpu_load(){ return cpu_percent(); }*/

    const char* get_name(){return client_name;}
private:
    jack_client_t *client;
    const char *client_name;
    const char **fromdevice;
    const char **todevice;

};

#endif // HANDSHAKE_H_INCLUDED
