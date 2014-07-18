/*Biblioteca sensorflujo*/
#include "sensorflujo.h"

#include "Arduino.h"

/*Constructor*/
sensorflujo::sensorflujo(){
_nombre = "Sensor de flujo";
pinMode(2, INPUT);
}

/*Mostrar nombre del sensor*/
char * sensorflujo::mostrarNombre(){
return _nombre;
}

/*Mostrar medida obtenida*/
double sensorflujo::mostrarMedida(){
return transformarMedida();
}

/*Transforma valor analogico*/
double sensorflujo::transformarMedida(){
	flujo = (revoluciones * 60 / 7.5);
	return flujo;
}

/*Cuenta las revoluciones del sensor*/
void sensorflujo::rpm ()    
{ 
  revoluciones++; 
} 

/*Setea valores de las revoluciones*/
void sensorflujo::set_rpm(){
  revoluciones=0;
}
