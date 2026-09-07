#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

class Cpu {
 private:
  uint32_t pc{0xbfc00000};

 public:
  void run_next_instruction() {
    uint32_t pc{this->pc};

    // uint32 instruction = load32(pc);

    this->pc += 4;

    // decode_and_execute(instruction);
  }
};

constexpr static const uint64_t BIOS_SIZE{512 * 1024};

class Bios {
 private:
  std::vector<uint8_t> data;

 public:
  Bios(const char* path) {
    std::ifstream file(path, std::ios::binary);

    std::vector<uint8_t> data(BIOS_SIZE);

    file.read(reinterpret_cast<char*>(data.data()), BIOS_SIZE);
    file.close();

    if (file.gcount() == BIOS_SIZE) {
      this->data = std::move(data);
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

namespace map {
class Range {
 public:
  uint32_t start;
  uint32_t length;

  Range(const uint32_t start, const uint32_t length)
  : start{start}, length{length} {}

  std::optional<uint32_t> contains(const uint32_t addr) const {
    if ((addr >= start) && (addr < (start + length))) {
      return addr - start;
    } else {
      return std::nullopt;
    }
  }
};

const static Range BIOS(0xbfc00000, 512 * 1024);
};  // namespace map

class Interconnect {
 public:
  Bios bios;

  Interconnect(const Bios& bios) : bios{bios} {}

  uint32_t load32(const uint32_t addr) const {
    const std::optional offset{map::BIOS.contains(addr)};

    if (offset) {
      return bios.load32(offset.value());
    } else {
      std::cout << "unhandled_fetch32_at_address_" << std::hex << addr << std::endl;

      throw -1;
    }
  }
};