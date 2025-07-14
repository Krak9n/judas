IDIR=./src/ -I./include/
CC=g++
CFLAGS=-std=c++17 -O2 -I$(IDIR)

ODIR=src

# for opengl
LIBS=-lstdc++ -lglfw -lGL -lvulkan -lXxf86vm -lX11 -lpthread -lXrandr -lXi -ldl -lm
_DEPS = deps.h utils.h window/*.hpp ui/nuklear/nuklear.h stb/stb_image.h shaders/shader2.h glm/glm.hpp glmm/gtc/matrix_transform.hpp glm/gtc/type_ptr.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.cpp glad.c ui/main.c
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -g $(LIBS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
