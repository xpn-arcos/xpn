#include "xpn/xpn_simple/xpn_file.h"


struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];
 
int xpn_init_file_table(){
	int i;

	for(i=0;i<XPN_MAX_FILE;i++){		
		xpn_file_table[i] = NULL;
	}

	return 0;
}


int xpn_destroy_file_table(){
        int i,j;

        for(i=0;i<XPN_MAX_FILE;i++){
		  if(xpn_file_table[i] != NULL){
			for(j=0;j<xpn_file_table[i]->data_vfh->n_nfih;j++){
				if(xpn_file_table[i]->data_vfh->nfih[j] != NULL){
					if(xpn_file_table[i]->data_vfh->nfih[j]->priv_fh != NULL){
						free(xpn_file_table[i]->data_vfh->nfih[j]->priv_fh);
					}
				 	free(xpn_file_table[i]->data_vfh->nfih[j]);				
				}
			}
			
			free(xpn_file_table[i]->data_vfh->nfih);
			free(xpn_file_table[i]->data_vfh);
			free(xpn_file_table[i]->mdata->policy);
			free(xpn_file_table[i]->mdata);
			free(xpn_file_table[i]);

			xpn_file_table[i] = NULL;
		  }
        }

        return 0;
}
