/*Biblioteca de comunicacion*/
#include "comunicacion.h"

/*Biblioteca de Arduino*/
#include "Arduino.h"

/*Biblioteca de C*/
#include "string.h"

/*Constructor*/
/*Habilita los canales de comunicacion
 * Solo para mega!!*/
comunicacion::comunicacion(){
  //Serial.begin(9600);
  mensaje="";
}

/*Destructor*/
comunicacion::~comunicacion(){
}
/*Lee mensajes por el puerto UART solicitado
 * retorna 1 si se logra, retorna 0 en caso
 * de pedir un uart no existente*/
int comunicacion::enviar_mensaje(int uart){
  mensaje+='*';
  if(uart == 1){
  Serial.print(mensaje);
  return 1;
  }
 if(uart > 4){
    return 0;
  }
}
/*Recibe datos por el puerto UART solicitado
 */
int comunicacion::recibir_mensaje(int uart){
  char ch;
    if(uart==1){
    while (Serial.available() > 0) {
        ch = Serial.read();
        if (ch == '*') {   //fin mensaje
            return 1;
        }
        else { // si no es fin agrega caracterer del buffer
           mensaje+=ch;
           if(ch=='\0'){
             return 0;
           }
        }
    }
   }
}
/*Borra contenido de mensaje*/
void comunicacion::delete_mensaje(){
  mensaje="\0";
}

/*Retorna el mensaje capturado*/
String comunicacion::get_mensaje(){
  return mensaje; 
}

/*Convierte numero de String a numero int*/
int comunicacion::convertir(String numero){
  char letra;
  String codigo;
  codigo=numero.substring(0,(numero.length()));
  if (codigo.length() > 0) {
        return(atoi(codigo.c_str()));
   }
  else{
     return 0;
   }
}

/*Limpia todos los puertos Seriales*/
void comunicacion::limpiar(){
  Serial.flush();

 }
 
/*Borra mensaje y lo setea con nuevo*/
void comunicacion::set_mensaje(String msg){
  mensaje="";
  mensaje=msg;
}
