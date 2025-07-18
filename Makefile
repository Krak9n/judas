IDIR=-I./src/ -I./include/
CC=g++
CFLAGS=-std=c++17 -O2 $(IDIR)

ODIR=src

# for vulkan and ogl
# -lvulkan -lXxf86vm
LIBS=-lstdc++ -lglfw -lGL -lvulkan -lXxf86vm -lX11 -lpthread -lXrandr -lXi -ldl -lm
# shaders/vulkan_stuff/*.hpp
_DEPS = deps.h utils.h window/*.hpp shaders/vulkan_stuff/*.hpp stb/stb_image.h shaders/opengl_stuff/shader2.h glm/glm.hpp glmm/gtc/matrix_transform.hpp glm/gtc/type_ptr.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.cpp glad.c ui/load.cpp shaders/vulkan_stuff/include_shader.cpp 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -g $(LIBS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
