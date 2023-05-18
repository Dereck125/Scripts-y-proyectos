CREATE TABLE c_estado(
  id_cestado SERIAL,
  nombre_estado varchar(40)
 );

CREATE TABLE c_municipio(
  id_cmunicipio SERIAL,
  nombre_municipio varchar(128),
  id_cestado integer
  );


CREATE TABLE c_codigo_postal(
  codigo_postal integer,
  id_cmunicipio integer
  );

CREATE TABLE c_colonia(
  id_ccolonia SERIAL,
  nombre_colonia varchar(40),
 codigo_postal integer
);


CREATE TABLE domicilio(
  id_domicilio SERIAL,
  numero_exterior integer,
  numero_interior integer,
  calle            varchar(75),
  id_ccolonia integer,
  id_persona integer
);

CREATE TABLE persona(
  id_persona SERIAL,
  correo	varchar(128),
  nombre	varchar(64),
  apellido_materno	varchar(32),
  apellido_paterno	varchar(32),
  fecha_de_nacimiento   date
);

CREATE TABLE telefono(
  id_telefono SERIAL,
  n_telefono  numeric(10,0),
  id_persona integer
);


CREATE TABLE empleado(
  id_empleado SERIAL,
  rfc varchar(128),
  id_crol integer,
  id_persona integer
);

CREATE TABLE crol_empleado(
  id_crol SERIAL,
  nombre_rol varchar(128)
);

CREATE TABLE cliente(
  id_cliente SERIAL,
  id_persona integer
);

CREATE TABLE orden_compra(
  id_de_compra SERIAL,
  fecha_de_orden date,
  costo_total   integer,
  id_cliente integer,
  id_cestatus integer
  );


CREATE TABLE empleado_orden_compra(
    id_empleado integer,
    id_de_compra integer
);

CREATE TABLE orden_devolucion(
  id_devolucion SERIAL,
  fecha_devolucion date,
  id_de_compra integer,
  id_cestatus integer
 );


CREATE TABLE c_estatus(
  id_cestatus SERIAL,
  nombre_estatus varchar(16)
);

CREATE TABLE cuenta_cliente(
  id_cuenta SERIAL,
  usuario varchar(128),
  password varchar(128),
  id_cliente integer
);

CREATE TABLE orden_envio(
  id_envio SERIAL,
  fecha_de_envio date,
  fecha_de_entrega date,
  id_cestatus integer,
  id_de_compra integer
);

CREATE TABLE orden_pago(
  id_orden_de_pago SERIAL,
  no_de_tarjeta NUMERIC(16,0),
  fecha_de_pago date,
  id_de_compra integer,
  id_cestatus integer,
  id_modo_de_pago integer
);

CREATE TABLE modo_de_pago(
  id_modo_de_pago SERIAL,
  tipo_de_tarjeta varchar(16)
  );

CREATE TABLE producto(
  sku_producto integer,
  cantidad integer,
  nombre    varchar(40),
  origen    varchar(20),
  precio    integer,
  id_cformato integer,
  id_cdepartamento integer
  );

CREATE TABLE producto_orden_compra(
  id_de_compra integer,
  sku_producto integer,
  cantidad      integer
  );


CREATE TABLE c_formato(
  id_cformato SERIAL,
  tipo_de_formato varchar(40)
);

CREATE TABLE c_departamento_genero(
  id_cdepartamento SERIAL,
  departamento_genero varchar(128)
);

CREATE TABLE c_interprete(
id_cinterprete SERIAL,
nombre_interprete varchar(42)
 );


CREATE TABLE producto_cinterprete(
 id_cinterprete integer,
 sku_producto integer
);
