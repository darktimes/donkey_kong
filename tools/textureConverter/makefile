H_FILES := includes
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS :=
LD_LIBS := -lglfw -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lGLEW -lrt -lXcursor -lSOIL
CC_FLAGS := -Wall -std=gnu++17 -I$(H_FILES)

textureConverter: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^ $(LD_LIBS)

obj/%.o: src/%.cpp
	g++ -c $(CC_FLAGS) -o $@ $<
