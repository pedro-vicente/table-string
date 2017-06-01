#ifndef TABLE_STRING_HH
#define TABLE_STRING_HH 1

#include <stdlib.h>

class table_str_t
{
public:
  table_str_t(size_t nbr_alloc);
  ~table_str_t();
  size_t m_size;
  size_t m_capacity;
  size_t m_nbr_alloc;
  char *m_str;
  void add(const char* str, size_t nbr);
  void dump();
};

#endif

