#include <cstdint>

class Cpu {
 public:
  uint32_t pc;

  void run_next_instruction() {
    uint32_t pc{this->pc};

    // uint32 instruction = load32(pc);

    this->pc += 4;

    // decode_and_execute(instruction);
  }
};