

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_write.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_swrite_fail_r5i
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
    	int end_index2;        /* Index of last block */
    	int row1, row2;        /* Index of first and last row */
    	int index1, index2;    /* Index of first and last block */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_swrite_fail_r5i(%d,%p,%lu,%lu); \n",
                   __FILE__,__LINE__,fd,buffer,(unsigned long)offset,(unsigned long)size);
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
	memset(buffer_row,0,fmeta.block_size*fmeta.servers_count);


        /* 
         * Read physical block positions
         */                                             

	/* Absolute index of first block */
        ret = MATH_MISC_locateInRAID5withInternalParity(block_index1,
                                                        fmeta.servers_count,
                                                        &SPi,&IPi,&SDi,&IDi);
	index1 = IDi * fmeta.servers_count + SDi ;

	/* Absolute index of last block */
        if (block_offset2 == 0)
	{
           ret = MATH_MISC_locateInRAID5withInternalParity(block_index2 - 1,
                                                           fmeta.servers_count,
                                                           &SPi,&IPi,&SDi,&IDi);
	   index2     = IDi * fmeta.servers_count + SDi ;
	   end_index2 = index2 + 1;
	}
	else
	{
           ret = MATH_MISC_locateInRAID5withInternalParity(block_index2,
                                                           fmeta.servers_count,
                                                           &SPi,&IPi,&SDi,&IDi);
	   index2     = IDi * fmeta.servers_count + SDi ;
	   end_index2 = index2 + 1;
	}

	/* Physical rows to read... */
	row1 = index1 / fmeta.servers_count ;
	row2 = index2 / fmeta.servers_count ;
	rows = row2 - row1 + 1;

        block_i = block_index1;

        for (row=0; row<rows; row++)
        {
            ret = MATH_MISC_locateInRAID5withInternalParity(block_i,
                                                            fmeta.servers_count,
                                                            &SPi,&IPi,&SDi,&IDi);
    
            begin_indexi   = IPi * fmeta.servers_count;
            parity_indexi  = IPi * fmeta.servers_count + SPi;
            data_indexi    = IDi * fmeta.servers_count + SDi;

	    /* end_indexi is not included */
	    if ((rows - 1) == row) /* última fila*/
	         end_indexi = end_index2 ;
	    else
	         end_indexi = begin_indexi + fmeta.servers_count; 


            begin_offseti  = begin_indexi  * fmeta.block_size;
            end_offseti    = end_indexi    * fmeta.block_size;
            parity_offseti = parity_indexi * fmeta.block_size;
            data_offseti   = data_indexi   * fmeta.block_size;

            /* read full row */
/* <Server 1 will fail ...> */
            ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
                                    buffer_row,
                                    begin_offseti,
                                    fmeta.block_size*fmeta.servers_count);

            /* get old parity for this row */
            buffer_parity = buffer_row + (parity_offseti - begin_offseti) ;

            /* for each data block: write new parity + write data */
            for 
            (
                 offseti  = data_offseti; 
                 offseti != end_offseti; 
                 offseti += fmeta.block_size
            )
            {
               /* skip parity block */
               if (offseti == parity_offseti)
                   continue ;

               /* get old data */
               buffer_data = buffer_row + (offseti - begin_offseti) ;

               /* compute new parity */
               ret = MATH_MISC_Xor(buffer_parity,
                                   buffer_parity,
                                   buffer_data,
                                   fmeta.block_size);

	       /* si (primera fila) Y (primer bloque de datos) Y (bloque parcial) */
               if ( (0 == row) && (0 != block_offset1) && (offseti == data_offseti) ) 
               {
                     if ( (unsigned int)(fmeta.block_size - block_offset1) < size )
			     memmove(buffer_data+block_offset1,
				     buffer,
				     fmeta.block_size-block_offset1);
		     else
			     memmove(buffer_data+block_offset1,
				     buffer,
				     size);

                     block_offseti = block_offset1 ;
                     block_bufferi = buffer_data;
               }
               else 
	       /* si (última fila) Y (último bloque de datos) Y (bloque parcial) */
	       if ( ((rows - 1) == row) && (0 != block_offset2) && 
			(end_offseti == (offseti + fmeta.block_size) ) )
	       {
                     memmove(buffer_data,
                             (char *)buffer + size - block_offset2,
                             block_offset2);

                     block_offseti = 0;
                     block_bufferi = buffer_data;
	       }
	       else
               {
                     block_offseti = 0;
                     block_bufferi = buffer_i;
               }

	       /* compute the new parity value */
               ret = MATH_MISC_Xor(buffer_parity,
                                   buffer_parity,
                                   block_bufferi,
                                   fmeta.block_size);

               /* write new parity */
               //SA = SPi;
               if (SPi != 1) /* if (SPi != <servidor_falla>) */
               {
                  ret = xpni_lowfsi_pwrite(xpni_fit_get_XPN_DATA_FD(fd), 
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
                                           fmeta.block_size);
#if defined(XPN2_DEBUG)
 	          if (ret < fmeta.block_size)
	              DEBUG_MSG_PrintF(ERROR,"xpni_lowfsi_pwrite -> %d.\n",ret);
#endif
               }

               /* next buffer block, and next data server */
               buffer_i = buffer_i + fmeta.block_size - block_offseti;

               SDi ++ ;
               if (SDi == SPi)
                   SDi ++ ;
            }

	    /* update the block_i: the first block of next row */
            if (0 == row)
            {
                block_i = block_i - (block_i % (fmeta.servers_count - 1));
            }
            block_i = block_i + fmeta.servers_count - 1;
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

