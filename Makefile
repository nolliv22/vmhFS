CC = gcc
FLAGS=-Wall
SRCDIR=./src
BUILDDIR=./build
TARGETS = dir main
export LIBRARY_PATH=/usr/local/Cellar/gsl/2.7.1/lib/
.PHONY: all dir clean

all: $(TARGETS)

dir:
	@mkdir -p $(BUILDDIR)

main: $(SRCDIR)/main.c
	$(CC) $(FLAGS) -o $(BUILDDIR)/vhmFS $^ -lrt
	@echo "Build successfully"

clean:
	rm -rf $(BUILDDIR)
	@echo "$(BUILDDIR) has been deleted"