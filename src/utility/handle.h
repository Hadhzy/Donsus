#ifndef UTILITY_HANDLE_H
#define UTILITY_HANDLE_H
// wrapper for raw pointers

namespace utility {

template <typename type> class handle {
public:
  handle() = default;
  handle(type *ptr) : m_ptr(ptr) {}

  template <typename other_type>
  explicit handle(other_type *other) : m_ptr(reinterpret_cast<type *>(other)) {}

  template <typename other_type>
  explicit handle(handle<other_type> other)
      : m_ptr(reinterpret_cast<type *>(other)) {}

  auto operator*() const -> type & { return *m_ptr; }
  auto operator->() const -> type * { return m_ptr; }

  /* // for iterators
   handle& operator++() {m_ptr++; return *this;}
   handle operator++(int) { handle tmp = *this; ++(*this); return tmp;}

   // for iterator(range-based for loop)
   friend bool operator== (const handle& a, const handle& b) { return a.m_ptr ==
   b.m_ptr; }; friend bool operator!= (const handle& a, const handle& b) {
   return a.m_ptr != b.m_ptr; };
 */
  auto get() const -> type * { return m_ptr; }
  auto operator==(const handle &other) const -> bool {
    return m_ptr == other.m_ptr;
  }
  operator bool() const noexcept { return m_ptr != nullptr; }

protected:
  type *m_ptr = nullptr;
};

} // namespace utility

#endif