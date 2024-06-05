#ifndef DONSUS_SLICES_H
#define DONSUS_SLICES_H
#include <iostream>
#include <stdint.h>
namespace utility {

template <typename type> class slice {
public:
  slice() = default;
  slice(type *data) : m_data(data) {}
  [[nodiscard]] auto operator[](uint64_t  index) -> type & {
    return m_data[index];
  }

private:
  type *m_data;
};
} // namespace utility
#endif