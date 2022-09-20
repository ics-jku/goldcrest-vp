mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

all : main.o
	riscv32-unknown-elf-ld main.o -o $(current_dir)
	mv $(current_dir) ../bin/

main.o : main.S data.s
	riscv32-unknown-elf-as main.S -o main.o -march=rv32i -mno-relax

dump-elf: all
	riscv32-unknown-elf-readelf -a ../bin/$(current_dir)

dump-code: all
	riscv32-unknown-elf-objdump -dj .text -M no-aliases ../bin/$(current_dir)

run: all
	goldcrest ../bin/$(current_dir) --quiet

clean:
	rm -f $(current_dir) main.o
