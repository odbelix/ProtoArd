/* 
 * File:   red.h
 * Author: gonzalo
 *
 */
#ifndef RED_H
#define	RED_H
#include <SPI.h>
#include <Ethernet.h>
class red{
        public:
	red();
	~red();
        void set_mac(byte* mac);
        void set_ip_local(byte* ip);
        void set_ip_server(byte* ip);
        void set_mask(byte* mask);
        void set_port_server(int port);
        int iniciar_red();
        int leer_mensaje();
        void enviar_mensaje();
        void borrar_mensaje();
        String get_mensaje();
        void set_mensaje(String msg);
        int server_disponible();
        int procedimiento(String msg);
        void enviar_por_red(String data);
        void error_red(int op);
        private:
            byte *mac;
            byte *ip;
            byte *ip_local;
            byte *ip_server;
            byte *mask;
            int port_server;
            String mensaje;
            EthernetClient client;
};


#endif	/* RED_H */
