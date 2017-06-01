# table-string
A fast string concatenator. It is simply a C array of characters, encapsulated in a class that keeps track of array size and number of allocated bytes.
Allocation is done at start and when size exceeds allocated bytes. For performance reasons, the number of allocations should be mininized (ideally one time).


# Usage
```c++
table_str_t table(3);
table.add("ab", 2);
table.dump();
table.add("cdefgh", 6);
table.dump();
```

# Benchmarks
Substancial improvement over C++ std::string.

| API                   | Seconds           
| ----------------------|----| 
| SDS                   | 19 |  
| std::string           | 11 |  
| std::string (reserve) | 9  |  
| table_str_t           | 1  |  

# Benchmark
```c++
clock_gettime_t timer;
const size_t nbr = 1000 * 1000 * 10;
const char* s = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
size_t len = strlen(s);
timer.start();
table_str_t table(nbr *len);
for (size_t idx = 0; idx < nbr; ++idx)
{
  table.add(s, len);
}
timer.now("end table");
timer.stop();
```
