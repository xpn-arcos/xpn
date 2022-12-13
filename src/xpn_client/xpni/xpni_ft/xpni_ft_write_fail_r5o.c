
   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_write.h"

/* eslint-disable no-use-before-define */
/* eslint-disable no-unused-vars */
/* eslint-disable block-scoped-var */


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_swrite_fail_r5o
      (
        int fd, 
        void *buffer, 
        off_t offset, 
        size_t size
      )
      {
        ssize_t ret;           /* Returned value from called functions */
        int block_index1;      /* Index of associated first data block */
        int block_offset1;     /* Offset inside associated to first data block */
        int block_index2;      /* Index of associated last data block */
        int block_offset2;     /* Offset inside associated to last data block */
        //int SA;                /* Server will be read/write */
        int SPi;               /* Server for Parity */
        int IPi;               /* Index for Parity */
        int SDi;               /* Server for Data */
        int IDi;               /* Index for Data */
        int parity_indexi;     /* Index of associate parity block */
        int parity_offseti;    /* Offset inside associate parity block */
        int data_indexi;       /* Index of associate data block */
        int data_offseti;      /* Offset inside associate data block */
        int end_indexi;        /* Index of associate end block */
        int end_offseti;       /* Offset inside associate end block */
        int begin_indexi;      /* Index of associate begin data block */
        int begin_offseti;     /* Offset inside associate begin data block */
        char *buffer_i;        /* Auxiliar pointer */
        char *buffer_parity;   /* Buffer for parity block */
        char *buffer_data;     /* Buffer for data block */
        char *block_bufferi;   /* Aditional auxiliar pointer */
        char *buffer_row;      /* Buffer for a row */
        fmeta_t fmeta;         /* File metadata (extended version) */
        int rows;              /* Rows between block_index1 and block_index2 */
        int row;               /* Auxiliar counter for rows */
        int block_i;           /* Auxiliar counter for blocks */
        int offseti;           /* Auxiliar counter for data block offset */
        int block_offseti;     /* Auxiliar counter for data block offset */
	int       aux2;        /* Auxiliar for some intermediate computations */
	int to_write;          /* Auxiliar for how many bytes write in a block */
	int to_write2;         /* Auxiliar for how many bytes write in total */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_swrite_r5o(%d,%p,%d,%d); \n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size);
        #endif


        /* 
         * Locate logical block position (first and last blocks)
         */                                             
        
        fmeta = xpni_fit_get_XPN_FMETA(fd) ;
        
        block_index1   = (offset)      / fmeta.block_size ;
        block_offset1  = (offset)      % fmeta.block_size ;
        block_index2   = (offset+size) / fmeta.block_size ;
        block_offset2  = (offset+size) % fmeta.block_size ;


        /* 
         * Alloc memory
         */                                             

        buffer_i = (char *)buffer;

        buffer_row = (char *)malloc(fmeta.block_size*fmeta.servers_count);
        if (NULL == buffer_row)
            return (-1) ;


        /* 
         * Read physical block positions
         */                                             

	/* Physical rows to read... */
	aux2 = (fmeta.servers_count - 1) ;
	rows = (block_index2 / aux2) - (block_index1 / aux2) + 1;

        block_i       = block_index1;
	block_offseti = block_offset1;

        for (row=0; row<rows; row++)
        {
            ret = MATH_MISC_locateInRAID5withExternalParity(block_i,
                                                            fmeta.servers_count,
                                                            &SPi,&IPi,&SDi,&IDi);
    
            parity_indexi  = IPi * fmeta.servers_count + SPi ;
            data_indexi    = block_i ;
	    begin_indexi   = data_indexi - (data_indexi % aux2) ; /* (data_indexi / aux2) * aux2 ; */

	    /* end_indexi is not included */
	    if ((rows - 1) == row) /* última fila*/
	         end_indexi = block_index2 + (block_offset2 != 0) ;
	    else
	         end_indexi = begin_indexi + aux2 ; 

            begin_offseti  = begin_indexi  * fmeta.block_size;
            end_offseti    = end_indexi    * fmeta.block_size;
            parity_offseti = parity_indexi * fmeta.block_size;
            data_offseti   = data_indexi   * fmeta.block_size;

            /* read full data row */
/* <Server 1 will fail ...> */
            ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
                                    buffer_row,
                                    begin_offseti,
                                    fmeta.block_size*aux2);

            /* read old parity for this row */
