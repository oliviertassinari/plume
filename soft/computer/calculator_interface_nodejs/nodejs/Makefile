OBJDIR = build
INCLUDE_DIR = 	../../calculator/ \
				../
INCLUDE_CC = $(foreach d, $(INCLUDE_DIR), -I$d)
INCLUDE_LD = $(foreach d, $(INCLUDE_DIR), -L$d)

CPP_FILES = test_nodejs.cpp \
			trajectory_simulation.cpp
C_FILES = 	dof.c \
			matrix_utils.c \
			magnetic.c \
			utils.c

OBJ_FILES = $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES += $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))
CC = g++
LDLIBS = -L/usr/local/lib/ -lrabbitmq -lpthread -lgsl -lcblas $(INCLUDE_LD)
CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror $(INCLUDE_CC)
CXXFLAGS = -g -O2 -std=c++11 -Wall -Wextra -Werror $(INCLUDE_CC)

all: $(OBJDIR)/test

$(OBJDIR)/test: $(OBJ_FILES)
	g++ $^ $(LDLIBS) -o $@

debug: CFLAGS+= -DDEBUG
debug: CXXFLAGS+= -DDEBUG
debug: all

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: ../../calculator/%.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: ../../calculator/%.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: ../%.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

clean: 
	rm -fR $(OBJDIR)
