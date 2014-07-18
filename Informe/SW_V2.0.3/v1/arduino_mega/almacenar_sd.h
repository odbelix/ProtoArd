#ifndef almacenar_sd_h
#define almacenar_sd_h
#include "Arduino.h"
class almacenar_sd{
  public:
    almacenar_sd();
    ~almacenar_sd();
    int escribir(String data,String nombre);
    void error_sd(int op);
    /*Funciones Futuras
    void leer();
    int crear(String name);
    */
    void init_sd();

};
#endif
