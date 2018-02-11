OBJDIR = build
CPP_FILES =	test_calculator_interface_nodejs.cpp \
			calculator_interface_nodejs.cpp 
C_FILES = 	utils.c

OBJ_FILES = $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES += $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))
CC = g++
LDLIBS = -L/usr/local/lib -lrabbitmq -pthread -Wl,--no-as-needed
CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror -Wl,--no-as-needed
CXXFLAGS = -g -O2 -std=c++11 -Wall -Wextra -Werror -Wl,--no-as-needed -Wno-write-strings

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
