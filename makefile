VULKAN_SDK_PATH := $VULKAN_SDK
H_FILES := includes
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS :=
LD_LIBS := -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan -lGLEW -lGL
#LD_LIBS := -ldl -lXinerama -lXrandr -lXi -lXcursor -lXxf86vm -lpthread -lGL  -lGLEW -lrt -lglfw
CC_FLAGS := -Wall -g -std=gnu++17 -I$(H_FILES) -I/usr/include -I$(VULKAN_SDK_PATH)/include

donkeyKong: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^ $(LD_LIBS)

obj/%.o: src/%.cpp
	g++ -c $(CC_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm obj/*

test: donkeyKong
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d ./donkeyKong
