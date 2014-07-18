/*Biblioteca de almacenar_sd*/
#include "almacenar_sd.h"

/*Biblioteca de Arduino*/
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

/*Biblioteca de C*/
#include "string.h"

/*Pines Alerta*/
#define alerta_sd 2

/*Constructor en almacenar_sd*/
almacenar_sd::almacenar_sd(){
  Serial.begin(9600);
   /*PINES PARA ALERTAS*/
 pinMode(alerta_sd,OUTPUT);
}

/*Destructor en almacenar_sd*/
almacenar_sd::~almacenar_sd(){
}


//Escribe los valores obtenidos en un archivo
int almacenar_sd::escribir(String data,String nombre_archivo){
   
  File dataFile = SD.open(nombre_archivo.c_str(), FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    return 1;
  }
  // Si el archivo no se crea enviamos error:
  else {
    Serial.println("error_opening"+nombre_archivo);
    return 0;
  }	
}

/* Funciones futuras
void almacenar_sd::leer(){
}

int almacenar_sd::crear(String name){
}
*/
/*Indica error segun parpadeo de LED*/
void almacenar_sd::error_sd(int op){
  /*Error en Escritura*/
  if(op==1){
    for(int a=0;a<5;a++){
      digitalWrite(alerta_sd,HIGH);
      delay(100);
      digitalWrite(alerta_sd,LOW);
      delay(1000);
    }
  }
  /*Error en Iniciar SD*/
  if(op==2){
    for(int a=0;a<5;a++){
      digitalWrite(alerta_sd,HIGH);
      delay(100);
      digitalWrite(alerta_sd,LOW);
      delay(100);
      digitalWrite(alerta_sd,HIGH);
      delay(100);
      digitalWrite(alerta_sd,LOW);
      delay(1000);
    }
  }
}

/*Iniciar SD*/
void almacenar_sd::init_sd()
{

  //Serial.print(F("Iniciando SD..."));
  

  // ve si la tarjeta esta presente e inicia la tarjeta
  if (!SD.begin(4)) {
    //Serial.println(F("Iniciacion Fallida, o SD no esta presente"));
    //No hace nada mas
    error_sd(2);
    return;
  }
  //Serial.println(F("SD iniciada."));
}
