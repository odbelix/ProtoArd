#include "comando.h"
#include "Arduino.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define error_scanner 3
comando::comando(){
  dispositivos=0;
    
}
comando::~comando(){
  pinMode(error_scanner,OUTPUT);
}
void comando::modo_comando(){
  //Serial.print("\r\n----------MODO COMANDO----------*\n");
  while(Serial3.available()>0){Serial3.read();}
   while(Serial3.available()==0){
    delay(1000);
   // Serial.println("\r\n+++");
    Serial3.print("+++");
    delay(1000);
  }
  leer_data();
  //escribir_data();

}

/*
void comando::leer_configuracion(){
      Serial.println("\r\n--------Lectura_configuracion_xbee --------");
      
      Serial.println("\r\nATID");  
      Serial3.println("\r\nATID");  
      leer_data();
      escribir_data();
      Serial.println("\r\nATCH");  
      Serial3.println("\r\nATCH"); 
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATMY");  
      Serial3.println("\r\nATMY");  
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATSH");  
      Serial3.println("\r\nATSH");  
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATSL"); 
      Serial3.println("\r\nATSL"); 
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATDH"); 
      Serial3.println("\r\nATDH"); 
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATDL"); 
      Serial3.println("\r\nATDL"); 
      leer_data();
      escribir_data();
      
      Serial.println("\r\nATBD"); 
      Serial3.println("\r\nATBD"); 
      leer_data();
      escribir_data();
}
*/

void comando::configurar_parametro(String DL,String DH){
    modo_comando();
   // Serial.print("\r\n-------- Configuracion_nueva--------*\n");
   
   //Serial.print("\r\nATDH"+(String)DH+"*\n");     
   Serial3.println("\r\nATDH"+DH);    
   leer_data();
   //escribir_data();
    
   // Serial.print("\r\nATDL"+(String)DL+"*\n");   
    Serial3.println("\r\nATDL"+(String)DL);    
    leer_data();
   // escribir_data();
    
    //Serial.print("\r\nATWR*\n");   
    Serial3.println("\r\nATWR");    
    leer_data();
   // escribir_data();
    salir_modo_comando();
    //Serial.print("\r\n----------*\n");

}

void comando::salir_modo_comando(){
  Serial3.println("\r\nATCN");
  leer_data();
  //escribir_data();
  //Serial.print("\r\n----------Escape modo comando----------*\n");
  delay(1000);
}

void comando::reset(){
    Serial.print("\r\n----------RESET----------*\n");
    Serial.print("\r\nATRE*\n");
    Serial3.println("\r\nATRE");
    leer_data();
    escribir_data();

    delay(1000);
}

void comando::limpiar(){
  Serial3.flush();
}

void comando::leer_data(){
    byte data1;
    data="";
    int a=0;
    while(Serial3.available()==0 && a<700){a++;delay(5);}
    while(Serial3.available()>0){
    data1=Serial3.read();
    delay(10);
    data+=(char)data1;  
    }
    limpiar();
    a=0;    
}

void comando::escribir_data(){
    Serial.print(data);
}

void comando::explorar_red(){
    int a,error;
   configurar_parametro("FFFF","0");
    do{
      delay(10000);
      error=0;
      modo_comando();
      //Serial.println("\r\nATND"); 
      Serial3.println("\r\nATND");
      a=0;
      /*****CAMBIAR VALOR PARA REGISTRAR MAS DISPOSITIVOS*******/
      while(a<limite_dispositivos){
          
          leer_data();
          tabla[dispositivos]="";
		/***47 CANTIDAD DE CARACTERES TRAIDOS EN RESPUESTA DE XBEE****/
          if(strlen(get_data().c_str())==47){
            tabla[dispositivos]=get_data();
            dispositivos++;
          }
          else{
            if(strlen(get_data().c_str())>47){
              error=1;
            }
          }
        delay(10); 
       a++; 
      }
    delay(100);
    if(error){
      Serial.print("Error_en_Scanner*\n");
      digitalWrite(error_scanner,HIGH);
      delay(3000);
      digitalWrite(error_scanner,LOW);
    }
    dispositivos=0;
    }while(error);
    //Serial.print("Scanner correcto*\n");

}

String comando::get_direccion(int a){
  return tabla[a];
}

String comando::get_data(){
  return data;
}
