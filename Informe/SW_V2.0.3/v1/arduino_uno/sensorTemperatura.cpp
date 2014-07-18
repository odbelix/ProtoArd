/*Biblioteca sensorTemperatura*/
#include "sensorTemperatura.h"

/*Biblioteca Arduino*/
#include "Arduino.h"


/*Constructor*/
sensorTemperatura::sensorTemperatura(){
_nombre = "Sensor de Temperatura";
Serial.begin(9600);
pinMode(A0, INPUT);
}

/*Mostrar nombre sensor*/
char * sensorTemperatura::mostrarNombre(){
return _nombre;
}

/*Devolver valor del sensor*/
double sensorTemperatura::mostrarMedida(){
return transformarMedida();
}

/*Transforma medida analogica del sensor*/
double sensorTemperatura::transformarMedida(){
 int valueread=analogRead(0);
   if(valueread<340) {
    return -3.0 + (valueread-120.0)/(340-120) * ( 15.0 + 3.0 ) + 3.5;
  } else if(valueread<500) {
    return 13.0 + (valueread-340.0)/(500-340) * (30 - 13.0) + 3.5;
  } else if(valueread<660) {
    return 28.0 + (valueread -500.0) /(660-500) * (50 - 28) + 3.5;
  } else {
    return 48.0 + (valueread-660.0) /(790 - 660) * (75 - 48) + 3.5;
  }
}

