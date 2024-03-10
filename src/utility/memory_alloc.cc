// Donsus memory allocator
#include "memory_alloc.h"

namespace utility {
DonsusAllocator::block::block(uint64_t *memory) : memory(memory), position(0) {}
// free one particular memory
DonsusAllocator::block::~block() {
  // "delete temp"
  std::free(memory);
}

DonsusAllocator::DonsusAllocator(uint64_t block_size) : block_size(block_size) {
  // alloc_block(); // allocate the first block
  first_block = current_block;
};

DonsusAllocator::~DonsusAllocator() {
  while (first_block) {
    const block *temp = first_block;
    first_block = first_block->next;
    delete temp; // call block destructor
  }
}

/*template <typename type>
auto DonsusAllocator::alloc() -> type* {
*//*  assert(size <= block_size); // memory alignment
  if (size == 0) {
    return nullptr;
  }*//*

  *//*if (current_block->position + size >= block_size) {
    alloc_block();
  }*//*
  auto a = new type;
  return a;
*//*  alloc_block();
  void *memory = current_block->memory +
                 current_block->position; // end of the current block
  current_block->position += sizeof(type);

  return (type*)memory;*//*
}*/

void DonsusAllocator::alloc_block() {
  auto memory =
      static_cast<uint64_t *>(std::malloc(static_cast<size_t>(block_size)));

  auto new_block = new block(memory);

  if (current_block) {
    current_block->next = new_block;
  }

  current_block = new_block;
  block_count++;
}
} // namespace utility
