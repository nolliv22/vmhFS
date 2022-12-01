CC = gcc
FLAGS=-Wall
SRCDIR=./src
BUILDDIR=./build
TARGETS = dir main

.PHONY: all dir clean

all: $(TARGETS)

dir:
	@mkdir -p $(BUILDDIR)

main: $(SRCDIR)/main.c
	$(CC) $(FLAGS) -o $(BUILDDIR)/vmhFS $^
	@echo "Build successfully"

clean:
	rm -rf $(BUILDDIR)
	@echo "$(BUILDDIR) has been deleted"