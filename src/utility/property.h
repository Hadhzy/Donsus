#include "handle.h"

namespace utility {
struct empty_property {};

template <typename... properties> class property {
public:
  template <typename type> auto get() -> type & {
    return *static_cast<type *>(m_property);
  }
  void set_property(void *value) { m_property = value; }

protected:
  void *m_property = nullptr;
};

} // namespace utility