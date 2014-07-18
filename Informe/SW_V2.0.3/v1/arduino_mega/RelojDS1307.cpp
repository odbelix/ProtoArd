///////////////////////////////////////////////////////////////////////////////// 
//   ARCHIVO: RelojDS1307.cpp
//     AUTOR: Fernando Santiago
// PROPOSITO: Manejar y controlar el reloj basado en el DS1307, para la gestion de fecha y hora.
//   VERSION: 0.3.00
//    E_MAIL: fsantiago@softweb.es
//     FECHA: 30/11/11 
//   Modificado por Gonzalo Muñoz
//       MOdificacion
//          Agrega funcion para verificar hora retorna 1 si la hora consultada 
//          es la del reloj 0 en caso contrario
//          Fecha: 28/01/14
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include "RelojDS1307.h"

#define DS1307_ADDRESS 0x68

char * aDiasSemana[8] = {"NULO","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};
char * aMeses[13] = {"NULO","Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};

///////////////////////////////////////////////////////////////
//
// CONTRUCTOR
//
///////////////////////////////////////////////////////////////
RelojDS1307Class::RelojDS1307Class()
{
  Wire.begin();
}
	
///////////////////////////////////////////////////////////////
//
// RESETEAR EL DS1307
//
///////////////////////////////////////////////////////////////	
void RelojDS1307Class::Reset()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();	
}	

///////////////////////////////////////////////////////////////
//
// FUNCCION INTERNA, ASIGNA LOS DATOS FECHA Y HORA AL DS1307
//
///////////////////////////////////////////////////////////////
void RelojDS1307Class::SetDatos()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(byte(0x00)); //stop Oscilador
  Wire.write(decToBcd(vSegundos));
  Wire.write(decToBcd(vMinutos));
  Wire.write(decToBcd(vHora));  
  Wire.write(decToBcd(vDiaSemana)); // Dia de la semana
  Wire.write(decToBcd(vDia));
  Wire.write(decToBcd(vMes));
  Wire.write(decToBcd(vAnyo));
  Wire.write(byte(0x00)); //start 
  Wire.endTransmission();		
}		

///////////////////////////////////////////////////////////////
//
// FUNCCION INTERNA, OBTIENE LOS DATOS FECHA Y HORA DEL DS1307
//
///////////////////////////////////////////////////////////////
void RelojDS1307Class::GetDatos()
{
  // Reset the register pointer
  Reset();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  vSegundos = bcdToDec(Wire.read());
  vMinutos = bcdToDec(Wire.read());
  vHora = bcdToDec(Wire.read() & 0b111111); //24 hour time
  vDiaSemana = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  vDia = bcdToDec(Wire.read());
  vMes = bcdToDec(Wire.read());
  vAnyo = bcdToDec(Wire.read());
}

///////////////////////////////////////////////////////////////
//
// FUNCION INTERNA, PASA DE BCD A DECIMAL
//
///////////////////////////////////////////////////////////////
byte bcdToDec(byte val)  
{
  return ( (val/16*10) + (val%16) );
}

///////////////////////////////////////////////////////////////
//
// FUNCION INTERNA, PASA DE DECIMAL A BCD
//
///////////////////////////////////////////////////////////////
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
	
///////////////////////////////////////////////////////////////
//
// ASIGNAMOS EL DIA DE LA SEMANA 
//
///////////////////////////////////////////////////////////////	
void RelojDS1307Class::SetDiaSemana( int DiaSemana )
{
  if ((DiaSemana < 0) || (DiaSemana > 6)) return;
  GetDatos();
  vDiaSemana = DiaSemana;
  SetDatos();
}	

///////////////////////////////////////////////////////////////
//
// OBTENEMOS EL DIA DE LA SEMANA
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetDiaSemana()
{
  GetDatos();
  return vDiaSemana; 
}
	
///////////////////////////////////////////////////////////////
//
// OBTENEMOS EL NOMBRE DEL DIA DE LA SEMANA
//
///////////////////////////////////////////////////////////////	
char * RelojDS1307Class::DiaName()
{
  GetDatos();
  return aDiasSemana[vDiaSemana];	
}

///////////////////////////////////////////////////////////////
//
// OBTENEMOS EL NOMBRE DEL MES
//
///////////////////////////////////////////////////////////////
char * RelojDS1307Class::MesName()
{
  GetDatos();
  return aMeses[vMes];	
}

///////////////////////////////////////////////////////////////
//
// ASIGNAMOS LA FECHA Y HORA 
//
///////////////////////////////////////////////////////////////			
void RelojDS1307Class::SetDateTime( int year,int month,int monthDay, int hour, int minute, int second )
{
  GetDatos();
  vSegundos = second;
  vMinutos = minute;
  vHora = hour;
  vDiaSemana = vDiaSemana;
  vDia = monthDay;
  vMes = month;
  vAnyo = year;	
  SetDatos();
}

///////////////////////////////////////////////////////////////
//
// OBTENEMOS LA FECHA Y HORA
//
///////////////////////////////////////////////////////////////
void RelojDS1307Class::GetDateTime( int &monthDay, int &month, int &year, int &hour, int &minute, int &second )
{
  GetDatos();
  second = vSegundos;
  minute = vMinutos;
  hour = vHora;
  monthDay = vDia;
  month = vMes;
  year = vAnyo;	
}  
  