/* <Server 1 will fail ...> */
            buffer_parity = buffer_row + fmeta.block_size*aux2;
            ret = xpni_lowfsi_pread(xpni_fit_get_XPN_MDATA_FD(fd),
                                    buffer_parity,
                                    parity_offseti,
                                    fmeta.block_size);

            /* for each data block: write new parity + write data */
            for 
            (
                 offseti  = data_offseti; 
                 offseti != end_offseti; 
                 offseti += fmeta.block_size
            )
            {
               /* get old data */
               buffer_data = buffer_row + (offseti - begin_offseti) ;

               /* compute new parity */
               ret = MATH_MISC_Xor(buffer_parity,
                                   buffer_parity,
                                   buffer_data,
                                   fmeta.block_size);

	       /* by default: write data from buffer_i (piece from buffer param)... */
               block_bufferi = buffer_i;

	       /* compute how many bytes will be written. */
	       to_write  = fmeta.block_size - block_offseti; /* bytes left to be written in a block */

	       if (offseti > offset)
	           to_write2 = size - (offseti - offset); /* total bytes left to be written */
	       else
		   to_write2 = size ; /* (offseti =< offset): first block, no data was written */

	       if (to_write > to_write2)
		   to_write = to_write2;

               /* ... but handle first data block (not all in buffer_i)... */
               if (0 != block_offseti)
               {
                     block_bufferi = buffer_data;

                     memmove(buffer_data+block_offseti,
                             buffer,
                             to_write);
		     to_write = to_write + block_offseti ;
               }

	       /* compute the new parity block */
               ret = MATH_MISC_Xor(buffer_parity,
                                   buffer_parity,
                                   block_bufferi,
                                   to_write);


               /* write the new parity block */
               //SA = SPi;
               if (SPi != 1) /* if (SPi != <servidor_falla>) */
               {
                  ret = xpni_lowfsi_pwrite(xpni_fit_get_XPN_MDATA_FD(fd), 
                                           buffer_parity, 
                                           parity_offseti, 
                                           fmeta.block_size);

#if defined(XPN2_DEBUG)
	          if (ret < fmeta.block_size)
	              DEBUG_MSG_PrintF(ERROR,"xpni_lowfsi_pwrite -> %d.\n",ret);
#endif
               }

               /* write data */
               //SA = SDi;
               if (SDi != 1) /* if (SDi != <servidor_falla>) */
               {
/* <rebuild data block> */
                  ret = MATH_MISC_Xor(buffer_parity,
                                      buffer_parity,
                                      buffer_parity,
                                      fmeta.block_size);
/* </rebuild data block> */

                  ret = xpni_lowfsi_pwrite(xpni_fit_get_XPN_DATA_FD(fd), 
                                           block_bufferi,
                                           offseti, 
                                           to_write);

#if defined(XPN2_DEBUG)
   	          if (ret < fmeta.block_size)
	              DEBUG_MSG_PrintF(ERROR,"xpni_lowfsi_pwrite -> %d.\n",ret);
#endif
               }

               /* next buffer block, and next data server */
               buffer_i = buffer_i + fmeta.block_size - block_offseti;
	       block_offseti = 0;

               SDi = (SDi + 1) % aux2 ;
            }

	    /* update the block_i: the first block of next row */
	    if (0 == row)
		block_i = block_i - (block_i % (fmeta.servers_count - 1)) ;

            block_i = block_i + (fmeta.servers_count - 1) ;
        }


        /*
         * Free memory
         */
        free(buffer_row) ;


        /*
         * Return bytes readed
         */

        return size ;
      }


   /* ................................................................... */

