/*
	C socket server example, handles multiple clients using threads
	   run: gcc cliente.c -o version  `mysql_config --cflags --libs` -lpthread

*/

#include <string.h>	//strlen
#include <stdlib.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <pthread.h> 
#include <mysql.h>
#include <stdio.h>


void *connection_handler(void *);
int function_sql(char *);
char* function_parser(char *,char*);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
    if (argc < 3) {
    		printf("Error Faltan argumentos\n./server <ip> <port>\n");
    		return 0;
  	}	
	//Creacion del Socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Error al Crear Socket");
    }
    puts("Socket Creado");
	
	//Estructura para la direccion
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));
	
	//Conectar
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		// Error si no existe conexion
		perror("Error conexion fallida");
		return 1;
	}
	puts("Conexion Realizada");
	
	//Escucha en el socket
	listen(socket_desc , 3);
	
	//conecciones entrantes
	puts("Esperando por conexiones...");
	c = sizeof(struct sockaddr_in);
	
	
	//Para conexiones nuevas inicia un hebra
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Conexion Aceptada");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("Lo sentimos, Servidor Ocupado");
			return 1;
		}
		puts("Inicio Recepcion");
	}
	
	if (client_sock < 0)
	{
		perror("Conexion Rechazada");
		return 1;
	}
	
	return 0;
}

/*
 * Para los clientes que se conecten
 * */
void *connection_handler(void *socket_desc)
{
	//Obtener el descriptor del socket
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000],sql[3000];
	
	//Mensaje de Bienvenida al Cliente
	message = "Bienvenido\n*";
	write(sock , message , strlen(message));
	
	//Mensaje proveniente desde el cliente
	while( (read_size = read(sock , client_message , 2000 )) > 0 )
	{
		//Enviar mensaje al cliente
		if(client_message[strlen(client_message)-1]!='*'){
			//parsear informacion proveniente del cliente
			//FECHA;TIPO;UBICACION;LECTURAS;
			sprintf(sql,"%s",function_parser(client_message,sql));
			//Envia a servidor sql a ejecutar
			printf("SQL: %s\n",sql );
			//En caso de realizar consulta 
			if(function_sql(sql)){
					write(sock , "1" , 18);
			}
			else{
				write(sock,"0",27);
			}
			//setear variables String
			bzero(client_message,2000);
			bzero(sql,3000);
			
		}
		else{
			read_size=0;
			break;
		}
	}
	
	if(read_size == 0)
	{
		puts("Cliente Desconectado");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("Recepcion fallida");
	}
		
	//Liberar Socket
	free(socket_desc);
	
	return 0;
}

// Funcion para realizar consulta insert
int function_sql(char* sql) {
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;

   //IMPORTANTE//
   //Cambiar datos si es necesario
   char *server = "localhost";
   char *user = "root";
   char *password = "89189903"; 
   char *database = "prueba";
   //IMPORTANTE

   conn = mysql_init(NULL);
   //Conexion a la Base de Datos
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      return 0;
   }
   /* Enviar Query*/
   if (mysql_query(conn, sql)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      return 0;
   }
   else{
   		mysql_close(conn);
		return 1;
	}
}
// Funcion para parsear datos y retornar el sql listo
char* function_parser(char* str, char* sql){
    char result[5][1000];  
    bzero(result[0],1000);//FECHA
    bzero(result[1],1000);//TIPO
    bzero(result[2],1000);//MAC
    bzero(result[3],1000);//UBICACION
    bzero(result[4],1000);//LECTURAS
    char* token;
    int i=0,a=0;
    token = strtok(str, ";");
    while(token != NULL) {
        if(i<4){
        	strcpy(result[i], token);
    	}
    	else{
			if(a>0){
    		strcat(result[4], ";");
			}
			a++;
    		strcat(result[4], token);
    	}
        token = strtok(NULL, ";");
        i++;
    }
   sprintf(sql,"insert into historial values(null,'%s','%s','%s','%s','%s')",result[3],result[1],result[4],result[0],result[2]);
    free(token);
    return sql;
}
