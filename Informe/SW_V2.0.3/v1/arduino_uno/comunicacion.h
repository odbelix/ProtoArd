#ifndef comunicacion_h
#define comunicacion_h
#include "Arduino.h"
class comunicacion{
  public:
    comunicacion();
    ~comunicacion();
    int enviar_mensaje(int uart);
    int recibir_mensaje(int uart);
    void delete_mensaje();
    int convertir(String numero);
    String get_mensaje();
    void limpiar();
    void set_mensaje(String msg);
  private:
    String mensaje;

};
#endif

