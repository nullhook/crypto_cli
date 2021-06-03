
CC     = cc
CFLAGS = -O2 -Wall
INC    = -framework IOKit
PREFIX = /usr/local
EXEC   = crypto
SOURCES := $(shell find . -name '*.c')

build : $(EXEC)

clean :
	rm $(EXEC)

crypto: crypto.cc
	g++ -std=c++17 -lcurl -Wall -o $@ $?

install : $(EXEC)
	@install -v $(EXEC) $(PREFIX)/bin/$(EXEC)