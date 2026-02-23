# bit-array
Implementation of a bit array in C 

Current implementation: 
`bool*` is used to store bits: `true` = busy

For range queries: 
Any operation on ranges `[start, end]` are inclusive (operates on `start, start+1, ... end-1, end`)

## Undefined behavior 
`size_t bit_array_size(const bit_array *bb)` does not check if `bb == NULL`, and will execute `bb->_size`. 
