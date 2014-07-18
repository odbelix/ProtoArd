#ifndef sensorHumedad_h
#define sensorHumedad_h

class sensorHumedad {
  public:
      sensorHumedad();
      double transformarMedida();
      double mostrarMedida();
      char * mostrarNombre();
  private:
      char* _nombre;
      double medida_hum;
};
#endif
