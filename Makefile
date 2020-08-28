#EXECUTABLE NAME
TARGET = game.exe
#MKDIR = cmd /E:ON /C md
MKDIR = mkdir
#DEL = del /Q
DEL = rm -rf
##COMPILER
CC = g++
LD = g++

##FLAGS
CFLAGS = -static-libstdc++ -static-libgcc -static -march=pentium-mmx -Wall -m32 -std=c++1z -fexceptions -fpermissive -fomit-frame-pointer
#CFLAGS += -O3
CFLAGS += -g

#GLIDE_DEFINES
DEFINES = -D__WIN32__ -DFX_DLL_ENABLE -DHWC_ACCESS_DDRAW=1 -DHWC_EXT_INIT=1 -DGLIDE_ALT_TAB=1 -DBETA=1 -DHWC_MINIVDD_HACK=1 -DWIN40COMPAT=1 -DWINXP_ALT_TAB_FIX=1 -DWINXP_SAFER_ALT_TAB_FIX=1 -DNEED_MSGFILE_ASSIGN -UWINNT -DGLIDE3 -DGLIDE3_ALPHA -DGLIDE_HW_TRI_SETUP=1 -DGLIDE_INIT_HWC -DGLIDE_PACKED_RGB=0 -DGLIDE_PACKET3_TRI_SETUP=1 -DGLIDE_TRI_CULLING=1 -DUSE_PACKET_FIFO=1 -DGLIDE_CHECK_CONTEXT -DH5 -DFX_GLIDE_H5_CSIM=1 -DFX_GLIDE_NAPALM=1 -DGLIDE_PLUG -DGLIDE_SPLASH -D__MINGW32__
#DEFINES += -DNDEBUG
DEFINES += -DDEBUG

#BUILD FOLDERS
MODULES = engine game
SRC_DIR = $(addprefix src/,$(MODULES))
BUILD_DIR = $(addprefix build/,$(MODULES))

SRC = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

#INCLUDES
GLIDE_INCLUDE = -Iglide\glide3x\h5\glide3\src -Iglide\swlibs\fxmisc -Iglide\glide3x\h5\incsrc -Iglide\glide3x\h5\glide3\tests
SDL_INCLUDE = -I3rd\include
INCLUDES = -Iinclude $(addprefix -I,$(SRC_DIR)) $(GLIDE_INCLUDE) $(SDL_INCLUDE)

#LIBS
CLIBS = -static-libstdc++ -static-libgcc -static
CLIBS += -m32 -lmingw32 3rd\lib\libSDLmain.a 3rd\lib\libSDL.dll.a -luser32 -lgdi32 -lwinmm -limm32 -lwinspool -lshell32 -lole32 -loleaut32 -lversion -luuid -lcomdlg32 -ladvapi32 -lkernel32

vpath %.cpp $(SRC_DIR)

define make-release
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean callDebug

all: checkdirs build/$(TARGET)

build/$(TARGET): $(OBJ)
	$(LD) $^ -o $@ $(CLIBS)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@$(MKDIR) $(subst /,\\,$@)

clean:
	@$(DEL) $(subst /,\\,$(BUILD_DIR))
	@$(DEL) build\$(TARGET)

Release: all

Debug: all

cleanDebug: clean
cleanRelease: clean

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-release,$(bdir))))


