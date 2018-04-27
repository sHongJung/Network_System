C          = gcc
CFLAGS      = -W -Wall -g
LDFLAGS     = -lm

serverd: prcserver_main.c prc_info.c select_sock.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	@rm -rf *.o serverd

