IDIR = src
CC  = g++
CFLAGS = -I$(IDIR) -Werror -Wall -Wextra -pedantic

SDIR = src
ODIR = build
LDIR = lib

LIBS=

_DEPS = generator.h word.h wordprovider.h csvwordprovider.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = generator.o csvwordprovider.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

generator: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 
