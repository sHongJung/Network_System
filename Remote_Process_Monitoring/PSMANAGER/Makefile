C          = gcc
CFLAGS      = -W -Wall -g
LDFLAGS     = -lm -lpthread

psmanager: psmanager_main.c psmnt.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	@rm -rf *.o psmanager

