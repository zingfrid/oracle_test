#include <stdio.h>
#include <string.h>
#include "memoryManagement.hpp"

MemoryManagement::MemoryManagement(void* _memory_chunk, size_t _chunk_size, size_t _block_size)
{
    int block_num = 0;

    if((NULL == _memory_chunk) || (_block_size > _chunk_size))
    {
        return;
    }

    //memory_bank = _memory_chunk; //See comment in the destructor for reason why this line is here.

    block_size = _block_size*sizeof(char);

    //We are interested only in full blocks. The last one will be greater in the case of no clear division.
    block_num = _chunk_size / block_size;
    void* tmp_ptr = NULL;

    for(int i=0; i < block_num; i++)
    {   //cast _memory_chunk to char* to be sure that we're working with bytes.
        tmp_ptr = (char*)_memory_chunk + block_size * i;

        if (NULL != tmp_ptr)
        {
            memory_free_set.insert(tmp_ptr);
            memory_map[tmp_ptr] = false;
            memset(tmp_ptr,'\0', block_size); 
        }              
    }


}

void* MemoryManagement::allocate() 
{
    if(memory_free_set.empty())
    {
         printf("STACK is empty. Cannot allocate more memory! \n");
         return (NULL);
    }
    else
    {
        std::set<void*>::iterator it = memory_free_set.begin();
        memory_free_set.erase(it);

        return *it;
    }
}    

int MemoryManagement::free(void* ptr)
{
    //if pointer is null - no sense to continue
    if (NULL == ptr)
    {
        return MEM_PARAMS_ERROR;
    }

    //if map is no contain this pointer - it was not allocated by us, so doing nothing.
    if(0 == memory_map.count(ptr))
    {
        return MEM_WRONG_PTR;
    }

    memset(ptr, '\0', block_size);
    memory_free_set.insert(ptr);
    
    return MEM_OK;
}

MemoryManagement::~MemoryManagement()
{   //According to best practice, everybody who allocate memory shoul cares about free it.
    //But in case behaviour should be changed and memory bank should be freed - uncomment line below and appropriate line in the
    //constructor.
    //std::free(memory_bank);
}

