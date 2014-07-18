#include "ventana.h"
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include "comunicacion.h"
#include <list>
#include <boost/algorithm/string/replace.hpp>
#include <mysql.h>
#define SERVER "localhost"
#define USER "root"
#define PASSWORD "89189903"
#define DATABASE "prueba"
using namespace std;
using namespace Gtk;

ventana::ventana(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Window(cobject), builder(refGlade),m_bActivityMode(false){
		  set_title("Red de Sensores");
		  
		  /*Union con el archivo de glade*/
		  builder->get_widget("buscar", buscar);
		  builder->get_widget("seleccionar", seleccionar);
		  builder->get_widget("medir", medir);
		  builder->get_widget("puertos",puertos);
		  builder->get_widget("mac",mac);
		  builder->get_widget("sensores",sensores);
		  builder->get_widget("barra",barra);
		  builder->get_widget("texto",texto);
		  builder->get_widget("port",port);
		  
		  /*Señales para los botones*/
		  buscar->signal_clicked().connect(sigc::mem_fun(*this, &ventana::on_buscar));
		  seleccionar->signal_clicked().connect(sigc::mem_fun(*this, &ventana::on_seleccionar));
		  medir->signal_clicked().connect(sigc::mem_fun(*this, &ventana::on_medir));
		  port->signal_clicked().connect(sigc::mem_fun(*this, &ventana::cargar_puertos));
		  
	
		  /*Señales para carga los combobox dependientes*/
		  puertos->signal_changed().connect(sigc::mem_fun(*this,&ventana::eleccion) );
		  mac->signal_changed().connect(sigc::mem_fun(*this,&ventana::seleccion_dispositivo) );
		  
		  /*Crea el buffer*/
          buffer = Gtk::TextBuffer::create();

}

void ventana::on_buscar(){
	mac->remove_all();
	sensores->remove_all();
	if(!(canal.empty())){
		//std::cout << "Puerto Buscado: " << canal << std::endl;

		conn=Glib::signal_timeout().connect(sigc::mem_fun(*this,&ventana::barra_progreso), 5 );
		int a=0;
		agregar_dispositivos();
	}
	
}
void ventana::agregar_dispositivos(){
    mac->remove_all();
	sensores->remove_all();
	string word;

	/* conectar con arduino por usb elegido
	 * enviarle a arduino orden para buscar 
	 * dispositivos, obtener respuesta
	 * procesarla, obtener mac de los equipos
	 * agregar mac en la lista*/
	comunicacion enlace;
	string enviar;
	enviar="/dev/";
	enviar+=canal;
	if(enlace.conectar(enviar)){
		enlace.enviar_mensaje("Listar*");
		word=enlace.get_mensaje();
		/*Extraer MAC*/
		list<string> mylist;
		list<string>::iterator it;
		
		 char * cstr = new char [word.length()+1];
		 strcpy (cstr, word.c_str());

		 char * p = strtok (cstr,";");
		 while (p!=0){
			mylist.push_back (p);
			p = strtok(NULL,";");
		  }
		int a=0;
		for (it=mylist.begin(); it!=mylist.end(); ++it){
			if(a%2==0){
			mac->append(*it);
			}
			a++;
		}
		
		  delete[] cstr;
	}
	else{
   		  std::cout << "Error en conexion" << std::endl;
	}
	
}
void ventana::on_seleccionar(){
	 Glib::ustring text = mac->get_active_text();
	if(!(text.empty())){
		mac_elegida=text;
		agregar_sensores();
		barra->set_fraction(0.0);
		conn.disconnect();
	}


}
/*Incluida en documentacion como listar sensores*/
void ventana::agregar_sensores(){
	sensores->remove_all();
	string word;
	char sql[250];
	sprintf(sql,"select sensores.idsensor,sensores.tipo from nodo_posee_sensor, sensores where nodo_posee_sensor.n_sensor='%s' and nodo_posee_sensor.sensor=sensores.idsensor",mac_elegida.c_str());
    MYSQL *connect; 
    connect=mysql_init(NULL); 
    if(!connect){
        fprintf(stderr,"No se puede conectar con Base de Dato");
    }
    connect=mysql_real_connect(connect,"localhost","root","89189903","prueba",0,NULL,0);
    MYSQL_RES *res_set; 
    MYSQL_ROW row; 
      
    mysql_query(connect,sql);
    unsigned int i = 0;
    res_set = mysql_store_result(connect);
  
    while ((row = mysql_fetch_row(res_set)) != NULL){
		 for(int i = 0; i < 2; i++) 
		  { 
			if(row[i] != NULL ){
				if(i==0){
					word+=row[i];
					word+=";";
				}
				if(i==1)
					word+=row[i];

			} 
		  }
		  /*Agrega en combobox sensores que posee equipo
		   * Segun Base de dato*/
		  sensores->append(word);
		  word=""; 
	}	
          
           mysql_close(connect);
    	//~ /*FIN CONSULTA*/


   
    
}

void ventana::on_medir(){
	Glib::ustring text = sensores->get_active_text();
	string word;
	if(!(text.empty())){
		sensor_elegido=text;
		peticion=preparar_peticion();
		//std::cout << "Peticion: " << peticion << std::endl;
		/* Aqui se debe interacturar con clase que maneje 
		 * la interaccion con el arduino por el puerto USB
		 * elegido
		 * */
		comunicacion enlace;
		string enviar;
		enviar="/dev/";
		enviar+=canal;
		if(enlace.conectar(enviar)){
			enlace.enviar_mensaje(peticion);
			word=enlace.get_mensaje();
			buffer_texto=word;
		}
		else{
			  std::cout << "Error en conexion" << std::endl;
		}
			 
			escribir();
		}
		

}

string ventana::preparar_peticion(){
	string orden;
	orden=mac_elegida;
	orden+=";";
	orden+=sensor_elegido.substr(0,1);
	orden+="*";
	return orden;
}

void ventana::seleccion_dispositivo(){
	 Glib::ustring text = mac->get_active_text();
	if(!(text.empty())){
		mac_elegida=text;
	}
	sensores->remove_all();

	
}

void ventana::eleccion(){
	 Glib::ustring text = puertos->get_active_text();
	if(!(text.empty())){
		canal=text;
	}
    mac->remove_all();
	sensores->remove_all();

	
}

bool ventana::barra_progreso()
{
	if(m_bActivityMode){
		barra->pulse();
	}
	else{
		double new_val = barra->get_fraction() + 0.00005;
		if(new_val > 1.0){
			barra->set_fraction(0.0);
			conn.disconnect();
		}
		else{
		barra->set_fraction(new_val);
	   }
	}
	return true;
		

}

	/* Listar puertos USB conectado con arduino
	 * esta consultas se debe hacer sobre el pc
	 */
void ventana::agregar_puertos(){
	
	puertos->remove_all();
	mac->remove_all();
	sensores->remove_all();
	buffer->set_text("");
	texto->set_buffer(buffer);
	system("ls /dev/ | grep 'ttyACM' >puertos.txt");
	FILE *port;
	port=fopen("puertos.txt","r");
    char linea[128];
    linea[0]='\0';
    char *c;
    if(port == NULL)
		cerr << "Error al abrir el archivo puertos.txt" << endl;
    else
    do{
    
        c=fgets(linea, sizeof(linea),port);
        if(linea !=NULL && linea[0]!='\0'){
			puertos->append(linea);
		}
        linea[0]='\0';
    }while(c != NULL);
    fclose(port);

	

}

void ventana::cargar_puertos(){
	agregar_puertos();
}

void ventana::escribir(){
	string area_texto;
	area_texto=buffer->get_text();
	area_texto+="\n"+buffer_texto;
	/*Reescribe con lo que hay el buffer*/
	//buffer_texto+="\n"+	buffer->get_text();
	replace( area_texto.begin(), area_texto.end(), '_', ' ');
	buffer->set_text(area_texto);
	texto->set_buffer(buffer);
}
