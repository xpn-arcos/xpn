

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_read.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_sread_nofail_r5i
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
        //int begin_offseti;     /* Offset inside associate begin data block */
        char *buffer_i;        /* Auxiliar pointer */
        int offset_start;      /* Offset of beginning of segment to read */
        int offset_end;        /* Ending Offset of segment to read */
        int segment_size;      /* Segment size to read */
        fmeta_t fmeta;         /* File metadata (extended version) */
        int rows;              /* Rows between block_index1 and block_index2 */
        int row;               /* Auxiliar counter for rows */
        int block_i;           /* Auxiliar counter for blocks */
        int end_index2;        /* Index of last block */
	int row1, row2;        /* Index of first and last row */
	int index1, index2;    /* Index of first and last block */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_sread_r5i(%d,%p,%d,%lu); \n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,(unsigned long)size);
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
         * Read physical block positions
         */                                             

	/* Absolute index of last block */
        ret = MATH_MISC_locateInRAID5withInternalParity(block_index1,
                                                        fmeta.servers_count,
                                                        &SPi,&IPi,&SDi,&IDi);
	index1 = IDi * fmeta.servers_count + SDi ;

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
	   end_index2 = index2 ;
	}

	/* Physical rows to read... */
	row1 = index1 / fmeta.servers_count ;
	row2 = index2 / fmeta.servers_count ;
	rows = row2 - row1 + 1;

        buffer_i = (char *)buffer;

        block_i = block_index1;

        for (row=0; row<rows; row++)
        {
            ret = MATH_MISC_locateInRAID5withInternalParity(block_i,
                                                            fmeta.servers_count,
                                                            &SPi,&IPi,&SDi,&IDi);
    
            begin_indexi   = (IPi)         * fmeta.servers_count;
            parity_indexi  = (IPi)         * fmeta.servers_count + SPi;
            data_indexi    = (IDi)         * fmeta.servers_count + SDi;

            if ((rows-1) != row)
                 end_indexi = (IPi + 1)    * fmeta.servers_count;
            else
                 end_indexi = end_index2 ;

            //begin_offseti  = begin_indexi  * fmeta.block_size;
            end_offseti    = end_indexi    * fmeta.block_size;
            parity_offseti = parity_indexi * fmeta.block_size;
            data_offseti   = data_indexi   * fmeta.block_size;

            if (0 == row) 
                data_offseti = data_offseti + block_offset1;

            if ((rows-1) == row)
                end_offseti  = end_offseti + block_offset2;

            if (
			(parity_offseti > data_offseti) 
			               &&
			(parity_offseti < end_offseti)
	       )
            {
                  offset_start = data_offseti ;
                  offset_end   = parity_offseti ;
                  segment_size = offset_end - offset_start ;
        	  ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
				          buffer_i,
				          offset_start,
				          segment_size);
                  buffer_i = buffer_i + segment_size ;
        
                  offset_start = parity_offseti + fmeta.block_size;
                  offset_end   = end_offseti ;
                  segment_size = offset_end - offset_start ;
        	  ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
				          buffer_i,
				          offset_start,
				          segment_size);
                  buffer_i = buffer_i + segment_size;
                }
                else
                {
                  offset_start = data_offseti ;
                  offset_end   = end_offseti ;
                  segment_size = offset_end - offset_start ;
        	  ret = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
				          buffer_i,
				          offset_start,
				          segment_size);
                  buffer_i = (char *)buffer + segment_size;
            }


            if (0 == row)
                block_i = block_i - (block_i % (fmeta.servers_count - 1));
            block_i = block_i + fmeta.servers_count - 1;

	    /*
            if (0 == row) 
                 block_i = (block_i - SDi) + fmeta.servers_count;
            else
                 block_i = (block_i)       + fmeta.servers_count;
	    */
        }


        /*
         * Return bytes readed
         */

        return size ;
      }


   /* ................................................................... */

