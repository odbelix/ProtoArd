#ifndef comunicacion_h
#define comunicacion_h
#include <string>
#include <iostream>
#include <SerialStream.h>

using namespace LibSerial;
using namespace std;



class comunicacion{
public:
		comunicacion();
		int conectar(string port);
		string get_mensaje();
		void limpiar_mensaje();
		void enviar_mensaje(string msj);	
	int valor;
	string mensaje;
	SerialStream serial;
};
#endif
