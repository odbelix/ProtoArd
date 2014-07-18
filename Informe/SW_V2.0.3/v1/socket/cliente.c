
#include<stdio.h>
#include<string.h>   
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include<mysql/mysql.h>
    
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    if (argc < 3) {
    		printf("Error Faltan argumentos\n./server <ip> <port>\n");
    		return 0;
  	}	
    //Creacion del Socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Error al Crear Socket");
    }
    puts("Socket Creado");
     //Estructura para la direccion
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[2]) );
 
    //Conexion remota al servidor
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");

    //Recibe mensaje del servidor "Bienvenido"
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            
        }
        else{
        	puts(server_reply);
        	bzero(server_reply,2000);
        }
     
    //mantener comunicacion con servidor
    while(1)
    {
        printf("Envie mensaje : ");
        gets(message);
         
        //Enviar data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Envio fallido");
            return 1;
        }
        if(message[strlen(message)-1]!='*'){ 
        //Recibe mensaje servidor
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("Recepcion fallida");
            break;
        }
         
        	puts("Servidor responde :");
        	puts(server_reply);
    	}
    	else{
    		break;
    	}
        bzero(server_reply,2000);
        bzero(message,1000);
    }
     
    close(sock);
    return 0;
}

