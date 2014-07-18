gcc server.c -o server  `mysql_config --cflags --libs` -lpthread
gcc cliente.c -o cliente  `mysql_config --cflags --libs` -lpthread
