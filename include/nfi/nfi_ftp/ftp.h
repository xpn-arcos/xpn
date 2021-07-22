#ifndef _FTP_H_
#define _FTP_H_
	    

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_ftp/list_dir.h"

#ifdef  __cplusplus
    extern "C" {
#endif

#define FTPPATHLEN 1024

#define OK "200 "
#define ERR_SIN "500 "
#define ERR_ARG "501 "
#define ORD_NO_NEC "202 "
#define ERR_ORD "502 "
#define ORD_INC "503 "
#define ERR_PAR "504 "
#define PASS_NEED "331 "
#define OK_PASS_NEED "220 "
#define OK_PASS_ACEP "230 "
#define PASV_ACEP "227 "
#define FICH_OK "150 "
#define CLOSE_DATOS "226 "
#define NO_DISP "550 "
#define ACC_FICH_OK "250 "
#define CREA_DIR_OK "257 "
#define QUIT_OK "221 "
#define REST_OK "350 "
#define ABOR_OK "226 "
#define ABOR_OK_DATA "426 "
#define SYST_OK "215 "
#define TRANS_ABOR "451"
#define LOCAL_TRANS_ABOR "450"


/*************************************************
Revisar los sitios donde ponga FTP o ftp
****************************************************************************/


int ftp_create_connection_control(char *name, int port);

void ftp_close_connection_control(int sc);

int ftp_create_connection_data(char *name, int port);

void ftp_close_connection_data(int sc);


void send_command(char * comand, int socket);
int receive_answer(int socket, char * respuest);
int receive_partial_data(int socket, char * respuest, int tam);
int get_port(char * respuest);
int get_filename(char * ruta);

int ftp_abor(int socket);
int ftp_login(int socket, char *login, char *pass, char * respuest);
int ftp_list(char * Server,int socket, struct list_dir *lista,char * ruta,char * respuest);
int ftp_store(char * Server,int socket,char * ruta, int pos, int tam, char * buffer, char * ruta_local, int f,char * exe_dir);

int ftp_retr(char * Server,int socket,char * ruta, int pos, int tam, char * respuest);
int ftp_cwd(int socket,char * ruta, char * respuest);
int ftp_mkd(int socket,char * ruta, char * respuest);
int ftp_rmd(int socket,char * ruta, char * respuest);
int ftp_rename(int socket,char * ruta, char * ruta2, char * respuest);
int ftp_dele(int socket,char * ruta, char * respuest);
int ftp_cdup(int socket, char * respuest);
int ftp_syst(int socket);
int ftp_noop(int socket);
int ftp_quit(int socket);

int seek_data_list(struct element_dir *list,char * file);
char * seek_list(struct element_dir *list,int pos);
int fill_struct(struct list_dir *list, char * respuest);

 #ifdef  __cplusplus
     }
 #endif


#endif /* _FTP_H */

