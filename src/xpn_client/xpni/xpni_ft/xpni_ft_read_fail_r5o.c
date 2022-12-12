

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_read.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_sread_fail_r5o
      (
         int fd,  
         void *buffer,  
         off_t offset,  
         size_t size
      )
      {

        int ret ;
        int block_index1;      /* Index of associated first data block */
        //int block_offset1;     /* Offset inside associated to first data block */
        int block_index2;      /* Index of associated last data block */
        //int block_offset2;     /* Offset inside associated to last data block */
        int SPi;               /* Server for Parity */
        int IPi;               /* Index for Parity */
        int SDi;               /* Server for Data */
        int IDi;               /* Index for Data */
        fmeta_t fmeta;         /* File metadata (extended version) */
	char *buffer_row;
	int toRecover, i, j;

        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_sread_r5o(%d,%p,%d,%d); \n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size);
        #endif


        fmeta = xpni_fit_get_XPN_FMETA(fd) ;
        
        block_index1   = (offset)      / fmeta.block_size ;
        //block_offset1  = (offset)      % fmeta.block_size ;
        block_index2   = (offset+size) / fmeta.block_size ;
        //block_offset2  = (offset+size) % fmeta.block_size ;

	toRecover = 0 ;
	for (i=block_index1; i<block_index2; i++)
	{
           ret = MATH_MISC_locateInRAID5withInternalParity(i,
                                                           fmeta.servers_count,
                                                           &SPi,&IPi,&SDi,&IDi);
	   if (1 == SDi) { 
	       toRecover++ ;
	   }
	}

	if (0 == toRecover)
	{
		ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
					 buffer,
					 (int)offset,
					 size) ;
	}
	else
	{
		buffer_row = (char *)malloc(fmeta.block_size*fmeta.servers_count);
		if (NULL == buffer_row)
		            return (-1) ;
		memset(buffer_row,0,fmeta.block_size*fmeta.servers_count);

	        for (i=0; i<toRecover; i++)
		{
	                for (j=0; j<fmeta.servers_count-1; j++) 
			{
				ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
							 buffer_row,
							 (int)0,
							 fmeta.block_size) ;
			}

			ret = MATH_MISC_Xor(buffer_row,
			                    buffer_row,
				            buffer_row,
				            fmeta.block_size);

		        ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
					         buffer,
					         (int)offset,
					         size) ;
		}

		free(buffer_row);
	}

	return ret ;
      }


   /* ................................................................... */