///////////////////////////////////////////////////////////////
//
// DEVUELVE LA FECHA Y HORA COMPLETA EN FORMATO TEXTO
//
///////////////////////////////////////////////////////////////  
char * RelojDS1307Class::DateTime()
{
  static char tmp[25];
  GetDatos();
  sprintf(tmp, "%02d/%02d/%02d %02d:%02d:%02d\0",vAnyo,vMes,vDia, vHora,vMinutos,vSegundos); 
  return tmp;
}
///////////////////////////////////////////////////////////////
//
// DEVUELVE LA FECHA Y HORA COMPLETA EN FORMATO TEXTO
//
///////////////////////////////////////////////////////////////  
char * RelojDS1307Class::DateTime2()
{
  static char tmp[25];
  GetDatos();
  sprintf(tmp, "%02d/%02d/%02d %02d:%02d\0", vDia,vMes,vAnyo, vHora,vMinutos); 
  return tmp;
}
///////////////////////////////////////////////////////////////
//
// DEVUELVE LA FECHA COMPLETA EN FORMATO TEXTO
//
///////////////////////////////////////////////////////////////
char * RelojDS1307Class::Date()
{
  static char tmp[12];
  GetDatos();
  sprintf(tmp, "%02d/%02d/%02d", vDia,vMes,vAnyo); 
  return tmp;
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE LA HORA COMPLETA EN FORMATO TEXTO
//
///////////////////////////////////////////////////////////////
char * RelojDS1307Class::Time()
{
  static char tmp[12];
  GetDatos();
  sprintf(tmp, "%02d:%02d:%02d", vHora,vMinutos,vSegundos); 
  return tmp;
}		

///////////////////////////////////////////////////////////////
//
// DEVUELVE SOLO LA HORA EN UN ENTERO
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetHour()
{
  GetDatos();
  return vHora; 
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE SOLO LOS MINUTOS EN UN ENTERO
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetMinute()
{
  GetDatos();
  return vMinutos;
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE SOLO EL A�O EN UN ENTERO
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetYear()
{
  GetDatos();
  return vAnyo;
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE SOLO EL MES EN UN ENTERO
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetMonth()
{
  GetDatos();
  return vMes;
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE SOLO EL DIA EN UN ENTERO
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::GetDay()
{
  GetDatos();
  return vDia;
}

///////////////////////////////////////////////////////////////
//
// DEVUELVE SI LA HORA Y MINUTOS ACTUAL ES SUPERIOR A LOS PASADOS  
//
///////////////////////////////////////////////////////////////
boolean RelojDS1307Class::IsLater( int hour, int minute )
{
  boolean Res = false;
  int Hora = hour;

  GetDatos();
  if ((vHora < 8) && (hour > 23)) Hora = (hour-24);
  if ( (vHora > Hora) ) Res = true;
  if ( (vHora == Hora) && (vMinutos > minute)) Res = true;	 	

  return Res;	
}	
 
///////////////////////////////////////////////////////////////
//
// DEVUELVE SI LA HORA Y MINUTOS ACTUAL ES INFERIOR A LOS PASADOS  
//
/////////////////////////////////////////////////////////////// 
boolean RelojDS1307Class::IsPrevious( int hour, int minute )
{
  boolean Res = false;
  int Hora = hour;  

  GetDatos();
  if ((vHora < 8) && (hour > 23)) Hora = (hour-24);  
  if ( (vHora < Hora) ) Res = true;
  if ( (vHora == Hora) && (vMinutos < minute)) Res = true;	 	

  return Res;
}

///////////////////////////////////////////////////////////////
//
// INCREMENTA LA HORA Y MINUTOS PASADOS EN LA CANTIDA DE MINUTOS PASADOS 
//
///////////////////////////////////////////////////////////////
void RelojDS1307Class::IncMinutos(int &hour, int &minute, int incremento )
{
  if ((minute + incremento) > 59) 
    {
	hour++;
    if (hour >= 24) { hour = (hour - 24); }
	minute = (minute + incremento) - 60; 
	}
  else
    {
	minute = (minute + incremento); 
	}
}

///////////////////////////////////////////////////////////////
//
// INCREMENTA LA HORA Y MINUTOS PASADOS EN LA CANTIDA DE HORAS PASADAS 
//
///////////////////////////////////////////////////////////////
int RelojDS1307Class::IncHoras(int hour, int incremento )
{
  if ((hour + incremento) >= 24) { return (hour + incremento) - 24; }
  if ((hour + incremento) < 24) { return (hour + incremento); }
}
///////////////////////////////////////////////////////////////
//
// MODIFICA FECHA SI NO CORRESPONDE A ACTUAL 
//  FORMATO: DD/MM/AA HH:MM
///////////////////////////////////////////////////////////////
int RelojDS1307Class::Change_Date(String Date){
  if(Date.equals((String)DateTime2())){
    
    return 1;
  }
  else{
    return 0;
  }
}
