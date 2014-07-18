/*
 * Clase para sensor DS18S20
 * 
*/
 
/*Biblioteca sensorTemperatura*/
#include "sensorTemperatura.h"

/*Biblioteca Arduino*/
#include "Arduino.h"
/*Biblioteca requerida en principal.ino*/
#include <OneWire.h>


/*Constructor*/
sensorTemperatura::sensorTemperatura(){
_nombre = "Sensor de Temperatura";
Serial.begin(9600);
pinMode(5, INPUT);
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
  byte data[12];
  byte addr[8];
  
  if(!ds.search(addr)){
    ds.reset_search();
    return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  
  for(int i=0;i<9;i++){
    data[i]=ds.read();
  }
  
  ds.reset_search();
  
  byte MSB= data[1];
  byte LSB= data[0];
  
  float TRead = ((MSB << 8)| LSB);
  medida_temp = TRead /16;
  return medida_temp;
}

