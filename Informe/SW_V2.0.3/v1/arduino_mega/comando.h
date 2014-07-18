#ifndef comando_h
#define comando_h
#include "Arduino.h"
/*LIMITE PARA REGISTRAR DISPOSITIVOS*/
#define limite_dispositivos 5
class comando{
  public:
    comando();
    ~comando();
    void modo_comando();
   /*void leer_configuracion();*/
    void configurar_parametro(String DL,String DH);
    void salir_modo_comando();
    void reset();
    void limpiar();
    void leer_data();
    String get_data();
    void escribir_data();
    void explorar_red();
    String get_direccion(int a);
   private:
     String data;
     int dispositivos;
     String tabla[limite_dispositivos];
};
#endif

