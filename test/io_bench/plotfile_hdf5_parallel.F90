!----------------------------------------------------------------------------
! subroutine plotfile
!----------------------------------------------------------------------------

      subroutine plotfile_hdf5_par(filenum, simtime, corners)
!
! plotfile using parallel i/o using HDF5
!
! MZ -- 4-29-00
!
! This version of the plotfile routine is based on the parallel HDF5 
! checkpoint.  The IO is done in parallel -- no copying of the data to 
! a single processor to do the writing is performed.  
!
! This is the SINGLE PRECISION version of the plotfile -- temporary
! storage is used to recast a variable (for every zone/block) into
! single precision before passing it onto the SP version of the C HDF 5
! write routines.
! 
! The data for all blocks is recast and written together.  This makes the
! amount of data that is written very large, which should perform better
! on the parallel filesystems.  The overhead for storing an entire 
! variable (with corners) is small, <~ 1%.
!
! HDF5 uses MPI-IO (via ROMIO) to support parallel IO.  Each processor
! must open the file, create the datasets, and dataspaces for each HDF
! record.  
!
! A single record for each of the PARAMESH data structures is created.  A
! processor only writes to a subset of this record.  Each record has a 
! dimension with length = tot_blocks.  The offset of a processor into this 
! dimension is computed by looking at the total number of blocks that are
! below the current processor.
!
! In this version of the plotfile, each variable is given its own 
! record -- this makes it easier to change the variable list in the
! future without disturbing the format of the file.
!
! The include file -- hdf5_flash.h is used for the C routines and mirrors
! the necessary data from physicaldata.fh
!
! written for HDF5 1.2.1
!
!---------------------------------------------------------------------------

#include "common.fh"

      integer filenum
      real simtime
      
      integer block_no
      integer i, j, k, ivar, i_store, j_store, k_store
      integer lnblockst 
      integer ngid 
      integer ierr

      integer n_to_left(0:16383)  ! must extend from 0 to NumPEs-1

! 2-20-00 -- we don't need to allocate more space than necessary
!      integer gid(mfaces+1+mchild,maxblocks_tr)
      integer gid(nfaces+1+nchild,maxblocks_tr)

      integer tot_blocks
      
      save gid, n_to_left

      integer nzones_block(3)

! create a character variable to hold the string representation of the block
! number.  Note this is set to be 4 characters long (i.e. max = 9999).  
      character*4  fnum_string
      character*80 filename, outstr

! set the number of variables we are going to write out
      integer, parameter ::  num_out = 4

! create a temporary array to hold the 4 character variable names
! this will include those defined in definitions.fh and network_common.fh
      character*4 unklabels(nvar), sunklabels(num_out)

! storage for the current date and time
      character date_string*40

      character(len=4) :: ionam(ionmax), record_label
      
! storage for the global block number we are writing out
      integer iblock_out

      integer file_id

      integer global_offset


! hold pointers to the location in unk of the variables we are writing out
      integer iout(num_out)

! allocate storage to hold a single variable information
! this should only be a small memory overhead
      integer, parameter :: single = SELECTED_REAL_KIND(p=6)
      real (kind=single) :: unkt_crn(1,nxb+1,nyb+k2d,nzb+k3d,maxblocks)
      real (kind=single) :: unkt(1,nxb,nyb,nzb,maxblocks)

! allocate storage to hold the coordinate information and bounding box
! information
      real (kind=single) :: xyz_single(mdim,maxblocks_tr)
      real (kind=single) :: bnd_single(2,mdim,maxblocks_tr)
      real (kind=single) :: sp_var1, sp_var2

      integer, parameter :: release_len = 20
      character (len=release_len) :: flash_release

      logical corners

!-----------------------------------------------------------------------------
! set the variables we are going to store
!-----------------------------------------------------------------------------
      iout(1) = idens
      iout(2) = itemp
      iout(3) = ipres

! store the first abundance
      iout(4) = inuc_begin


!-----------------------------------------------------------------------------
! compute the total number of blocks left of a given processor number
!-----------------------------------------------------------------------------

! use an allgather routine here to get the number of blocks on each proc.
      call MPI_Allgather(lnblocks, 1,MPI_INTEGER, & 
     &                   n_to_left,1,MPI_INTEGER, & 
     &                   MPI_COMM_WORLD,ierr)
      
! compute the total number of blocks
      tot_blocks = 0

      do i = 0,NumPEs-1
         tot_blocks = tot_blocks + n_to_left(i)
      end do
      
! compute the number of procssors to the left of a processor
      do i = NumPEs-1,1,-1
         n_to_left(i) = n_to_left(i-1)
      end do

      n_to_left(0) = 0
      do i = 2,NumPEs-1
         n_to_left(i) = n_to_left(i) + n_to_left(i-1)
      end do


