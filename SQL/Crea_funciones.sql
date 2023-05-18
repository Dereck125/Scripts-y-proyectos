

-- Regresa todos los productos con inventario mayor a x


CREATE OR REPLACE FUNCTION fn_cont_productos(cant integer)
    RETURNS TABLE
            (
                sku_producto        integer,
                nombre              varchar(128),
                tipo_de_formato     varchar(128),
                departamento_genero varchar(128)
            )
AS
$$
DECLARE

BEGIN

    RETURN QUERY
    SELECT producto.sku_producto, producto.nombre, c_formato.tipo_de_formato, c_departamento_genero.departamento_genero
    FROM c_departamento_genero
             INNER JOIN producto ON producto.id_cdepartamento = c_departamento_genero.id_cdepartamento
             INNER JOIN c_formato ON c_formato.id_cformato = producto.id_cformato
    WHERE cantidad > cant;
END;
$$

VOLATILE LANGUAGE 'plpgsql';


SELECT *
    FROM fn_cont_productos(49);





--Funcion que regresa el precio del producto mas caro dependiendo del genero
CREATE OR REPLACE FUNCTION fnc_maxxgen(
    pgenero varchar(128)
)
    RETURNS integer
AS
$$
DECLARE
BEGIN
    IF (pgenero NOT IN (SELECT departamento_genero
                        FROM c_departamento_genero))
    THEN
        RETURN 0;
    ELSE
        RETURN (SELECT DISTINCT MAX(precio)
                FROM producto
                         JOIN c_departamento_genero cdg on cdg.id_cdepartamento = producto.id_cdepartamento
                WHERE departamento_genero LIKE pgenero);
    end if;
END;
$$
    LANGUAGE 'plpgsql';

--PRUEBA
SELECT * FROM fnc_maxxgen('Disco')






-- EJEMPLO DE UNA FUNCIÓN QUE REALIZA UNA ACCIÓN A SU BD

--CREAR UNA FUNCION QUE REGISTRE A UNA NUEVA PERSONA

CREATE OR REPLACE FUNCTION fnc_registrapersona(
    pcorreo       varchar(50),
    psnombre      varchar(64),
    psapm         varchar(32),
    psapp         varchar(32),
    pcfecha       DATE
)
    RETURNS varchar(64)
AS
$$
DECLARE vidpersona     INTEGER;

        estado_de_insercionpersona varchar (32);
BEGIN
    IF ((SELECT COUNT(*)
         FROM persona
         WHERE nombre = psnombre
           AND   apellido_materno = psapm
           AND   apellido_paterno = psapp
           AND   fecha_de_nacimiento = pcfecha
        )=0)
    THEN
        vidpersona = ((SELECT max(id_persona) FROM persona) +1);
        INSERT INTO persona(id_persona,correo, nombre, apellido_materno, apellido_paterno, fecha_de_nacimiento)
        VALUES (vidpersona,pcorreo, upper(psnombre), upper(psapm), upper(psapp), pcfecha);
        estado_de_insercionpersona = 'insercion_exitosapersona';
    ELSE
        estado_de_insercionpersona = 'Esta_persona_ya_esta_registrada';
    END IF;


    RETURN estado_de_insercionpersona;

END;
$$
    LANGUAGE 'plpgsql' volatile ;


--Prueba

SELECT * FROM fnc_registrapersona('carlos3445@gmail.com','carlos','lopez','portillo','2002-12-03');
SELECT * FROM persona;