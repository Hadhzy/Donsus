// Donsus memory allocator
#ifndef UTILITY_MEMORY_ALLOC_H
#define UTILITY_MEMORY_ALLOC_H
#include <cstdint>
#include <iostream>

// Arena allocator:https://github.com/mtrebi/memory-allocators/tree/master

namespace utility {
class DonsusAllocator {
public:
  DonsusAllocator() = default;
  DonsusAllocator(std::size_t block_size);
  // free(all) the memory
  ~DonsusAllocator();

  template <typename type> auto r_alloc() -> type * {
    auto a = new (allocate(sizeof(type), 2 * sizeof(void *))) type;
    return a;
  }

private:
  void *allocate(std::size_t size, std::size_t alignment = 0);
  std::size_t calculate_padding(std::size_t address, std::size_t alignment);
  std::size_t total_size;

  void *start_ptr;
  std::size_t offset;
};
} // namespace utility
#endif
