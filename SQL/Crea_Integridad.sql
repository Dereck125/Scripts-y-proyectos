ALTER TABLE c_estado ADD CONSTRAINT pk_c_estado_id_cestado PRIMARY KEY (id_cestado);

ALTER TABLE c_municipio ADD CONSTRAINT pk_c_municipio_id_cmunicipio PRIMARY KEY (id_cmunicipio);
ALTER TABLE c_municipio ADD CONSTRAINT fk_c_municipio_id_cestado FOREIGN KEY (id_cestado) REFERENCES c_estado( id_cestado);

ALTER TABLE c_codigo_postal ADD CONSTRAINT pk_c_codigo_postal_codigo_postal PRIMARY KEY (codigo_postal);
ALTER TABLE c_codigo_postal ADD CONSTRAINT fk_c_codigo_postal_id_cmunicipio FOREIGN KEY (id_cmunicipio) REFERENCES c_municipio(id_cmunicipio);

ALTER TABLE c_colonia ADD CONSTRAINT pk_c_colonia_id_ccolonia PRIMARY KEY (id_ccolonia);
ALTER TABLE c_colonia ADD CONSTRAINT fk_c_colonia_codigo_postal FOREIGN KEY (codigo_postal) REFERENCES c_codigo_postal(codigo_postal);

ALTER TABLE domicilio ADD CONSTRAINT pk_domicilio_id_domicilio PRIMARY KEY (id_domicilio);
ALTER TABLE domicilio ADD CONSTRAINT fk_domicilio_id_ccolonia FOREIGN KEY (id_ccolonia) REFERENCES c_colonia(id_ccolonia);
ALTER TABLE persona ADD CONSTRAINT pk_persona_id_persona PRIMARY KEY (id_persona);

ALTER TABLE domicilio ADD CONSTRAINT fk_domicilio_id_persona FOREIGN KEY (id_persona) REFERENCES persona(id_persona);


ALTER TABLE telefono ADD CONSTRAINT pk_telefono_id_telefono PRIMARY KEY (id_telefono);
ALTER TABLE telefono ADD CONSTRAINT fk_telefono_id_persona FOREIGN KEY (id_persona) REFERENCES persona(id_persona);

ALTER TABLE empleado ADD CONSTRAINT pk_empleado_id_empleado PRIMARY KEY (id_empleado);
ALTER TABLE crol_empleado ADD CONSTRAINT pk_crol_empleado_id_crol PRIMARY KEY (id_crol);
ALTER TABLE empleado ADD CONSTRAINT fk_empleado_id_crol FOREIGN KEY (id_crol) REFERENCES crol_empleado(id_crol);
ALTER TABLE empleado ADD CONSTRAINT fk_empleado_id_persona FOREIGN KEY (id_persona) REFERENCES persona(id_persona);

ALTER TABLE cliente ADD CONSTRAINT pk_cliente_cliente PRIMARY KEY (id_cliente);
ALTER TABLE cliente ADD CONSTRAINT fk_cliente_id_persona FOREIGN KEY (id_persona) REFERENCES persona(id_persona);

ALTER TABLE orden_compra ADD CONSTRAINT pk_orden_compra_id_de_compra PRIMARY KEY (id_de_compra);
ALTER TABLE orden_compra ADD CONSTRAINT fk_orden_compra_id_cliente FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente);
ALTER TABLE c_estatus ADD CONSTRAINT pk_c_estatus_id_cestatus PRIMARY KEY (id_cestatus);
ALTER TABLE orden_compra ADD CONSTRAINT fk_orden_compra_id_cestatus FOREIGN KEY (id_cestatus) REFERENCES c_estatus(id_cestatus);

ALTER TABLE empleado_orden_compra ADD CONSTRAINT pk_empleado_orden_compra_id_empleado PRIMARY KEY (id_empleado,id_de_compra);
ALTER TABLE empleado_orden_compra ADD CONSTRAINT fk_empleado_orden_compra_id_empleado FOREIGN KEY (id_empleado) REFERENCES empleado(id_empleado);
ALTER TABLE empleado_orden_compra ADD CONSTRAINT fk_empleado_orden_compra_id_de_compra FOREIGN KEY (id_de_compra) REFERENCES orden_compra(id_de_compra);

