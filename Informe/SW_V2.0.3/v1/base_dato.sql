
/*
crea base de datos
*/

create DATABASE Red_Sensorial;
use Red_Sensorial;

/*
 tabla ubicacion
*/
create table ubicacion(
idubicacion int not null primary key auto_increment,
nombre_lugar text not null
);

/*
 Tabla nodo_sensor
*/
create table nodo_sensor(
	idnodo_sensor  VARCHAR(50) not null,
	ubicacion int not null,
	unique (idnodo_sensor),
	constraint pk_nodo_sensorid primary key (idnodo_sensor,ubicacion),
	foreign key (ubicacion) references ubicacion(idubicacion)
	on delete cascade
	on update cascade

);

/*
 Tabla sensores
*/
create table sensores(
	idsensor int not null auto_increment,
	tipo text not null,
	marca text not null,
	tolerancia text not null,
	constraint pk_idsensor primary key (idsensor)
);

/*
Tabla nodo_posee_sensor
*/
create table nodo_posee_sensor(
	n_sensor VARCHAR(50) not null,
	sensor int not null,
	constraint pk_nodo_posse_sensor primary key(sensor,n_sensor),
	foreign key (n_sensor) references nodo_sensor(idnodo_sensor)
	on delete cascade
	on update cascade,
	foreign key (sensor) references sensores(idsensor)
	on delete cascade
	on update cascade
	
);

/*
Tabla historial
*/
create table historial(
	id_historia int not null auto_increment,
	ubicacion int not null,
	id_sensor int not null,
	lectura text not null,
	fecha DATETIME not null,
	id_n_sensor varchar(50) not null,
	constraint pk_historial primary key(id_historia,id_n_sensor,id_sensor),
	foreign key(id_n_sensor) references nodo_sensor(idnodo_sensor)
	on delete cascade
	on update cascade,
	foreign key(id_sensor) references sensores(idsensor)
	on delete cascade
	on update cascade,
	foreign key (ubicacion) references ubicacion(idubicacion)
	on delete cascade
	on update cascade
	 
);

/*
	inserciones
*/
insert into ubicacion (idubicacion, nombre_lugar)
values (null, 'parcela 13'), (null, 'parcela 12'), (null, 'parcela 11'), (null, 'parcela 10'), (null, 'parcela 9');

insert into sensores (idsensor, tipo, marca, tolerancia) 
values (null, 'TEMP', 'zx-thermometer', '3%'), (null, 'HUM', 'Groove', '5%'),
 (null, 'HUM', 'MC', '5%'), (null, 'HUM', 'dc-a', '5%'), (null, 'TEMP', 'Electronics', '5%');

insert into nodo_sensor (idnodo_sensor, ubicacion) 
values ('40B1DB6C', '1'), ('40A04CA5', '2');

insert into nodo_posee_sensor (n_sensor, sensor) 
values ('40B1DB6C', '1'), ('40B1DB6C', '2');

insert into historial(id_historia,ubicacion,id_sensor,lectura,fecha,id_n_sensor)
values(null,'1','2','23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;','2014-02-04 08:00:00','40B1DB6C'),
(null,'1','2','23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;23,00;','2014-02-04 08:00:00','40B1DB6C');

