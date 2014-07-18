/*Biblioteca de Arduino*/
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>

/*Biblioteca de C*/
#include <iostream>

/*Biblioteca de red*/
#include "red.h"

/*Limita la cantidad de intentos de envio al servidor*/
#define INTENTOS_ENVIO_SERVIDOR 5
#define error_ethernet 5
using namespace std;

	/*Constructor*/
  red::red(){
      Serial.begin(9600);

  }
	/*Destructor*/
  red::~red(){
  }
  
  /*Setear variables para la configuracion de la conexion al servidor*/
   void red::set_mac(byte* e){
     mac=e;
   }
  void red::set_ip_local(byte* e){
     ip_local=e;
   }
  void red::set_ip_server(byte* e){
     ip_server=e;
   }
  void red::set_mask(byte* e){
     mask=e;
   }
  void red::set_port_server(int port){
    port_server=port;
  }
  
  /*Iniciar red*/
  int red::iniciar_red(){
	  /*Asigna ip local*/
    IPAddress ip(ip_local);
    /*Asigna ip servidor*/
    IPAddress server(ip_server);
    /*inciar ethernet local*/
    Ethernet.begin(mac, ip);
	/* Deshabilita w5100 SPI */
   digitalWrite(10,HIGH);
    /*Dar un segundo para inciciar red*/
    delay(1000);
    Serial.println("Conectando...");
  
    /*Si hay conexion retorna 1 si no 0*/
    if (client.connect(server, port_server)) {
      return 1;
    }
    else {
      error_red(1);
      return 0;
    }
  }
  
/*Leer un mensaje entrante por la red*/
 int red::leer_mensaje(){
   char ch;
    while (client.available() > 0) {
        
        ch = client.read();
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

/*Envia a la red un mensaje*/
 void red::enviar_mensaje(){
     Serial.print("Mensaje_Enviado: ");Serial.println(mensaje);
      client.print((char*)mensaje.c_str());

 }
 
 /*Borra mensaje*/
 void red::borrar_mensaje(){
  mensaje="";
 }
 
 /*Retorna el mensaje*/
 String red::get_mensaje(){
    return mensaje; 
 }
 
 /*Setear mensaje*/
 void red::set_mensaje(String msg){
  mensaje="";
  mensaje=msg;
}
/*Si el servidor esta disponible o detiene al cliente*/
 int red::server_disponible(){
  if (!client.connected()) {
    client.stop();
    return 0;
  }
  else{
    return 1;
  }
 }

/*Procedimiento para obtener conexion estable con el servidor*/ 
int red::procedimiento(String data){
  int valor=1;
  //Iniciar conexion en caso de perdida o al inicio
  if(server_disponible()==0){
     if(iniciar_red()){
        // Serial.println("CONEXION ESTABLECIDA");
         delay(5000);
      }
      else{
        Serial.println("ERROR_EN_CONEXION");
        error_red(2);
        valor=0;
      }
  }

  //lee mensaje de bienvenida de conexion
  if(leer_mensaje()){
          //imprime mensaje de bienvenida
          //Serial.print("Mensaje Server: ");Serial.println(get_mensaje());
          //Enviara mensajes mientras el servidor este disponible
          if(server_disponible()){
            //borra buffer mensaje de bienvenida
            borrar_mensaje();
            //seteamos mensaje a enviar
            set_mensaje(data);
            //enviamos mensaje a (ip:port)
            enviar_mensaje();
            borrar_mensaje();
            delay(2000);
            //leer mensaje de confirmacion
            leer_mensaje();
            if(strcmp(get_mensaje().c_str(),"1")){
              valor=0;
            }
            //imprimir mensaje del servidor
            //Serial.print("Mensaje Server: ");Serial.println(get_mensaje());
            //lapso para proveer tiempo al servidor y revisar posteriomente
            //el estado de la conexion
            delay(8000);
            client.stop();
            borrar_mensaje();
         }
    }
   return valor;
}

/*Funcion que recibe el mensaje listo para ser enviado en caso de que 
 * la conexion falle esta se encargue de que el mensaje se entregue
 * (Restringir a cantidad de intentos de envio para no caer a un bucle infinito)*/
void red::enviar_por_red(String data){
  int a=0;
  while(procedimiento(data)==0 && a<INTENTOS_ENVIO_SERVIDOR){
      delay(1000);
      Serial.println("Enviar_Data_otra_vez");
      a++;
    }
    if(a>=INTENTOS_ENVIO_SERVIDOR){
      Serial.println("Error_Fatal:_Revise_estado_de_conexion");
      error_red(2);
    }
    delay(3000);
}

/*Funcion para indicar los errores que ocurren
  */
void red::error_red(int op){
  /*Error Iniciando red*/
  if(op==1){
    for(int a=0;a<10;a++){
      digitalWrite(error_ethernet,HIGH);
      delay(100);
      digitalWrite(error_ethernet,LOW);
      delay(100);
      digitalWrite(error_ethernet,HIGH);
      delay(100);
      digitalWrite(error_ethernet,LOW);
      delay(1000);
    }
  }
  /*Error al enviar por red*/
  if(op==2){
  for(int a=0;a<5;a++){
      digitalWrite(error_ethernet,HIGH);
      delay(100);
      digitalWrite(error_ethernet,LOW);
      delay(100);
      digitalWrite(error_ethernet,HIGH);
      delay(100);
      digitalWrite(error_ethernet,LOW);
      delay(1000);
    }
    
  }
  
}

