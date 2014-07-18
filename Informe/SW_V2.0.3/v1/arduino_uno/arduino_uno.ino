
/*Bibliotecas Propias*/
#include "sensorTemperatura.h"
#include "sensorHumedad.h"
#include "sensorNivel.h"
#include "sensorflujo.h"
#include "almacen.h"
#include "comunicacion.h"

#define rele 4  //pin para rele
#define medidas 20 // cantidad de medidas a adquirir
/***********************************************************************/

/*Objetos de las clases*/
comunicacion chanel;			//Canal de comunicacion UART1
sensorTemperatura termometro;	//Objeto del Sensor de Temperatura
sensorflujo flujometro;			//Objeto del Sensor de Flujo
sensorNivel Nivel;				//Objeto del Sensor de Nivel de Agua
almacen contenedor;				//Almacen de las medidas para un sensor por vez
sensorHumedad medir_humedad;
/***********************************************************************/

/*Variables globales*/
int eleccion; //Sensor consultado
int cantidad=0,intentos=0; //cantidad de medidas, intentos por guardar, opcion a seguir luego de llamar a escribir en SD
/***********************************************************************/

/*Iniciar variable de Arduino*/
void setup(){
		/*Iniciar salida */
	Serial.begin(9600);


	/*Interrupcion para el sensor de flujo
	*requiere tiempo para lograr marcar un valor
	*/
	attachInterrupt(0, r, RISING);

	pinMode(rele, OUTPUT);  //Pin para Rele
     //Serial.print("Iniciado\n");
}

/*Ciclo de trabajo arduino*/
void loop(){
        /*Si existe activacion envia a UART1 menu*/
         if(eleccion==0){
            chanel.delete_mensaje();
            //recibo eleccion menu
            if(Serial.available()>0){
              while(chanel.recibir_mensaje(1)==0){
                
              }
              
              //set eleccion
              eleccion=atoi(chanel.get_mensaje().c_str());
              chanel.delete_mensaje();
            }
         }
          //Sensor de temperatura
         if(eleccion==2){
           //ciclo de trabajo para obtener medidas del sensor
           //max 889
	    while(cantidad<medidas){
              delay(100);
              contenedor.almacenar(termometro.mostrarMedida());
              cantidad++;
            }
        
            do{
                 
                 chanel.set_mensaje("2"+contenedor.enviar_medicion());
                 chanel.enviar_mensaje(1);
                 chanel.delete_mensaje();
                 //solo se libera si se confirma la recepcion
                while(chanel.recibir_mensaje(1)==0){
                 }
              }while(atoi(chanel.get_mensaje().c_str()) != 1);
              //
              contenedor.liberar();
              //reinicio cantidad con su valor 
              cantidad=0;
              eleccion=0;
         }
          //Sensor de Flujo
         if(eleccion==3){
           //ciclo de trabajo para obtener medidas del sensor
           //max 889
	    while(cantidad<medidas){
               flujometro.set_rpm();
               delay(1000);
      	       contenedor.almacenar(flujometro.mostrarMedida());
               cantidad++;
            }
            
              //enviar medicion
            do{
                 chanel.delete_mensaje();
                 chanel.set_mensaje("3"+contenedor.enviar_medicion());
                 chanel.enviar_mensaje(1);
                 chanel.limpiar();
                 chanel.delete_mensaje();
                 //solo se libera si se confirma la recepcion
                while(chanel.recibir_mensaje(1)==0){
                 }
              }while(atoi(chanel.get_mensaje().c_str()) != 1);
              //
              contenedor.liberar();
              //reinicio cantidad con su valor 
              cantidad=0;
              eleccion=0;
         }
          //Sensor de Nivel
         if(eleccion==4){
           
          //ciclo de trabajo para obtener medidas del sensor
           //max 889
	     while(cantidad<medidas){
              delay(100);
              contenedor.almacenar(Nivel.mostrarMedida());
              cantidad++;
            }
        
            do{
                 
                 chanel.set_mensaje("4"+contenedor.enviar_medicion());
                 chanel.enviar_mensaje(1);
                 chanel.delete_mensaje();
                 //solo se libera si se confirma la recepcion (implementar)
                while(chanel.recibir_mensaje(1)==0){
                 }
              }while(atoi(chanel.get_mensaje().c_str()) != 1);
              //
              contenedor.liberar();
              //reinicio cantidad con su valor 
              cantidad=0;
              eleccion=0;
         }
         if(eleccion==5){
            

          if(digitalRead(rele)==HIGH){
            digitalWrite(rele,LOW);
          }
          else
          digitalWrite(rele,HIGH);
           eleccion=0;
       }
       if(eleccion>5 || eleccion<0){
           eleccion=0;
       }    
         
}

/*Funcion ligada a la interrupcion*/
void r(){
  flujometro.rpm();
}


