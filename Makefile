CC = g++
CFLAGS = -std=c++20 -O2 -Wall -Wextra -MMD -MP

FIND = find
ECHO = echo 
EXEC = ./

TARGET = test
OBJDIR = objs
OUTDIR = bin
BINEXT = .out

SRCS += $(shell  $(FIND) ./src/ -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCLUDE = -I./include
DEPS = $(OBJS:.o=.d)

.PHONY: all
all: $(TARGET)
	$(EXEC)$(OUTDIR)/$(TARGET)$(BINEXT)

.PHONY: debug
debug : CFLAGS += -D DEBUG
debug : all

$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	@$(CC) $(CFLAGS) -o $(OUTDIR)/$@$(BINEXT) $^ $(LIBS)
	@$(ECHO) link :::: $@

.PHONY: clean
clean:
	$(FIND) -type f \( -name "*.o" -o -name "*~" -o -name "*#*" \) -delete
	$(FIND) -type f \( -name "*.o" -o -name "*~" -o -name "*#*" \) -delete
	rm -rf objs

#.cpp.o:
$(OBJDIR)/%.o:%.cpp
	@$(ECHO) compiling :::: $<
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<
	

-include $(DEPS)