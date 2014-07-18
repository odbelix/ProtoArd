
/*Biblioteca sensorTemperatura*/
#include "sensorHumedad.h"

/*Biblioteca Arduino*/
#include "Arduino.h"



/*Constructor*/
sensorHumedad::sensorHumedad(){
_nombre = "Sensor de Humedad";
pinMode(A3, INPUT);

}

/*Mostrar nombre sensor*/
char * sensorHumedad::mostrarNombre(){
return _nombre;
}

/*Devolver valor del sensor*/
double sensorHumedad::mostrarMedida(){
return transformarMedida();
}

/*Transforma medida analogica del sensor*/
double sensorHumedad::transformarMedida(){
  medida_hum= analogRead(3);
  return medida_hum;
 }


