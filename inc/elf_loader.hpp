// Copyright (c) 2017-2018 Group of Computer Architecture, University of Bremen <riscv@systemc-verification.org>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// https://github.com/agra-uni-bremen/riscv-vp

#pragma once

#include <boost/iostreams/device/mapped_file.hpp>

#include <cstdint>
#include <vector>

template <typename T>
struct GenericElfLoader {
  typedef typename T::addr_t addr_t;
  typedef typename T::Elf_Ehdr Elf_Ehdr;
  typedef typename T::Elf_Phdr Elf_Phdr;
  typedef typename T::Elf_Shdr Elf_Shdr;
  typedef typename T::Elf_Sym Elf_Sym;
  static_assert(sizeof(addr_t) == sizeof(Elf_Ehdr::e_entry), "architecture mismatch");

  const char *filename;
  boost::iostreams::mapped_file_source elf;
  const Elf_Ehdr *hdr;

  GenericElfLoader(const char *filename) : filename(filename), elf(filename) {
    assert(elf.is_open() && "file not open");

    hdr = reinterpret_cast<const Elf_Ehdr *>(elf.data());
  }

  std::vector<const Elf_Phdr *> get_load_sections() {
    std::vector<const Elf_Phdr *> sections;

    for (int i = 0; i < hdr->e_phnum; ++i) {
      const Elf_Phdr *p =
	reinterpret_cast<const typename T::Elf_Phdr *>(elf.data() + hdr->e_phoff + hdr->e_phentsize * i);

      if (p->p_type != T::PT_LOAD)
	continue;

      sections.push_back(p);
    }

    return sections;
  }

  void load_executable_image(uint8_t *dst, addr_t size, addr_t offset, bool use_vaddr = true) {
    for (auto section : get_load_sections()) {
      if (use_vaddr) {
	assert((section->p_vaddr >= offset) && (section->p_vaddr + section->p_memsz < offset + size));

	// NOTE: if memsz is larger than filesz, the additional bytes are zero initialized (auto. done for
	// memory)
	memcpy(dst + section->p_vaddr - offset, elf.data() + section->p_offset, section->p_filesz);
      } else {
	if (section->p_filesz == 0) {
	  // skipping empty sections, we are 0 initialized
	  continue;
	}
	if (section->p_paddr < offset) {
	  std::cerr << "Section physical address 0x" << std::hex << section->p_paddr
		    << " not in local offset (0x" << std::hex << offset << ")!" << std::endl;
	  // raise(std::runtime_error("elf cant be loaded"));
	}
	if (section->p_paddr + section->p_memsz >= offset + size) {
	  std::cerr << "Section would overlap memory (0x" << std::hex << section->p_paddr << " + 0x"
		    << std::hex << section->p_memsz << ") >= 0x" << std::hex << offset + size << std::endl;
	  // raise(std::runtime_error("elf cant be loaded"));
	}
	assert((section->p_paddr >= offset) && (section->p_paddr + section->p_memsz < offset + size));

	// NOTE: if memsz is larger than filesz, the additional bytes are zero initialized (auto. done for
	// memory)
	memcpy(dst + section->p_paddr - offset, elf.data() + section->p_offset, section->p_filesz);
      }
    }
  }

  addr_t get_memory_end() {
    const Elf_Phdr *last =
      reinterpret_cast<const Elf_Phdr *>(elf.data() + hdr->e_phoff + hdr->e_phentsize * (hdr->e_phnum - 1));

    return last->p_vaddr + last->p_memsz;
  }

  addr_t get_heap_addr() {
    // return first 8 byte aligned address after the memory image
    auto s = get_memory_end();
    return s + s % 8;
  }

  addr_t get_entrypoint() {
    return hdr->e_entry;
  }

  const char *get_section_string_table() {
    assert(hdr->e_shoff != 0 && "string table section not available");

    const Elf_Shdr *s =
      reinterpret_cast<const Elf_Shdr *>(elf.data() + hdr->e_shoff + hdr->e_shentsize * hdr->e_shstrndx);
    const char *start = elf.data() + s->sh_offset;
    return start;
  }

