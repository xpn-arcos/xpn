#!/bin/bash

#############################################################################
#                                                                           #
# Este script crea una copia de seguridad de varios directorios importantes #
#                                                                           #
#############################################################################

REPOSITORIO=$HOME/repositorio

DATE=`date +%Y-%m-%d_%H-%M`

FILE_BACKUP_DIR_INTERCEPT="intercept_completo_$DATE.tar.gz"
FILE_BACKUP_DIR_XPN_10="expand-1.0_$DATE.tar.gz"
FILE_BACKUP_DIR_XPN_20="expand-2.0_$DATE.tar.gz"

DIR_INTERCEPT=$HOME/intercept
DIR_XPN_10=$HOME/expand-1.0
DIR_XPN_20=$HOME/expand-2.0

tar zcf $REPOSITORIO/$FILE_BACKUP_DIR_INTERCEPT $DIR_INTERCEPT
tar zcf $REPOSITORIO/$FILE_BACKUP_DIR_XPN_10 $DIR_XPN_10
tar zcf $REPOSITORIO/$FILE_BACKUP_DIR_XPN_20 $DIR_XPN_20