ALTER TABLE orden_devolucion ADD CONSTRAINT pk_orden_devolucion_id_devolucion PRIMARY KEY (id_devolucion);
ALTER TABLE orden_devolucion ADD CONSTRAINT fk_orden_devolucion_id_de_compra FOREIGN KEY (id_de_compra) REFERENCES orden_compra(id_de_compra);
ALTER TABLE orden_devolucion ADD CONSTRAINT fk_orden_devolucion_id_cestatus FOREIGN KEY (id_cestatus) REFERENCES c_estatus(id_cestatus);


ALTER TABLE cuenta_cliente ADD CONSTRAINT pk_cuenta_cliente_id_cuenta PRIMARY KEY (id_cuenta);
ALTER TABLE cuenta_cliente ADD CONSTRAINT fk_cuenta_cliente_id_cliente FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente);

ALTER TABLE orden_envio ADD CONSTRAINT pk_orden_envio_id_envio PRIMARY KEY (id_envio);
ALTER TABLE orden_envio ADD CONSTRAINT fk_orden_envio_id_cestatus FOREIGN KEY (id_cestatus) REFERENCES c_estatus(id_cestatus);
ALTER TABLE orden_envio ADD CONSTRAINT fk_orden_envio_id_de_compra FOREIGN KEY (id_de_compra) REFERENCES orden_compra(id_de_compra);

ALTER TABLE orden_pago ADD CONSTRAINT pk_orden_pago_id_orden_de_pago PRIMARY KEY (id_orden_de_pago);
ALTER TABLE orden_pago ADD CONSTRAINT fk_orden_pago_id_de_compra FOREIGN KEY (id_de_compra) REFERENCES orden_compra(id_de_compra);
ALTER TABLE orden_pago ADD CONSTRAINT fk_orden_pago_id_cestatus FOREIGN KEY (id_cestatus) REFERENCES c_estatus(id_cestatus);
ALTER TABLE modo_de_pago ADD CONSTRAINT pk_modo_de_pago_id_modo_de_pago PRIMARY KEY (id_modo_de_pago);
ALTER TABLE orden_pago ADD CONSTRAINT fk_orden_pago_id_modo_de_pago FOREIGN KEY (id_modo_de_pago) REFERENCES modo_de_pago (id_modo_de_pago);

ALTER TABLE producto ADD CONSTRAINT pk_producto_sku_producto PRIMARY KEY (sku_producto);
ALTER TABLE c_formato ADD CONSTRAINT pk_c_formato_id_cformato PRIMARY KEY (id_cformato);
ALTER TABLE producto ADD CONSTRAINT fk_producto_id_cformato FOREIGN KEY (id_cformato) REFERENCES c_formato(id_cformato);
ALTER TABLE c_departamento_genero ADD CONSTRAINT pk_c_departamento_genero_id_cdepartamento PRIMARY KEY (id_cdepartamento);
ALTER TABLE producto ADD CONSTRAINT fk_producto_id_cdepartamento FOREIGN KEY (id_cdepartamento) REFERENCES c_departamento_genero(id_cdepartamento);

ALTER TABLE producto_orden_compra ADD CONSTRAINT pk_producto_orden_compra_sku_producto PRIMARY KEY (sku_producto,id_de_compra);
ALTER TABLE producto_orden_compra ADD CONSTRAINT fk_producto_orden_compra_sku_producto FOREIGN KEY (sku_producto) REFERENCES producto(sku_producto);
ALTER TABLE producto_orden_compra ADD CONSTRAINT fk_producto_orden_compra_id_de_compra FOREIGN KEY (id_de_compra) REFERENCES orden_compra(id_de_compra);

ALTER TABLE c_interprete ADD CONSTRAINT pk_c_interprete_id_cinterprete PRIMARY KEY (id_cinterprete);

ALTER TABLE producto_cinterprete ADD CONSTRAINT pk_producto_cinterprete_id_cinterprete PRIMARY KEY (id_cinterprete,sku_producto);
ALTER TABLE producto_cinterprete ADD CONSTRAINT fk_producto_cinterprete_id_cinterprete FOREIGN KEY (id_cinterprete) REFERENCES c_interprete(id_cinterprete);
ALTER TABLE producto_cinterprete ADD CONSTRAINT fk_producto_cinterprete_sku_producto FOREIGN KEY (sku_producto) REFERENCES producto(sku_producto);