!-----------------------------------------------------------------------------
! compute the global id -- this is a single array which stores the 
! neighbor block numbers, the parent, and the children of a given block
!-----------------------------------------------------------------------------
      do block_no = 1,lnblocks

         ngid = 0

! loop over the faces and store the neighbors
         do j = 1,nfaces
            ngid = ngid + 1

! if the neighbor exists, then store the block number of the neighbor
! -- take into account the number of blocks below the processor that the
! neighbor is on, so the block number is global
            if (neigh(1,j,block_no).gt.0) then
               gid(ngid,block_no) = neigh(1,j,block_no) +  & 
     &              n_to_left(neigh(2,j,block_no))
            else

! the neighbor is either a physical boundary or does not exist at that 
! level of refinement
               gid(ngid,block_no) = neigh(1,j,block_no)
            end if
         end do
        
! store the parent of the current block
         ngid = ngid + 1
         if (parent(1,block_no).gt.0) then
            gid(ngid,block_no) = parent(1,block_no) +  & 
     &           n_to_left(parent(2,block_no))
         else
            gid(ngid,block_no) = parent(1,block_no)
         end if
         
! store the children of the current block
         do j = 1,nchild
            ngid = ngid + 1
            if (child(1,j,block_no).gt.0) then
               gid(ngid,block_no) = child(1,j,block_no) +  & 
     &              n_to_left(child(2,j,block_no))
            else
               gid(ngid,block_no) = child(1,j,block_no)
            end if
         end do

      end do


!-----------------------------------------------------------------------------
! open the hdf file
!-----------------------------------------------------------------------------
      write (fnum_string, '(i4.4)') filenum

      if (corners) then
         filename = trim(basenm) // 'hdf5_plt_crn_'// fnum_string
      else
         filename = trim(basenm) // 'hdf5_plt_cnt_'// fnum_string
      endif

      call h5_initialize_file(file_id, filename)


!-----------------------------------------------------------------------------
! store the scalar information -- # of blocks, simulation time, etc
!-----------------------------------------------------------------------------
      
! get the current time and date
      date_string = "now"

! store the number of zones / block in each direction
      if (corners) then
         nzones_block(1) = nxb+1
         nzones_block(2) = nyb+k2d
         nzones_block(3) = nzb+k3d
      else
         nzones_block(1) = nxb
         nzones_block(2) = nyb
         nzones_block(3) = nzb
      endif

! get the names of the fluids being followed
      call get_mfluid_property ("short name", ionam)

! merge the two variable lists into one for storage
      unklabels(1:nvar-ionmax)     = varnam(:)
      unklabels(nvar-ionmax+1:nvar) = ionam(:)

! get the subset of the variable labels, corresponding to what we are storing
      do i = 1, num_out
         sunklabels(i) = unklabels(iout(i))
      enddo

      sp_var1 = real(simtime, kind = single)
      sp_var2 = real(dt, kind = single)

      call h5_write_header_info_sp(MyPE, & 
     &                             num_out, & 
     &                             file_id, & 
     &                             date_string, & 
     &                             flash_release(), & 
     &                             run_comment, & 
     &                             tot_blocks, & 
     &                             sp_var1, & 
     &                             sp_var2, & 
     &                             nstep, & 
     &                             nzones_block, & 
     &                             sunklabels)

      global_offset = n_to_left(MyPE)

!-----------------------------------------------------------------------------
! store the tree information
!-----------------------------------------------------------------------------

! store the refinement level
      call h5_write_lrefine(file_id,  & 
     &                      lrefine,  & 
     &                      lnblocks,  & 
     &                      tot_blocks,  & 
     &                      global_offset)

! store the nodetype
      call h5_write_nodetype(file_id,  & 
     &                       nodetype,  & 
     &                       lnblocks,  & 
     &                       tot_blocks,  & 
     &                       global_offset)

! store the global id
      call h5_write_gid(file_id,  & 
     &                  gid,  & 
     &                  lnblocks,  & 
     &                  tot_blocks,  & 
     &                  global_offset)


!-----------------------------------------------------------------------------
! store the grid information
!-----------------------------------------------------------------------------
      
! store the coordinates

      do block_no = 1, lnblocks
         xyz_single(:,block_no) = real(coord(:,block_no), kind = single)
      enddo

      call h5_write_coord_sp(file_id,  & 
     &                       maxblocks_tr, & 
     &                       xyz_single,  & 
     &                       lnblocks,  & 
     &                       tot_blocks,  & 
     &                       global_offset)


      do block_no = 1, lnblocks
         xyz_single(:,block_no) = real(size(:,block_no), kind = single)
      enddo

