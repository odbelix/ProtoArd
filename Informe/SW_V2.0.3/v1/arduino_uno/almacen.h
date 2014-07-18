#ifndef almacen_h
#define almacen_h
#include "Lista.h"
#define alerta_insercion 3
class almacen{
  public:
	almacen();
	~almacen();
	void almacenar(double value);
	void liberar();
	String enviar_medicion();
        void set_sensor(String nombre);
	String get_sensor();
  private:
	LList<double> data;
	String name_sensor;
	
	
};
#endif
