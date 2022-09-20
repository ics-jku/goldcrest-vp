#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "core.hpp"
#include "compliant.hpp"
#include "bitops.hpp"
#include "memory.hpp"
#include "microcode.hpp"
#include "elf_loader.hpp"

using namespace std;
namespace po = boost::program_options;

void dump_signature(Memory* memory);

struct Options : public po::options_description {
public:
  typedef unsigned int addr_t;
  std::string input_program;
  size_t memory_size = 1024 * 1024 * 32;
  int steps = -1;
  bool quiet = false;
  bool signature = false;
  
  Options(void) {
    add_options()
      ("help", "produce help message")
      ("input-file", po::value<std::string>(&input_program)->required(), "input file to use for execution")
      ("memory-size", po::value<size_t>(&memory_size), "set memory size")
      ("steps", po::value<int>(&steps), "RISC-V steps")
      ("quiet", po::bool_switch(&quiet), "suppress output")
      ("signature", po::bool_switch(&signature), "output signature");
      pos.add("input-file", 1);
  }

  void parse(int argc, char **argv) {
    try {
      auto parser = po::command_line_parser(argc, argv);
      parser.options(*this).positional(pos);

      po::store(parser.run(), vm);

      if (vm.count("help")) {
	std::cout << *this << std::endl;
	exit(0);
      }

      po::notify(vm);
    } catch (po::error &e) {
      std::cerr
	<< "Error parsing command line options: "
	<< e.what()
	<< std::endl;
      exit(1);
    }
  }

  private:
  po::positional_options_description pos;
  po::variables_map vm;
};

int main(int argc, char **argv) {
  Options options;
  options.parse(argc, argv);

  auto memory = new Memory(options.memory_size);
  memory->load_elf(options.input_program);
  GoldcrestCore* core = (GoldcrestCore*) new CompliantCore(memory);
  //GoldcrestCore* core = (GoldcrestCore*) new BitopsCore(memory);

  int steps = 0;
  while (!memory->stopped && (!(options.steps != -1) || steps < options.steps)) {
    auto instruction = memory->fetch();
    if (!options.quiet) cout << "RiscV 0x" << hex << memory->load<uint32_t>(PC) << " = 0x" << hex << instruction << endl;
    core->execute(instruction);
    steps++;
  }
 
  if (!options.quiet) core->dump_state();

  if (options.signature) dump_signature(memory);

  printf("Ran for  %lu cycles\n", core->cycles + memory->delay);
  printf("Executed %lu RISC-V instructions\n", core->rv_instructions);  
  printf("%f cycles per RISC-V instruction\n", (((float) core->cycles + memory->delay)/core->rv_instructions));
}

// Dumps a signature for the RISC-V architectural tests
void dump_signature(Memory* memory) {
  unsigned int sig_start = memory->load<uint32_t>(300);
  unsigned int sig_end = memory->load<uint32_t>(304);

  for (unsigned int addr = sig_start; addr < sig_end; addr += 4) {
    cerr << hex << setfill('0') << setw(8) << memory->load<uint32_t>(addr) << endl;
  }
}
