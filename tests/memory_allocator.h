// pool allocator
#include <iostream>
#include <stdint.h>

// declaration of the class
class BlockAllocator{
public:
    struct block{
        block(uint8_t *memory);
        ~block();

        uint8_t* position;
        uint8_t* memory;
        block *next = nullptr; // referencing to itself
    };

    BlockAllocator(uint8_t block_size);
    ~BlockAllocator();

    auto allocate(uint8_t size) -> void*;
    auto allocate_zero(uint8_t size) ->void*;

private:
    void allocate_block();
    block* first_block = nullptr;
    block* current_block = nullptr;

    uint8_t block_size;
    uint8_t block_count = 0;
};