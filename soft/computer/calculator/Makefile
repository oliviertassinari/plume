OBJDIR = build
INCLUDE_DIR = 	../emitter_interface/ \
				../calculator_interface_nodejs/
INCLUDE_CC = $(foreach d, $(INCLUDE_DIR), -I$d)
INCLUDE_LD = $(foreach d, $(INCLUDE_DIR), -L$d)

CPP_FILES = plume_api.cpp \
			plume_receiver.cpp \
			plume_emitter.cpp \
			calibration_db.cpp \
			calculator_interface_nodejs.cpp \

C_FILES = 	dof.c \
			magnetic.c \
			matrix_utils.c \
			debug_utils.c \
			emitter_interface.c \
			utils.c

OBJ_FILES = $(addprefix $(OBJDIR)/,$(CPP_FILES:.cpp=.o))
OBJ_FILES += $(addprefix $(OBJDIR)/,$(C_FILES:.c=.o))
CC = gcc
LDLFLAGS = -lm -lgsl -lcblas -lstdc++ -lpthread $(INCLUDE_LD) -lrabbitmq -ljsoncpp
CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror $(INCLUDE_CC)
CXXFLAGS = -g -O2 -std=c++11 -Wall -Wextra -Werror $(INCLUDE_CC) -Wno-write-strings

all: shared

shared: CFLAGS += -fPIC
shared: CXXFLAGS += -fPIC
shared: LDLFLAGS += -shared
shared: $(OBJDIR)/plume_api.so

debug: CFLAGS += -DDEBUG
debug: CXXFLAGS += -DDEBUG
debug: all

$(OBJDIR)/plume_api.so: $(OBJ_FILES)
	g++ $^ $(LDLFLAGS) -o $@

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: ../calculator_interface_nodejs/%.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: ../emitter_interface/%.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: ../calculator_interface_nodejs/%.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

clean: 
	rm -fR $(OBJDIR)
