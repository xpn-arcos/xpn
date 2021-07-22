gcc -g -Wall -Wextra -DLINUX -I/opt/xpn/include/ test.c -L/opt/xpn/lib/ -lxpn  -lmxml -lpthread -o test

gcc -g -Wall -Wextra -DLINUX -I/opt/xpn/include/ syscall_test.c -L/opt/xpn/lib/ -lxpn  -lmxml -lpthread -o syscall_test