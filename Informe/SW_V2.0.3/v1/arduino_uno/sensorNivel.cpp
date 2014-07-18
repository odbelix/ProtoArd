/*Biblioteca sensorNivel*/
#include "sensorNivel.h"

/*Bbliotecas de Arduino*/
#include "Arduino.h"

/*Constructor*/
sensorNivel::sensorNivel(){
_nombre = "Sensor de Nivel";
Serial.begin(9600);
pinMode(A2, INPUT);
}

/*Mostrar nombre del sensor*/
char * sensorNivel::mostrarNombre(){
return _nombre;
}

/*Mostrar medida obtenida*/
double sensorNivel::mostrarMedida(){
return transformarMedida();
}

/*Obtiene valor analogico*/
double sensorNivel::transformarMedida(){
	nivel=analogRead(2);
	return nivel;
}

/*Devuelve valor del nivel*/
double sensorNivel::get_nivel(){
	return nivel;
}
