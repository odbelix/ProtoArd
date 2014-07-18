#ifndef ventana_h
#define ventana_h
#include <gtkmm.h>
#include <iostream>
#include <string>
using namespace std;


class ventana : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Button *buscar;
	Gtk::Button *seleccionar;
	Gtk::Button *medir;
	Gtk::Button *port;
	Gtk::ComboBoxText *puertos;
	Gtk::ComboBoxText *sensores;
	Gtk::ComboBoxText *mac;
	Gtk::ProgressBar *barra;
	Gtk::TextView *texto;

public:
	ventana(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor
	string canal;
	string mac_elegida;
	string sensor_elegido;
	string peticion;
	string buffer_texto;
	bool m_bActivityMode;
	sigc::connection conn;
	
protected:
	int valor;
	
	Glib::RefPtr<Gtk::TextBuffer> buffer;

	//Seniales
	
	/*Busca los puertos*/
	void cargar_puertos();
	
	/*Agrega los puertos disponibles en sistema*/
	void agregar_puertos();
	
	/*Eleccion el puerto a trabajar*/
	void eleccion();
	
	/*Ordena Buscar Dispositivos */
	void on_buscar();
	
	/*Edita la barra de progreso*/
	bool barra_progreso();
	
	/*Agrega Mac Encontradas en un combobox*/
	void agregar_dispositivos();

	/*Selecciona la Mac para obtener sensores conectados*/
	void on_seleccionar();
	
	/*SObre lista*/
	void seleccion_dispositivo();
	
	/*Agrega Sensores encontrados en BD del equipo elegido*/
	void agregar_sensores();
	
	/*Ordena obtener medidas del equipo y sensor elegido*/
	void on_medir();
	
	/*Preparar orden de peticion de datos*/
	string preparar_peticion();
	
	/*Escribir en Textview*/
	void escribir();
	
};
#endif
