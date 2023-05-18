--Script con un ejemplo de cada una de los siguientes tipos de consulta


--básica (solo incluye una tabla)
SELECT *
FROM domicilio
where id_persona = 35;

--subconsulta (al menos 3 niveles)

--El modo de pago que más se uso
SELECT tipo_de_tarjeta
FROM modo_de_pago
WHERE id_modo_de_pago IN (SELECT id_modo_de_pago
                          FROM (SELECT id_modo_de_pago, count(id_orden_de_pago) TOTAL
                                FROM orden_pago
                                group by id_modo_de_pago) T2
                          WHERE T2.TOTAL IN (SELECT max(TOTAL)
                                             FROM (SELECT id_modo_de_pago, count(id_orden_de_pago) TOTAL
                                                   FROM orden_pago
                                                   group by id_modo_de_pago) T1));


-- compuesta(3 tablas al menos deben estar involucradas en un JOIN)

--Se quiere conocer el nombre, app, apm, costo_total, id_modo_de_pago de la compra más alta.

SELECT nombre, apellido_paterno, apellido_materno, costo_total, id_modo_de_pago
FROM orden_compra
         LEFT JOIN orden_pago ON orden_compra.id_de_compra = orden_pago.id_de_compra
         JOIN cliente ON orden_compra.id_cliente = cliente.id_cliente
         JOIN persona ON cliente.id_persona = persona.id_persona
where costo_total = (SELECT max(costo_total)
                     FROM orden_compra
                              LEFT JOIN orden_pago ON orden_compra.id_de_compra = orden_pago.id_de_compra);



--paginación
-- Los datos de 150 empleados
SELECT id_crol, rfc, correo, nombre, apellido_paterno
FROM empleado
         JOIN persona ON empleado.id_persona = persona.id_persona
ORDER BY id_crol ASC
OFFSET 50 FETCH NEXT 150 ROWS ONLY;



--CROSSTAB

--Nos genera una tabla que nos dice cuántos productos hay organizados por el tipo de formato y su genero/departamento.
SELECT *
FROM crosstab('SELECT departamento_genero,tipo_de_formato,total_sum
FROM(
SELECT DISTINCT T2.id_cdepartamento,departamento_genero,tipo_de_formato,SUM(cantidad) over(partition by (T2.id_cdepartamento,tipo_de_formato)) as total_sum
       FROM (producto
                inner join c_formato cf on producto.id_cformato = cf.id_cformato) T2
         inner join c_departamento_genero on T2.id_cdepartamento = c_departamento_genero.id_cdepartamento  order by id_cdepartamento) as aux',
              'SELECT DISTINCT tipo_de_formato FROM c_formato ORDER BY tipo_de_formato') AS T1 (departamento_genero varchar,
                                                                                                cassette INT, CD INT,
                                                                                                digital INT, vinil INT);



--función de ventana--

--PROMEDIO DE COSTO TOTAL DE VENTA POR MES
SELECT DISTINCT MES, PROMEDIO_DE_VENTA
FROM (SELECT extract(MONTH FROM fecha_de_orden)                                      MES,
             costo_total,
             avg(costo_total) OVER (PARTITION BY extract(MONTH FROM fecha_de_orden)) PROMEDIO_DE_VENTA
      FROM orden_compra
               JOIN cliente c on c.id_cliente = orden_compra.id_cliente) T1
ORDER BY MES;



--Agrupación (debe resolver el mismo problema que la función deventana)--

--PROMEDIO DE COSTO TOTAL DE VENTA POR MES
SELECT extract(MONTH FROM fecha_de_orden) MES, avg(costo_total) prom_costo
FROM orden_compra
GROUP BY MES
ORDER BY MES;

