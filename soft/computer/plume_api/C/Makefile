OBJDIR = build
INCLUDE_DIR = ../../calculator/
INCLUDE_CC = $(foreach d, $(INCLUDE_DIR), -I$d)
INCLUDE_LD = $(foreach d, $(INCLUDE_DIR), -L$d)

CPP_FILES = test_plume_api.cpp
C_FILES = 

OBJ_FILES = $(addprefix $(OBJDIR)/,$(CPP_FILES:.cpp=.o))
OBJ_FILES += $(addprefix $(OBJDIR)/,$(C_FILES:.c=.o))
OBJ_FILES += ../../calculator/build/plume_api.so
CC = gcc
LDLFLAGS = -lgsl -lcblas
CFLAGS = -g -O2 -std=gnu99 -Wall -Wextra -Werror $(INCLUDE_CC)
CXXFLAGS = -g -O2 -std=c++11 -Wall -Wextra -Werror $(INCLUDE_CC) -Wno-write-strings

all: $(OBJDIR)/test_plume_api

$(OBJDIR)/test_plume_api: $(OBJ_FILES) 
	g++ $^ $(LDLFLAGS) -o $@

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: ../calculator_interface_nodejs/%.cpp | $(OBJDIR)
	g++ $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc $(CFLAGS) -o $@ -c $<

clean: 
	rm -fR $(OBJDIR)