CC=g++
IDIR=include
ODIR=obj
SDIR=source
CFLAGS=-std=c++0x -I $(IDIR) -Wall -Wextra -pedantic
LIBS=-L /usr/local/lib
LSFML=-lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window
APP=Bargraph

all: $(APP)

$(APP): $(ODIR)/Bar.o $(ODIR)/Bars.o $(ODIR)/Main.o
	$(CC) -o $(APP) $(ODIR)/*.o $(LIBS) $(LSFML)

$(ODIR)/Main.o: $(SDIR)/Main.cpp $(IDIR)/Bars.hpp
	$(CC) $(CFLAGS) -c $(SDIR)/Main.cpp
	mv Main.o $(ODIR)

$(ODIR)/Bars.o: $(SDIR)/Bars.cpp $(SDIR)/Bar.cpp
	$(CC) $(CFLAGS) -c $(SDIR)/Bars.cpp
	mv Bars.o $(ODIR)

$(ODIR)/Bar.o: $(SDIR)/Bar.cpp
	$(CC) $(CFLAGS) -c $(SDIR)/Bar.cpp
	mv Bar.o $(ODIR)

clean:
	rm $(ODIR)/*.o $(APP)

