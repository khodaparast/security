CC = g++
CFLAGS = -ggdb -Wall -Werror -DDEBUG



IDIR = /openssl 
SDIR = src
lib = lib

VPATH = SDIR IDIR

LIBS = -lcrypto -lpthread -lssl  -lz -ldl


ssl_enc.o: ./AES/$(SDIR)/ssl_enc.cc
	$(CC)  -c ./AES/$(SDIR)/ssl_enc.cc -I$(IDIR)


AES_Enc: ssl_enc.o
	 $(CC)  ssl_enc.o -L./AES/$(lib) $(LIBS) -I$(IDIR) -o AES_Enc





ssl_enc_file.o: ./AES_File/$(SDIR)/ssl_enc_file.cc
	$(CC)  -c ./AES_File/$(SDIR)/ssl_enc_file.cc -I$(IDIR) 


AES_File_Enc: ssl_enc_file.o
	 $(CC)  ssl_enc_file.o -L./AES_File/$(lib) $(LIBS) -I$(IDIR) -o AES_File_Enc



clean:
	rm  *Enc *.o
