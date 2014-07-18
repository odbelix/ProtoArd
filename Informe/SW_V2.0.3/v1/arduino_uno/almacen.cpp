/*Biblioteca de Arduino*/
#include "Arduino.h"

/*Biblioteca de Lista*/
#include "Lista.h"

/*Biblioteca de almacen*/
#include "almacen.h"

/*Constructor*/
almacen::almacen(){
	Serial.begin(9600);
    pinMode(alerta_insercion, OUTPUT);  	
}
/*Destructor*/
almacen::~almacen(){}

/*Agregar valor a la lista
 * por cada valor agregado enciende el designadado (12) led
 * */
void almacen::almacenar(double value){
	data.push_back(value);
    digitalWrite(alerta_insercion, HIGH);   // Enciende led)
    delay(250);               // espera
    digitalWrite(alerta_insercion, LOW);    // APaga led
    delay(250);
}

/*Libera la lista*/
void almacen::liberar(){
	data.clear();
}

/*Envia mediciones realizadas en formato de String*/
String almacen::enviar_medicion(){
	String mensaje;
	int a=data.size();
	//mensaje+=name_sensor;
    while(a>0){
		mensaje+=';';
		mensaje+=data.getElement((a-1));
        a--;
    }
    mensaje.replace('.',',');

   return mensaje;
}