  const char *get_symbol_string_table() {
    auto s = get_section(".strtab");
    return elf.data() + s->sh_offset;
  }

  const Elf_Sym *get_symbol(const char *symbol_name) {
    const Elf_Shdr *s = get_section(".symtab");
    const char *strings = get_symbol_string_table();

    assert(s->sh_size % sizeof(Elf_Sym) == 0);

    auto num_entries = s->sh_size / sizeof(typename T::Elf_Sym);
    for (unsigned i = 0; i < num_entries; ++i) {
      const Elf_Sym *p = reinterpret_cast<const Elf_Sym *>(elf.data() + s->sh_offset + i * sizeof(Elf_Sym));

      // std::cout << "check symbol: " << strings + p->st_name << std::endl;

      if (!strcmp(strings + p->st_name, symbol_name)) {
	return p;
      }
    }

    throw std::runtime_error("unable to find symbol in the symbol table " + std::string(symbol_name));
  }

  addr_t get_begin_signature_address() {
    auto p = get_symbol("begin_signature");
    return p->st_value;
  }

  addr_t get_end_signature_address() {
    auto p = get_symbol("end_signature");
    return p->st_value;
  }

  addr_t get_to_host_address() {
    auto p = get_symbol("tohost");
    return p->st_value;
  }

  const Elf_Shdr *get_section(const char *section_name) {
    if (hdr->e_shoff == 0) {
      throw std::runtime_error("unable to find section address, section table not available: " +
			       std::string(section_name));
    }

    const char *strings = get_section_string_table();

    for (unsigned i = 0; i < hdr->e_shnum; ++i) {
      const Elf_Shdr *s = reinterpret_cast<const Elf_Shdr *>(elf.data() + hdr->e_shoff + hdr->e_shentsize * i);

      // std::cout << "check section: " << strings + s->sh_name << std::endl;

      if (!strcmp(strings + s->sh_name, section_name)) {
	return s;
      }
    }

    throw std::runtime_error("unable to find section address, section seems not available: " +
		                         std::string(section_name));
  }
};

// see: http://wiki.osdev.org/ELF_Tutorial for ELF definitions

typedef uint16_t Elf32_Half;  // Unsigned half int
typedef uint32_t Elf32_Off;   // Unsigned offset
typedef uint32_t Elf32_Addr;  // Unsigned address
typedef uint32_t Elf32_Word;  // Unsigned int
typedef int32_t Elf32_Sword;  // Signed int

constexpr unsigned ELF_NIDENT = 16;

typedef struct {
  uint8_t e_ident[ELF_NIDENT];
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;
  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct {
  Elf32_Word p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
} Elf32_Phdr;

typedef struct {
  Elf32_Word sh_name;
  Elf32_Word sh_type;
  Elf32_Word sh_flags;
  Elf32_Addr sh_addr;
  Elf32_Off sh_offset;
  Elf32_Word sh_size;
  Elf32_Word sh_link;
  Elf32_Word sh_info;
  Elf32_Word sh_addralign;
  Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf32_Half st_shndx;
} Elf32_Sym;

enum Elf32_PhdrType { PT_NULL = 0, PT_LOAD = 1, PT_DYNAMIC = 2, PT_INTERP = 3, PT_NOTE = 4, PT_SHLIB = 5, PT_PHDR = 6 };

struct Elf32Types {
  typedef uint32_t addr_t;
  typedef Elf32_Ehdr Elf_Ehdr;
  typedef Elf32_Phdr Elf_Phdr;
  typedef Elf32_Shdr Elf_Shdr;
  typedef Elf32_Sym Elf_Sym;
  static constexpr unsigned PT_LOAD = Elf32_PhdrType::PT_LOAD;
};

typedef GenericElfLoader<Elf32Types> ELFLoader;

