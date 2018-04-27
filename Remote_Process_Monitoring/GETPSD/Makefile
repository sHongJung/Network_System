C          = gcc
CFLAGS      = -W -Wall -O2
LDFLAGS     = -lm

getpsd : prcli_main.c prcfile_gen.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	@rm -rf *.o getpsd

