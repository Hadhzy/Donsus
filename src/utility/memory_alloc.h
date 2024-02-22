// Donsus memory allocator
#ifndef UTILITY_MEMORY_ALLOC_H
#define UTILITY_MEMORY_ALLOC_H
#include <cstdint>
#include <iostream>

namespace utility {
class DonsusAllocator {
  // one particular block
  struct block {
    // free(block) the memory
    block(uint64_t *memory);
    ~block();
    uint64_t *memory;
    uint64_t position;
    block *next = nullptr; // next block
  };

public:
  DonsusAllocator() = default;
  DonsusAllocator(uint64_t block_size);
  // free(all) the memory
  ~DonsusAllocator();

  void print_used() const;

  // Allocate uninitialised object and set up the constructor

  template <typename type, typename... params>
  auto alloc_constructor(params... pargs) -> type * {
    return ::new (std::malloc(sizeof(type))) type(pargs...);
  }

  template <typename type> auto alloc() -> type * {
    type *a = (type *)std::malloc(sizeof(type));
    return a;
  }
  /*  auto alloc(uint64_t size) -> void *;*/
  /*  auto alloc_zero(uint64_t size) -> void *;*/

  /*
    template <typename type> auto alloc() -> type * {
      return static_cast<type *>(alloc(sizeof(type)));
    };

    template <typename type, typename... value_types>
    auto emplace(value_types &&...values) -> type * {
      return new (alloc(sizeof(type)))
    type(std::forward<value_types>(values)...);
    }
  */

  template <typename type> void *emplace() { return std::malloc(sizeof(type)); }

  auto get_block_count() const -> uint64_t;

  auto get_block_size() const -> uint64_t;

private:
  // void alloc_block();

  block *first_block;
  block *current_block;

  uint64_t block_size;
  uint64_t block_count = 0;
};
} // namespace utility

#endif
