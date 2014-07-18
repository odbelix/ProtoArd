/*COMPILAR: g++ ventana.cpp ventana.h main.cpp -o examplewindow `pkg-config gtkmm-3.0 --cflags --libs` `mysql_config --cflags --libs`*/

#include <gtkmm.h>
#include "ventana.h"
#include <iostream>

using namespace std;
using namespace Gtk;

int main(int argc, char *argv[])
{
	cout << "Iniciando..." << endl;

	Main kit(argc, argv);

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(argv[1]);

	ventana *ventana_principal = 0;

	builder->get_widget_derived("Red Sensores", ventana_principal);

	kit.run(*ventana_principal);
	cout << "Gracias por utilizar RS_v0.1" << endl;

  return 0;
}
