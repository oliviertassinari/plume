OBJDIR = build
CPP_FILES = 
C_FILES = 	emitter_interface.c \
			test.c
			
OBJ_FILES = $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES += $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))
CC = g++
LDLIBS = 
CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror
CXXFLAGS = -g -O2 -std=c++0x -Wall -Wextra -Werror

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

clean: 
	rm -fR $(OBJDIR)