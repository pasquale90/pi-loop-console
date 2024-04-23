#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

class Config {
    public:
      static Config& getInstance();

      // Add your public members and methods here
      // ...
      int eraseme;

    private:
      Config();
      Config(const Config&) = delete;
      void operator=(const Config&) = delete;

      static Config* cfg_instance_ptr;
};

#endif