#include <jack/jack.h>
#include <jack/types.h>
#include <jack/control.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <string.h>

static jackctl_driver_t * jackctl_server_get_driver(jackctl_server_t *server, const char *driver_name)
{
    const JSList * node_ptr = jackctl_server_get_drivers_list(server);

    while (node_ptr) {
        if (strcmp(jackctl_driver_get_name((jackctl_driver_t *)node_ptr->data), driver_name) == 0) {
            return (jackctl_driver_t *)node_ptr->data;
        }
        node_ptr = jack_slist_next(node_ptr);
    }

    return NULL;
}

static jackctl_parameter_t * 
jackctl_get_parameter(const JSList * parameters_list,const char * parameter_name)
{
    while (parameters_list)
    {
        if (strcmp(jackctl_parameter_get_name((jackctl_parameter_t *)parameters_list->data), parameter_name) == 0)
        {
            return (jackctl_parameter_t *)parameters_list->data;
        }

        parameters_list = jack_slist_next(parameters_list);
    }

    return NULL;
}


jackctl_parameter_value set_param_value(union jackctl_parameter_value value, jackctl_param_type_t type, void *setvalue)
{
    switch (type) {

        case JackParamInt:
            //value.i=setvalue;
            printf("parameter value = %d\n", );
            break;

         case JackParamUInt:
            //value.ui=setvalue;
            printf("parameter value = %u\n", value.ui);
            break;

         case JackParamChar:
            //value.c=setvalue;
            //printf("parameter value = %c\n", value.c);
            break;

         case JackParamString:
            //value.str=setvalue;
            //printf("parameter value = %s\n", value.str);
            break;

         case JackParamBool:
            //value.b=setvalue;
            //printf("parameter value = %d\n", value.b);
            break;
     }
     return value;
}

int main ()
{
        std::cout<<"Start jack server"<<std::endl;
        jackctl_server_t * server;
        const JSList * parameters;
        jackctl_sigmask_t * sigmask;
        const char* driver_name = "alsa";
        
        static jackctl_driver_t *driver=jackctl_server_get_driver(server,driver_name);
        
        server = jackctl_server_create(NULL, NULL);
        parameters = jackctl_server_get_parameters(server);
        
        jackctl_server_open(server, driver);
        jackctl_server_start(server);

        // load one internal client.
        //const char* client_name = "internal_client_random_name!";
        //jackctl_server_load_internal(server, jackctl_server_get_internal(server, client_name));


        //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
       
        //get parameter using name of parameter -- in this case :parameter name=device
        static jackctl_parameter_t * device=jackctl_get_parameter(parameters,"device");
        //get the value
        union jackctl_parameter_value * device_value=print_value(jackctl_parameter_get_default_value(device), jackctl_parameter_get_type(device),"hw:1");
        device_value.str="hw:1";


        

         //doc --> set parameters value
        bool jackctl_parameter_set_value (
            jackctl_parameter_t *parameter, 
            const union jackctl_parameter_value *value_ptr);
        //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

        //if (jackctl_parameter_set_value();

        /*
        jackctl_parameter_set_value()
        ( 	jackctl_parameter_t *  	parameter,
		const union jackctl_parameter_value *  	value_ptr 
	) 	



        jackctl_server_open(server, jackctl_server_get_driver(server, driver_name));
        jackctl_server_start(server);//,driver
        jackctl_server_load_internal(server, jackctl_server_get_internal(server, client_name));



        sigmask = jackctl_setup_signals(0);
        jackctl_wait_signals(sigmask);
        //closing server
        jackctl_server_stop(server);
        jackctl_server_close(server);
        jackctl_server_destroy(server);
//
std::ofstream cout_log ("logs/cout.log");
std::ofstream cerr_log ("logs/cerr.log");

// connect stream buffers
std::streambuf *coutbuf = std::cout.rdbuf();
std::cout.rdbuf(cout_log.rdbuf () );
std::streambuf *cerrbuf = std::cerr.rdbuf();
std::cerr.rdbuf(cerr_log.rdbuf () );


    



// restore
std::cout.flush ();
std::cout.rdbuf(cerrbuf);

std::cerr.flush ();
std::cerr.rdbuf(cerrbuf);

        return 0;
}
