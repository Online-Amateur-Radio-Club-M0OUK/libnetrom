CC      = cc
CXX     = c++
CFLAGS  = -g -O3 -Wall -std=c++17 -fPIC
LIBS    = 
LDFLAGS = -g -shared -fPIC

OBJECTS = Client.o Server.o Socket.o

all:		libnetrom.so

libnetrom.so:	$(OBJECTS)
		$(CXX) $(OBJECTS) $(LDFLAGS) $(LIBS) -o libnetrom.so

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<

install:	all
		install -m 644 libnetrom.so /usr/local/lib/

clean:
		$(RM) libnetrom.so *.o *.bak *~

