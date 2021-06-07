HDIR=/headers/
IDIR = /openssl/ 
SDIR = src
lib = /lib/




CXX=g++
CPPFLAGS =  -ggdb -Wall -Werror -DDEBUG 




VPATH = SDIR IDIR HDIR

LIBS = -lcrypto -lpthread -lssl  -lz -ldl


AES_Enc: ssl_enc.o
	 $(CXX)  ssl_enc.o -L./AES/$(lib) $(LIBS) -I$(IDIR) -o AES_Enc

ssl_enc.o: ./AES/$(SDIR)/ssl_enc.cc 
	$(CXX)  -c ./AES/$(SDIR)/ssl_enc.cc -I$(IDIR)






AES_File_Enc: ssl_enc_file.o 
	 $(CXX) -g  ssl_enc_file.o -L./AES_File/$(lib) $(LIBS) -I$(IDIR)  -I$(HDIR) -o AES_File_Enc


ssl_enc_file.o: ./AES_File/$(SDIR)/ssl_enc_file.cc 
	$(CXX)  -c ./AES_File/$(SDIR)/ssl_enc_file.cc -I$(lib) -I$(HDIR) -I$(IDIR) 





clean:
	rm  *Enc *.o


# g++ -g ssl_enc_file.cc -I../lib/ -I../headers/ -I ../../openssl/   -lcrypto -lpthread -lssl  -lz -ldl -o enc-file 