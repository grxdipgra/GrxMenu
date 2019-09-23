//*************************************************************************************
// CLASE que representa a un usuario del dominio
// según se representa en la base de datos
// permite consultar sus datos y modificarlos
// incluido los grupos a los que pertenece
//*************************************************************************************

#ifndef USUARIO_H
#define USUARIO_H

#include <QString>
#include <QVector>

//#include <QtDebug>

class usuario
{

public:

    usuario();

    void carga_id (int id_entry_arg);
    void carga_usuario (QString usuario_arg);
    void carga_nombre (QString nombre_arg);
    void carga_cambio_clave (QString cambio_clave_arg);
    void carga_clave (QString clave_arg);//no hace falta no se guarda en la BD
    void carga_correo (QString correo_arg);
    void carga_creada (QString creada_arg);
    void carga_estado (QString estado_arg);
    void carga_fecha_correo (QString fecha_correo_arg);
    void carga_intentos (int intentos_arg);
    void carga_logon (int logon_arg);
    void carga_modificacion_cuenta (QString modificacion_cuenta_arg);
    void carga_telefono (QString telefono_arg);
    void carga_ultimo_login (QString ultimo_login_arg);
    void carga_descripcion (QString descripcion_arg);
    void carga_caduca_clave (QString caduca_clave_arg);
    void carga_caduca_cuenta (QString caduca_cuenta_arg);
    void carga_dn (QString dn_arg);
    void carga_useraccountcontrol (int useraccountcontrol_arg);
    void carga_ultimo_cambio_clave (QString cambio_clave_arg);
    //void carga_pass_nunca_expira (bool pass_no_ex);

    void carga_user(QString str);
    void carga_user_preW2000(QString str);
    void carga_nombre_solo(QString str);
    void carga_apellidos(QString str);
    void carga_nombre_mostrar(QString str);
    void carga_movil(QString str);
    void carga_fax(QString str);
    void carga_departamento(QString str);
    void carga_oficina(QString str);
    void carga_organizacion(QString str);
    void carga_puesto(QString str);
    void carga_provincia(QString str);
    void carga_localidad(QString str);
    void carga_direccion(QString str);
    void carga_cp(QString str);
    void carga_notas(QString str);
    void carga_ultima_sincro(QString str);
    void carga_grupos (QString grupo);

    int cual_es_id ();
    QString cual_es_usuario ();
    QString cual_es_nombre ();
    QString cual_es_cambio_clave ();
    QString cual_es_clave ();//no hace falta no se guarda en la BD
    QString cual_es_correo ();
    QString cual_es_creada ();
    QString cual_es_estado ();
    QString cual_es_fecha_correo ();
    int cual_es_intentos ();
    int cual_es_logon ();
    QString cual_es_modificacion_cuenta ();
    QString cual_es_telefono ();
    QString cual_es_ultimo_login ();
    QString cual_es_descripcion ();
    QString cual_es_caduca_clave ();
    QString cual_es_caduca_cuenta ();
    QString cual_es_dn ();
    int cual_es_useraccountcontrol ();
    QString cual_es_ultimo_cambio_clave ();
    //bool cual_es_pass_nunca_expira ();

    QString cual_es_user();
    QString cual_es_usuario_preW2000();
    QString cual_es_nombre_solo();
    QString cual_es_apellidos();
    QString cual_es_nombre_mostrar();
    QString cual_es_movil();
    QString cual_es_fax();
    QString cual_es_departamento();
    QString cual_es_oficina();
    QString cual_es_organizacion();
    QString cual_es_puesto();
    QString cual_es_provincia();
    QString cual_es_localidad();
    QString cual_es_direccion();
    QString cual_es_cp();
    QString cual_es_notas();
    QString cual_es_ultima_sincro();

    QVector<QString> cual_es_grupos ();


private:


    struct entry{
            int id_entry;
            QString nusuario;
            QString nombre;
            QString cambio_clave;
            QString clave;//no hace falta no se guarda en la BD
            QString correo;
            QString creada;
            QString estado;
            QString fecha_correo;
            int intentos;
            int logon;
            QString modificacion_cuenta;
            QString telefono;
            QString ultimo_login;
            QString descripcion;
            QString caduca_clave;
            QString caduca_cuenta;
            QString dn;
            int useraccountcontrol;
            QString ultimo_cambio_clave;

            QString user;
            QString user_preW2000;
            QString nombre_solo;
            QString apellidos;
            QString nombre_mostrar;
            QString movil;
            QString fax;
            QString departamento;
            QString oficina;
            QString organizacion;
            QString puesto;
            QString provincia;
            QString localidad;
            QString direccion;
            QString cp;
            QString notas;
            QString ultima_sincro;

            QVector<QString> vec_grupos;

    }entrada;

};

#endif // USUARIO_H
