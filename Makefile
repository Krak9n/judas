IDIR1=./include/stb/ 
IDIR2=./include/glad/
IDIR3=~/cglm/include
CC=gcc
CFLAGS=-I$(IDIR1) -I$(IDIR2) -I.$(IDIR3)

ODIR=src
# for sdl3
LIBS=-lSDL3 

# for opengl
LIBS2=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
_DEPS = stb_image.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = master.c glad.c ui/bip.c
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIBS2)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
