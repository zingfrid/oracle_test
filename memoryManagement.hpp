#include <map>
#include <set> 

enum
{
    MEM_OK           = 0,
    MEM_PARAMS_ERROR = -1,
    MEM_WRONG_PTR    = -2
} MemoryError;


class MemoryManagement
{
    private:
        void* memory_bank = NULL;
        size_t block_size = 0;
        std::map<void*, bool> memory_map;
        std::set<void*> memory_free_set;

    public:
        MemoryManagement(void* memory_chunk, size_t chunk_size, size_t _block_size);
        void* allocate(); 
        int free(void* ptr);
         ~MemoryManagement();
};
