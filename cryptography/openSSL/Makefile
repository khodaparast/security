CC = g++
CFLAGS = -ggdb -Wall -Werror -DDEBUG


IDIR = /home/casauser/Desktop/Project-git/c++/openssl
SDIR = /home/casauser/Desktop/Project-git/c++/enc_dec_openssl/src

VPATH = SDIR IDIR

LIBS = -lcrypto -lpthread -lssl  -lz -ldl

EDO: test_ssl_enc.o
	 $(CC)  test_ssl_enc.o -L/home/casauser/Desktop/Project-git/c++/enc_dec_openssl/lib $(LIBS) -I$(IDIR) -o EDO


main.o: $(SDIR)/test_ssl_enc.cc
	$(CC)  -c $(SDIR)/test_ssl_enc.cc -I$(IDIR)



clean:
	rm EDO *.o
