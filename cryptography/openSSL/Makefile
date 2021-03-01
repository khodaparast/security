CC = g++
CFLAGS = -ggdb -Wall -Werror -DDEBUG


IDIR = ./openssl
SDIR = ./src

VPATH = SDIR IDIR

LIBS = -lcrypto -lpthread -lssl  -lz -ldl

EDO: test_ssl_enc.o
	 $(CC)  test_ssl_enc.o -L/lib $(LIBS) -I$(IDIR) -o EDO


test_ssl_enc.o: $(SDIR)/test_ssl_enc.cc
	$(CC)  -c $(SDIR)/test_ssl_enc.cc -I$(IDIR)



clean:
	rm EDO *.o
