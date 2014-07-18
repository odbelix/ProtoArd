#ifndef sensorNivel_h
#define sensorNivel_h
class sensorNivel {
public:
sensorNivel();
double transformarMedida();
double mostrarMedida();
char * mostrarNombre();
double get_nivel();
private:
double nivel;
char* _nombre;
};
#endif
