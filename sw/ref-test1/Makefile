mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

all : main.c
	riscv32-unknown-elf-gcc -o $(current_dir) main.c bootstrap.S -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax
	mv $(current_dir) ../bin/

dump-elf: all
	riscv32-unknown-elf-readelf -a ../bin/$(current_dir)

dump-code: all
	riscv32-unknown-elf-objdump -dj .text -M no-aliases ../bin/$(current_dir)

run: all
	goldcrest ../bin/$(current_dir) --quiet

build-x86:
	gcc main.c -o $(current_dir)-x86 -DX86

clean:
	rm -f $(current_dir) main.o
