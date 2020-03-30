# CC=gcc
# CFLAGS=-I.
# DEPS = hellomake.h
#
# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)
#
# diseaseMonitor: diseaseMonitor.o functions.o
# 	$(CC) -o diseaseMonitor diseaseMonitor.o functions.o

#
# CC := gcc # This is the main compiler
# SRCDIR := src
# BUILDDIR := build
# TARGET := bin/diseaseMonitor
#
# SRCEXT := c
# SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
# OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
# CFLAGS :=
# INC := -I include
#
# $(TARGET): $(OBJECTS)
# 	@echo " Linking..."
# 	@echo " $(CC) $^ -o $(TARGET) "; $(CC) $^ -o $(TARGET)
#
# $(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
# 		@mkdir -p $(BUILDDIR)
# 	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -w
#
# clean:
# 	@echo " Cleaning...";
# 	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
#
# .PHONY: clean

# edit : main.o helpfulFunctions.o menuFunctions.o recordList.o hashTable.o avlTtee.o
# 		cc -o edit main.o helpfulFunctions.o menuFunctions.o recordList.o hashTable.o avlTtee.o

objects = src/main.o src/helpfulFunctions.o src/menuFunctions.o src/recordList.o src/hashTable.o src/avlTree.o

diseaseMonitor : $(objects)
		cc -o bin/diseaseMonitor $(objects)

src/main.o : include/Interface.h
src/helpfulFunctions.o : include/Interface.h
src/menuFunctions.o : include/Interface.h
src/recordList.o : include/Interface.h
src/hashTable.o : include/Interface.h
src/avlTree.o : include/Interface.h

.PHONY: clean
clean :
		rm bin/diseaseMonitor $(objects)
