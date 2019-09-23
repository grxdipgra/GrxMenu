#ifndef LIB_H
#define LIB_H

#define NUM_COL_MUNICIPIO_ID 0
#define NUM_COL_MUNICIPIO_IDCOMARCA 1
#define NUM_COL_MUNICIPIO_NOMBRE 2
#define NUM_COL_MUNICIPIO_NOMBREID 3
#define NUM_COL_MUNICIPIO_CODIGOINE 4
#define NUM_COL_MUNICIPIO_NUMEROHABITANTES 5
#define NUM_COL_MUNICIPIO_SUPERFICIE 6
#define NUM_COL_MUNICIPIO_ALTITUD 7
#define NUM_COL_MUNICIPIO_LATITUD 8
#define NUM_COL_MUNICIPIO_LONGITUD 9
#define NUM_COL_MUNICIPIO_URLBANDERA 10
#define NUM_COL_MUNICIPIO_URLESCUDO 11
#define NUM_COL_MUNICIPIO_WEB 12
#define NUM_COL_MUNICIPIO_SIGLASPARTIDO 13
#define NUM_COL_MUNICIPIO_NOMBREPARTIDO 14
#define NUM_COL_MUNICIPIO_NOMBREALCALDE 15
#define NUM_COL_MUNICIPIO_FECHAELECCIONES 16

#define NUM_COL_NODO_ID 0
#define NUM_COL_NODO_IDPOBLACION 1
#define NUM_COL_NODO_IDCENTRO 2
#define NUM_COL_NODO_NOMBRE 3
#define NUM_COL_NODO_CIF 4
#define NUM_COL_NODO_CODIGODIR3 5
#define NUM_COL_NODO_TIPOVIA 6
#define NUM_COL_NODO_NOMBREDIRECCION 7
#define NUM_COL_NODO_NUMERODIRECCION 8
#define NUM_COL_NODO_LETRADIRECCION 9
#define NUM_COL_NODO_ESCALERADIRECCION 10
#define NUM_COL_NODO_PISODIRECCION 11
#define NUM_COL_NODO_PUERTADIRECCION 12
#define NUM_COL_NODO_CODIGOPOSTAL 13
#define NUM_COL_NODO_LATITUD 14
#define NUM_COL_NODO_LONGITUD 15
#define NUM_COL_NODO_CONTACTO 16
#define NUM_COL_NODO_EXTENSION 17
#define NUM_COL_NODO_FAX 18
#define NUM_COL_NODO_WEB 19
#define NUM_COL_NODO_SEDE 20
#define NUM_COL_NODO_URLTABLON 21
#define NUM_COL_NODO_URLPORTALTRANSPARENCIA 22
#define NUM_COL_NODO_ADSLLINEA 23
#define NUM_COL_NODO_NUMADMINISTRATIVO 24
#define NUM_COL_NODO_IPLINEA 25
#define NUM_COL_NODO_IPCIFRADO 26
#define NUM_COL_NODO_SERVICIOLINEA 27
#define NUM_COL_NODO_CAUDAL 28
#define NUM_COL_NODO_EQUIPAMIENTO 29
#define NUM_COL_NODO_NUMEROSERIEROUTER 30
#define NUM_COL_NODO_DNS1 31
#define NUM_COL_NODO_DNS2 32
#define NUM_COL_NODO_PROXY 33
#define NUM_COL_NODO_ESAYUNTAMIENTO 34

#define NUM_COL_PROGRAMA_ID 0
#define NUM_COL_PROGRAMA_ANIO 1
#define NUM_COL_PROGRAMA_PORTALWEB 2
#define NUM_COL_PROGRAMA_EMAIL 3
#define NUM_COL_PROGRAMA_BASEDATOSJURIDICA 4
#define NUM_COL_PROGRAMA_SUSCRIPCIONDOMINIO 5
#define NUM_COL_PROGRAMA_PERFILCONTRATANTE 6
#define NUM_COL_PROGRAMA_GESTIONMUNICIPAL 7
#define NUM_COL_PROGRAMA_GESTIONECONOMICA 8
#define NUM_COL_PROGRAMA_SOPORTE 9
#define NUM_COL_PROGRAMA_SEDEELECTRONICA 10
#define NUM_COL_PROGRAMA_EPOL 11
#define NUM_COL_PROGRAMA_EPOLMOVIL 12
#define NUM_COL_PROGRAMA_SIAPOL 13

#define NUM_COL_POBLACION_ID 0
#define NUM_COL_POBLACION_IDMUNICIPIO 1
#define NUM_COL_POBLACION_IDELA 2
#define NUM_COL_POBLACION_NOMBRE 3
#define NUM_COL_POBLACION_NOMBREID 4

#define NUM_COL_TELEFONO_ID 0
#define NUM_COL_TELEFONO_TELEFONO 1

#define NUM_COL_CENTRO_ID 0
#define NUM_COL_CENTRO_NOMBRE 1
#define NUM_COL_CENTRO_DIRECCION 2
#define NUM_COL_CENTRO_TELEFONO 3

#define NUM_COL_MANCOMUNIDAD_ID 0
#define NUM_COL_MANCOMUNIDAD_NOMBRE 1

#define NUM_COL_COMARCA_ID 0
#define NUM_COL_COMARCA_NOMBRE 1

#define NUM_COL_EMAILNODO_ID 0
#define NUM_COL_EMAILNODO_EMAIL 1

#define NUM_COL_DIAFESTIVOPOBLACION_ID 0
#define NUM_COL_DIAFESTIVOPOBLACION_DATE 1
#define NUM_COL_DIAFESTIVOPOBLACION_DESCRIPCION 2

#define NUM_COL_APLICACIONES_ID 0
#define NUM_COL_APLICACIONES_ATALAYA 1
#define NUM_COL_APLICACIONES_GLPI 2
#define NUM_COL_APLICACIONES_OCS 3

#define WEB_ATALAYA 0
#define WEB_GLPI 1

#define RUTA_IMAGENES "/usr/share/grx/asistencia/imagenes/"
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QDateTime>

bool fileExists(QString path);
bool dirExists(QString path);
char* convierte(QString dato);
unsigned int puerto_libre();
int estado_habilitado(int valor);
bool es_fecha(QString fecha);
QString dominio_basedn();
QString gtime_to_string(QString fecha);

#endif // LIB_H
