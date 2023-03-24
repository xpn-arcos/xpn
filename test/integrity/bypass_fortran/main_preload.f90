program copy
implicit none

   ! From Felix Garcia Carballeira
   real, dimension(100) :: x, y
   real, dimension(100) :: p, q
   integer :: i

   ! data  
   do i=1,100
      x(i) = i * 0.1
      y(i) = sin(x(i)) * (1-cos(x(i)/3.0))
   end do

   ! output data into a file 
   open(1, file = '/tmp/expand/P1/demo.txt', status = 'new')
   do i=1,100
      write(1,*) x(i), y(i)
   end do

   close(1)

end program copy
