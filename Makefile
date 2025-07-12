IDIR1=./include/stb/ 
IDIR2=./include/glad/
IDIR4=./src/shaders/
IDIR5=./include/glm/
IDIR6=./include/glm/gtc/
CC=gcc
CFLAGS=-I$(IDIR1) -I$(IDIR5) -I$(IDIR2) -I.$(IDIR3) -I$(IDIR4) 

ODIR=src
# for sdl3
LIBS=-lSDL3 

# for opengl
LIBS2=-lstdc++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
_DEPS = stb_image.h 
_DEPS2 = shader2.h
_DEPS3 = glm.hpp 
_DEPS4 = matrix_transform.hpp type_ptr.hpp
DEPS = $(patsubst %,$(IDIR1)/%,$(_DEPS),$(IDIR4)/%,$(_DEPS2), $(IDIR5)/%,$(_DEPS3),$(IDIR6)/%,$(_DEPS4))

_OBJ = master.cpp glad.c ui/main.c
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -g $(LIBS) $(LIBS2)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
