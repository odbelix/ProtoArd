
/*
	Bibliotecas c
*/
#include <iostream>
#include <fstream>
#include <string>
/*********************************************************************/
/*
	Bibliotecas Arduino
*/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
/*********************************************************************/
/*
	Bibliotecas Propias
*/

#include "red.h"			   // Biblioteca para la comunicacion con el servidor
#include "RelojDS1307.h"       // Biblioteca para utilizar el Reloj de Shield SD+RTC
#include "comunicacion.h"	   // Biblioteca para realizar comunicacion con los puertos UART
#include "almacenar_sd.h"      // Biblioteca para realizar el almacenamiento en memoria SD de Shield Ethernet
#include "comando.h"          //Biblioteca para la configuracion del enrutamiento

/*********************************************************************/

/*
	Objetos de las clases
*/

red router;
comunicacion chanel;
RelojDS1307Class Reloj;
almacenar_sd logger;
comando terminal;

/*********************************************************************/

/*
	Variables globales utilizadas
*/
int opcion=0;										   // Variable para decir curso de accion despues de llamar a escribir()
int a=0;  //Variable para contar cantidad de dispositivos y truncar ciclos de espera
int intentos=0; // Variable para contabilizar la cantidad de intentos fallidos de escribir
int activar_rele=1;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // Direccion mac utilizada para el shield ethernet
byte ip_local[] = {192, 168, 1, 177};			   // Direccion ip para el shield ethernet
byte ip_server[]={192, 168, 1, 33};				   // Direccion ip del servidor 
int port_server=3010;							   // Puerto para conectarse en el servidor
String mensaje="";
String direccion="";
String sensor="";
String sensores[10]={"","1","2","3","4","5"};
/*
Variables para el direccionamiento de la peticion
char* direccion[10] ={"40b1db6c","40a04ca5"};
char* DH="13a200";
*/

char* DH="13a200";
/*
	Iniciacion de variables para arduino
*/
void setup(){
	/*Iniciar salida */
	Serial.begin(9600);

  /*Iniciar variables del objeto router para comunicacion con servidor*/
  router.set_mac(mac);
  router.set_ip_local(ip_local);
  router.set_ip_server(ip_server);
  router.set_port_server(port_server);
  
  /* Deshabilitar w5100 para iniciar SD*/
   pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
   Wire.begin();
   logger.init_sd();
 
 /*Reinicio del reloj RTC*/
 //Reloj.SetDateTime( 14, 02, 17, 16, 16, 30 );
 
 /********************************************************/
 /*AGREGAR LISTAR DISPOSITIVOS CONECTADOS*/
 /***Indicaciones***/
//Serial.println("\nIngrese:\nListar\n o ");
//Serial.println("mac;n_sensor\n");
//Serial.print("*");
}

/*Ciclo de trabajo de Arduino*/
void loop(){
		
        if(Serial.available()==0){
        
          /*Recepcion de mensaje en puerto Uart1 */
          while(chanel.recibir_mensaje(1)==0){
              
              }
/*************************************************************************/
              /* Listar equipos disponibles en la red*/
              /*Si mensaje dice listar entrega la tabla de direcciones*/
/*************************************************************************/ 
          chanel.limpiar();
          if(chanel.get_mensaje().equals("Listar")){
            Serial.println("*****Inicio_Listar*****_(1_min)");
            terminal.explorar_red();
            Serial.println("Nodos_Disponibles: ");
            while(a<5){
              if(terminal.get_direccion(a).length()>1){
              Serial.println(terminal.get_direccion(a));
              }
              a++;
            }
            a=0;
            chanel.delete_mensaje();
            mensaje="";
            direccion="";
            sensor="";
            Serial.println("*****Fin_Listar*****\n");
            Serial.println("FIN");
          }
          if(chanel.get_mensaje().length()==10){
          /*Obtener MAC y sensor*/
          mensaje = chanel.get_mensaje();
          direccion = mensaje.substring(0,8);
          sensor = mensaje.substring(9,mensaje.length());
/***********************************************************************/
		  /*Configurar parametros de Xbee*/
	  terminal.configurar_parametro(direccion.c_str(),DH);
	  
/***********************************************************************/		  
          /*Solicitud para habilitar Sensor*/
          chanel.set_mensaje(sensor);
        
          /*Enviar mensaje por UART3 (Xbee)*/
          chanel.enviar_mensaje(4);
          
          /*Se vacia el mensaje*/
          chanel.delete_mensaje();         
          }
        }
        //Sensor 
        if(sensor.length()>0){
          if(atoi(sensor.c_str())!=5){
           control_sensor();
          }
          else{
            //Activa rele
            if(activar_rele==1){
              Serial.println("ACTIVADO");
              Serial.println("FIN");

              activar_rele=0;
            }
            else{
            Serial.println("DESACTIVADO");
            Serial.println("FIN");
             activar_rele=1;
            }
          }

        }
        chanel.limpiar();
       
}

