// Donsus memory allocator
#include "memory_alloc.h"

// Arena allocator
namespace utility {
DonsusAllocator::DonsusAllocator(std::size_t total_size)
    : total_size(total_size) {
  start_ptr = std::malloc(total_size);
  offset = 0;
}

DonsusAllocator::~DonsusAllocator() {
  free(start_ptr);
  start_ptr = nullptr;
}

void *DonsusAllocator::allocate(std::size_t size, std::size_t alignment) {
  std::size_t padding = 0;
  std::size_t curr_address = (std::size_t)start_ptr + (std::size_t)offset;

  if (alignment != 0 && offset % alignment != 0) {
    // requires alignment, either because of a custom alignment or because the
    // alignment doesn't match the offset
    padding = calculate_padding(curr_address, alignment);
  }
  if (offset + padding + size > total_size) {
    return nullptr;
  }

  offset += padding;

  std::size_t nextAddress =
      curr_address + padding; // the proper address with padding

  offset += size;             // occupy the needed memory
  return (void *)nextAddress; // cast it back to void*
}

std::size_t DonsusAllocator::calculate_padding(std::size_t address,
                                               std::size_t alignment) {
  std::size_t multiplier = (address / alignment) + 1;
  std::size_t alignedAddress = multiplier * alignment;
  std::size_t padding = alignedAddress - address;
  return padding;
}

} // namespace utility
