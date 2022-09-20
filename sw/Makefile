SUBDIRS := $(wildcard */.)
TESTBINS := $(wildcard bin/asm-*)

all: bindir $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm -rf bin

bindir:
	mkdir -p bin
	ln -sfn ../../build/goldcrest bin

$(TESTBINS):
	goldcrest $@ --quiet

run: $(TESTBINS)

.PHONY: all $(SUBDIRS) test-all $(TESTBINS)
