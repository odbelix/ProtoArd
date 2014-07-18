///////////////////////////////////////////////////////////////////////////////// 
//   ARCHIVO: RelojDS1307.h
//     AUTOR: Fernando Santiago
// PROPOSITO: Manejar y controlar el reloj basado en el DS1307, para la gestion de fecha y hora.
//   VERSION: 0.3.00
//    E_MAIL: fsantiago@softweb.es
//     FECHA: 30/11/11
////////////////////////////////////////////////////////////////////////////////

#ifndef RelojDS1307_h
#define RelojDS1307_h

#include "Arduino.h"

class RelojDS1307Class
{
  int vSegundos;
  int vMinutos;
  int vHora;
  int vDiaSemana;
  int vDia;
  int vMes;
  int vAnyo;
  void SetDatos();
  void GetDatos();
  private:
  public:
   RelojDS1307Class();
   void Reset();
   void SetDateTime( int monthDay, int month, int year, int hour, int minute, int second );
   void GetDateTime( int &monthDay, int &month, int &year, int &hour, int &minute, int &second );
   char * DateTime();
   char * DateTime2();
   char * Date();
   char * Time();
   int GetHour();
   int GetMinute();
   int GetYear();
   int GetMonth();
   int GetDay();
   void SetDiaSemana( int DiaSemana );
   int  GetDiaSemana();
   char * DiaName();
   char * MesName();
   void IncMinutos(int &hour, int &minute, int incremento );
   int IncHoras(int hour, int incremento );
   boolean IsLater( int hour, int minute ); 
   boolean IsPrevious( int hour, int minute );
   int Change_Date(String Date);
};

byte bcdToDec(byte val);  
byte decToBcd(byte val);

extern RelojDS1307Class RelojDS1307;

#endif 