/*
	Confirmar la cantida de valores solicitados
	(Caso ejemplo: En 20 valores de Temperatura deben
	venir 20 , (comas) si no es asi se devuelve un 0)
*/
int confirmar(int value){
  int l=chanel.get_mensaje().length();
  char cadena[l];
  strcpy(cadena,chanel.get_mensaje().c_str());
  int c=0;
  for(int i=0;i<l;i++){
  if(cadena[i]==',') c++;
  }
  if(c==value){
    return 1;
  }
  else{
    return 0;
  }
} 

/*
	Control sobre el ciclo de trabajo completo
	por sensor
*/
void control_sensor(){
               //recibe data
               //Serial.print("Received: ");
               do{
                 chanel.delete_mensaje();
                 while(chanel.recibir_mensaje(4)==0){delay(5);
                 }
                 a=0;
                 chanel.limpiar();
                 //envio confirmacion de recibo cantidad
                 if(confirmar(20)){
                   //Imprimir pantalla mensaje con datos recibido
                   //chanel.enviar_mensaje(1);
                   //guardar data en SD
			/*********************************************************************/
				/*Preparacion del mensaje para ser guardado y enviado por ethernet*/      

                    String data;
                    data+=Reloj.DateTime();
                    data+=';';
                    data+=sensores[atoi(sensor.c_str())];
                    data+=';';
                    data+=direccion;
                    data+=';';
                    data+=chanel.get_mensaje();
                    
                    
				
				/*Envio de informacion a SD*/
		    do{
                          opcion=logger.escribir(data,sensores[atoi(sensor.c_str())]+".txt");
      		          if(opcion==1){
                          //2013/10/02 08:00:00;TEMP;1.1;100,00;100,00
      		          Serial.println("\n**********Guardado_en_SD**********\n");
                          Serial.print("*");
                          }
                          else{
                       	  Serial.println("**********Error_en_SD**********");
                          Serial.print("*");
                          }
                          intentos++;
                      }while(opcion==0 && intentos<5);
  		        /*Envio de informacion al servidor*/
		    Serial.println("**********Inicio_Enviando_informacion_a_Servidor**********\n");
                    Serial.print("*");
					router.enviar_por_red(data);
		    Serial.println("\n**********Fin_Enviando_informacion_a_Servidor**********\n");
                    Serial.println("FIN");
			/*********************************************************************/
				
                   chanel.delete_mensaje();
                   chanel.set_mensaje("1");
                   chanel.limpiar();
                   //Envio de mensaje confirmacion a nodo sensor, imprimir send y imprimir por UART1 mensaje enviado
                   //Serial.print("Send: ");
                   chanel.enviar_mensaje(4);
                   //chanel.enviar_mensaje(1);
                 }
                else{
                  chanel.delete_mensaje();
                   chanel.set_mensaje("0");
                   chanel.limpiar();
                   chanel.enviar_mensaje(4);
                   //Envio de mensaje para repetir accion
                   //chanel.enviar_mensaje(1);
                }
               }while(chanel.get_mensaje().equals("0"));
              chanel.delete_mensaje();
	}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

