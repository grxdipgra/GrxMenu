#include "usuarios/usuario.h"

usuario::usuario()
{

}

void usuario::carga_id (int id_entry_arg){
    entrada.id_entry=id_entry_arg;
}
void usuario::carga_usuario (QString nusuario_arg){
    entrada.nusuario=nusuario_arg;
}
void usuario::carga_nombre (QString nombre_arg){
    entrada.nombre=nombre_arg;
}
void usuario::carga_cambio_clave (QString cambio_clave_arg){
    entrada.cambio_clave=cambio_clave_arg;
}
void usuario::carga_ultimo_cambio_clave (QString cambio_clave_arg){
    entrada.ultimo_cambio_clave=cambio_clave_arg;
}
void usuario::carga_clave (QString clave_arg){
    entrada.clave=clave_arg;
}
void usuario::usuario::carga_correo (QString correo_arg){
    entrada.correo=correo_arg;
}
void usuario::carga_creada (QString creada_arg){
    entrada.creada=creada_arg;
}
void usuario::carga_estado (QString estado_arg){
    entrada.estado=estado_arg;
}
void usuario::carga_fecha_correo (QString fecha_correo_arg){
    entrada.fecha_correo=fecha_correo_arg;
}
void usuario::carga_intentos (int intentos_arg){
    entrada.intentos=intentos_arg;
}
void usuario::carga_logon (int logon_arg){
    entrada.logon=logon_arg;
}
void usuario::carga_modificacion_cuenta (QString modificacion_cuenta_arg){
    entrada.modificacion_cuenta=modificacion_cuenta_arg;
}
void usuario::carga_telefono (QString telefono_arg){
    entrada.telefono=telefono_arg;
}
void usuario::carga_ultimo_login (QString ultimo_login_arg){
    entrada.ultimo_login=ultimo_login_arg;
}
void usuario::carga_descripcion (QString descripcion_arg){
    entrada.descripcion=descripcion_arg;
}
void usuario::carga_caduca_clave (QString caduca_clave_arg){
    entrada.caduca_clave=caduca_clave_arg;
}
void usuario::carga_caduca_cuenta (QString caduca_cuenta_arg){
    entrada.caduca_cuenta=caduca_cuenta_arg;
}
void usuario::carga_dn (QString dn_arg){
    entrada.dn=dn_arg;
}
void usuario::carga_useraccountcontrol (int useraccountcontrol_arg){
    entrada.useraccountcontrol=useraccountcontrol_arg;
}
/*void usuario::carga_pass_nunca_expira (bool pass_no_ex){
    entrada.debe_cambiar_pass=pass_no_ex;
}*/

void usuario::carga_user(QString str){
    entrada.user=str;
}
void usuario::carga_user_preW2000(QString str){
    entrada.user_preW2000=str;
}
void usuario::carga_nombre_solo(QString str){
    entrada.nombre_solo=str;
}
void usuario::carga_apellidos(QString str){
    entrada.apellidos=str;
}
void usuario::carga_nombre_mostrar(QString str){
    entrada.nombre_mostrar=str;
}
void usuario::carga_movil(QString str){
    entrada.movil=str;
}
void usuario::carga_fax(QString str){
    entrada.fax=str;
}
void usuario::carga_departamento(QString str){
    entrada.departamento=str;
}
void usuario::carga_oficina(QString str){
    entrada.oficina=str;
}
void usuario::carga_organizacion(QString str){
    entrada.organizacion=str;
}
void usuario::carga_puesto(QString str){
    entrada.puesto=str;
}
void usuario::carga_provincia(QString str){
    entrada.provincia=str;
}
void usuario::carga_localidad(QString str){
    entrada.localidad=str;
}
void usuario::carga_direccion(QString str){
    entrada.direccion=str;
}
void usuario::carga_cp(QString str){
    entrada.cp=str;
}
void usuario::carga_notas(QString str){
    entrada.notas=str;
}
void usuario::carga_ultima_sincro(QString str){
    entrada.ultima_sincro=str;
}

void usuario::carga_grupos (QString grupo){
    entrada.vec_grupos.append(grupo);
}



int usuario::cual_es_id (){
    return(entrada.id_entry);
}
QString usuario::cual_es_usuario (){
    return(entrada.nusuario);
}
QString usuario::cual_es_nombre (){
    return(entrada.nombre);
}
QString usuario::cual_es_cambio_clave (){
    return(entrada.cambio_clave);
}
QString usuario::cual_es_ultimo_cambio_clave (){
    return(entrada.ultimo_cambio_clave);
}
QString usuario::cual_es_clave (){
    return(entrada.clave);
}
QString usuario::cual_es_correo (){
    return(entrada.correo);
}
QString usuario::cual_es_creada (){
    return(entrada.creada);
}
QString usuario::cual_es_estado (){
    return(entrada.estado);
}
QString usuario::cual_es_fecha_correo (){
    return(entrada.fecha_correo);
}
int usuario::cual_es_intentos (){
    return(entrada.intentos);
}
int usuario::cual_es_logon (){
    return(entrada.logon);
}
QString usuario::cual_es_modificacion_cuenta (){
    return(entrada.modificacion_cuenta);
}
QString usuario::cual_es_telefono (){
    return(entrada.telefono);
}
QString usuario::cual_es_ultimo_login (){
    return(entrada.ultimo_login);
}
QString usuario::cual_es_descripcion (){
    return(entrada.descripcion);
}
QString usuario::cual_es_caduca_clave (){
    return(entrada.caduca_clave);
}
QString usuario::cual_es_caduca_cuenta (){
    return(entrada.caduca_cuenta);
}
QString usuario::cual_es_dn (){
    return(entrada.dn);
}
int usuario::cual_es_useraccountcontrol (){
    return(entrada.useraccountcontrol);
}
/*bool usuario::cual_es_pass_nunca_expira (){
    return(entrada.pass_nunca_expira);
}*/

QString usuario::cual_es_user(){
    return(entrada.user);
}
QString usuario::cual_es_usuario_preW2000(){
    return(entrada.user_preW2000);
}
QString usuario::cual_es_nombre_solo(){
    return(entrada.nombre_solo);
}
QString usuario::cual_es_apellidos(){
    return(entrada.apellidos);
}
QString usuario::cual_es_nombre_mostrar(){
    return(entrada.nombre_mostrar);
}
QString usuario::cual_es_movil(){
    return(entrada.movil);
}
QString usuario::cual_es_fax(){
    return(entrada.fax);
}
QString usuario::cual_es_departamento(){
    return(entrada.departamento);
}
QString usuario::cual_es_oficina(){
    return(entrada.oficina);
}
QString usuario::cual_es_organizacion(){
    return(entrada.organizacion);
}
QString usuario::cual_es_puesto(){
    return(entrada.puesto);
}
QString usuario::cual_es_provincia(){
    return(entrada.provincia);
}
QString usuario::cual_es_localidad(){
    return(entrada.localidad);
}
QString usuario::cual_es_direccion(){
    return(entrada.direccion);
}
QString usuario::cual_es_cp(){
    return(entrada.cp);
}
QString usuario::cual_es_notas(){
    return(entrada.notas);
}
QString usuario::cual_es_ultima_sincro(){
    return(entrada.ultima_sincro);
}


QVector<QString> usuario::cual_es_grupos (){
    return(entrada.vec_grupos);
}



