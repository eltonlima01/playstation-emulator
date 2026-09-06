#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

class Cpu {
 public:
  uint32_t pc{0xbfc00000};

  void run_next_instruction() {
    uint32_t pc{this->pc};

    // uint32 instruction = load32(pc);

    this->pc += 4;

    // decode_and_execute(instruction);
  }
};

constexpr static const uint64_t BIOS_SIZE{512 * 1024};

class Bios {
 public:
  std::vector<uint8_t> data;

  Bios(const char* path) {
    std::ifstream file(path, std::ios::binary);

    std::vector<uint8_t> data(BIOS_SIZE);

    file.read(reinterpret_cast<char*>(data.data()), BIOS_SIZE);
    file.close();

    if (file.gcount() == BIOS_SIZE) {
      this->data = data;
    } else {
      std::cout << "Invalid_BIOS_size" << std::endl;
    }
  }

  uint32_t load32(uint32_t offset) const {
    const uint32_t b0{static_cast<uint32_t>(data[offset])};
    const uint32_t b1{static_cast<uint32_t>(data[++offset])};
    const uint32_t b2{static_cast<uint32_t>(data[++offset])};
    const uint32_t b3{static_cast<uint32_t>(data[++offset])};

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
  }
};