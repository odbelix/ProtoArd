#include "comunicacion.h"
#include <string>
#include <iostream>
#include <SerialStream.h>
#include <stdlib.h>

using namespace LibSerial;
using namespace std;



comunicacion::comunicacion(){
	
}

/*Conectar por puerto USB a Arduino*/
int comunicacion::conectar(string port){
	/*quita salto de linea extra*/
	port.replace(port.length()-1,1,"\0");
	
	serial.Open(port);
	serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	serial.SetBaudRate(SerialStreamBuf::BAUD_9600);
	serial.SetNumOfStopBits(1);
	serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_SOFT);
	if(serial.good()){
		sleep(10);
		return 1;
	}
	else
		return 0;
}
string comunicacion::get_mensaje(){
	return mensaje;
	
}
void comunicacion::limpiar_mensaje(){
		
}
void comunicacion::enviar_mensaje(string msj){
	string salida;
	mensaje="";
	int a=0,b=1;
	if(msj.compare("Listar*")==0){
		if(serial.good()){
			sleep(5);
				serial << msj;
				do{
					serial >> salida;
					if(salida.compare("FIN") != 0 && salida.compare("Error_en_Scanner*") != 0){
							/*MAC*/
							a++;
							if(b==1){
								if(a==5){
									mensaje+=salida;
								}
								/*TIPO*/
								if(a==6){
									mensaje+=";";
									mensaje+=salida;
									mensaje+=";";
									a=1;
									b=0;
								}
							}
							else{
								if(a==9){
									mensaje+=salida;
								}
								/*TIPO*/
								if(a==10){
									mensaje+=";";
									mensaje+=salida;
									mensaje+=";";
									a=1;
								}
							}
						
					}
					cout << salida << endl;
				}while( salida.compare("FIN") != 0);
		}
		else{
			cout << "ERROR: No se puede abrir puerto." << endl;
		}
		serial.Close();
	}
	else{
		//~ cout << "mac;sensor"<<msj << endl;
		if(serial.good()){
			sleep(5);
				serial << msj;
				do{
					serial >> salida;
					if(salida.compare("FIN") != 0){
							
								mensaje+=" ";
								mensaje+=salida;
								mensaje+="\n";
				
					}
					cout << salida << endl;
				}while( salida.compare("FIN") != 0);
		}
		else{
			cout << "ERROR: No se puede abrir puerto." << endl;
		}
		serial.Close();
	}
}