! store the block size
      call h5_write_size_sp(file_id,  & 
     &                      maxblocks_tr, & 
     &                      xyz_single,  & 
     &                      lnblocks,  & 
     &                      tot_blocks,  & 
     &                      global_offset)

! store the bounding box

      do block_no = 1, lnblocks
         bnd_single(:,:,block_no) =  & 
     &        real(bnd_box(:,:,block_no), kind = single)
      enddo

      call h5_write_bnd_box_sp(file_id, & 
     &                         maxblocks_tr, & 
     &                         bnd_single, & 
     &                         lnblocks,  & 
     &                         tot_blocks,  & 
     &                         global_offset)


!-----------------------------------------------------------------------------
! store the unknowns -- here we will pass the entire unk array on each 
! processor.  The HDF 5 memory space functionality will pick just the 
! interior cells to write to disk.
!-----------------------------------------------------------------------------

      do ivar = 1, num_out
         record_label = unklabels(iout(ivar))

! put the data at the corners if necessary
         if (corners) then

! interpolate only the variable we are storing to the corners            

! ** Important, the limits of the unkt_crn array do not include 
!    guard cells, so we need to map the interior of the unk array
!    into the unkt_crn array.
            do block_no = 1, lnblocks
            
               do k = nguard*k3d+1,nguard*k3d+nzb+k3d
                  k_store = k - nguard*k3d

                  do j = nguard*k2d+1,nguard*k2d+nyb+k2d
                     j_store = j - nguard*k2d
                     
                     do i = nguard+1,nguard+nxb+1
                        i_store = i - nguard

#if N_DIM == 2
                        unkt_crn(1,i_store,j_store,k_store,block_no) =  & 
     &                       real( & 
     &                      .25*(unk(iout(ivar),i-1,j,  k,block_no) + & 
     &                           unk(iout(ivar),i  ,j,  k,block_no) + & 
     &                           unk(iout(ivar),i  ,j-1,k,block_no) + & 
     &                           unk(iout(ivar),i-1,j-1,k,block_no)), & 
     &                       kind = single)
#endif
#if N_DIM == 3
                        unkt_crn(1,i_store,j_store,k_store,block_no) =  & 
     &                       real( & 
     &                      .125*(unk(iout(ivar),i-1,j  ,k  ,block_no) + & 
     &                            unk(iout(ivar),i  ,j  ,k  ,block_no) + & 
     &                            unk(iout(ivar),i  ,j-1,k  ,block_no) + & 
     &                            unk(iout(ivar),i-1,j-1,k  ,block_no) + & 
     &                            unk(iout(ivar),i-1,j  ,k-1,block_no) + & 
     &                            unk(iout(ivar),i  ,j  ,k-1,block_no) + & 
     &                            unk(iout(ivar),i  ,j-1,k-1,block_no) + & 
     &                            unk(iout(ivar),i-1,j-1,k-1,block_no)), & 
     &                       kind = single)
#endif
                        
                     end do
                  end do
               end do
               
            enddo


! we now have the data at the corners, in a 4-byte real array

            call h5_write_unknowns_sp(file_id, & 
     &                                1,           &  ! unkt has only 1 var
     &                                1,     & 
     &                                nxb+1,   & 
     &                                nyb+k2d,   & 
     &                                nzb+k3d, & 
     &                                0,           &  ! no guardcells in unkt
     &                                maxblocks,    & 
     &                                unkt_crn,          & 
     &                                record_label,  & 
     &                                lnblocks,  & 
     &                                tot_blocks,  & 
     &                                global_offset)


         else

            unkt(1,:,:,:,:) = real(unk(iout(ivar), & 
     &                                 nguard+1:nguard+nxb, & 
     &                                 nguard*k2d+1:nguard*k2d+nyb, & 
     &                                 nguard*k3d+1:nguard*k3d+nzb,:), & 
     &                             kind = single)
            
            

            call h5_write_unknowns_sp(file_id, & 
     &                                1,           &  ! unkt has only 1 var
     &                                1,     & 
     &                                nxb,   & 
     &                                nyb,   & 
     &                                nzb, & 
     &                                0,           &  ! no guardcells in unkt
     &                                maxblocks,    & 
     &                                unkt,          & 
     &                                record_label,  & 
     &                                lnblocks,  & 
     &                                tot_blocks,  & 
     &                                global_offset)

         endif

      enddo

!-----------------------------------------------------------------------------
! close the file
!-----------------------------------------------------------------------------

      call h5_close_file(file_id)

      if (MyPE .EQ. MasterPE) then
         print *, '*** Wrote output to ', trim(filename),  & 
     &        ' (', tot_blocks, 'blocks ) ***'

      end if

      return
      end







