/*
 * Clase para sensor DS18S20
 *
*/

#ifndef sensorTemperatura_h
#define sensorTemperatura_h
class sensorTemperatura {
public:
sensorTemperatura();
double transformarMedida();
double mostrarMedida();
char * mostrarNombre();
private:
char* _nombre;
double medida_temp;
};
#endif
