
#include <vector>
#include <string.h>
#include "memoryManagement.hpp"

#define CHUNK_SIZE 512 //in bytes
#define BLOCK_SIZE 5   //in bytes  


int test_random_number (MemoryManagement mm, int cycles)
{
    int times = 0;
    char check_str[BLOCK_SIZE] = {0};
    void* block = NULL;
    std::vector<void*> test_array;

    srand (time(NULL));

    for(int i=0; i < BLOCK_SIZE; i++) check_str[i] = rand() % 126 + 10;


    for (int i=0; i < cycles; i++)
    {
        times = rand() % (CHUNK_SIZE/BLOCK_SIZE) + 1;
        for(int j=0; j < times; j++)
        {
            block = mm.allocate();

            printf("%d: Block pointer is: %p \n",j, block);  

            strncpy((char*)block, check_str, BLOCK_SIZE);
        
            test_array.push_back(block);
        }


        for(int k=0; k < times; k++)
        {
            block = test_array.back();
            test_array.pop_back();
            if(0 != strncmp((char*)block, check_str, BLOCK_SIZE))
            {
                printf("\n<<<=== TEST ERROR ===>>>\n");
                printf("i: %d, k: %d, block: %s, block_ptr %p\n", i, k, (char*)block, block);
                return -1;
            }
            mm.free(block);
        
        }
 
    }

}

int test_invalid_pointers(MemoryManagement mm)
{
    void* block = mm.allocate();

    printf("Invalid pointers 1: %d\n", mm.free(NULL));

    printf("Invalid pointers 2: %d\n", mm.free(nullptr));
    printf("Invalid pointers 3: %d\n", mm.free((void*)0x1016dce));

    printf("Invalid pointers 4: %d\n", mm.free(block));
}

int test_stack_overflow(MemoryManagement mm)
{
    std::vector<void*> test_array;
    void* block = NULL;
    
    for(int i=0; i < CHUNK_SIZE/BLOCK_SIZE + 5; i++)
    {
       block = mm.allocate();
       
       test_array.push_back(block);  
    }

    for(auto const& val: test_array)
    {
        mm.free(test_array.back());
        test_array.pop_back();
    }

    return 0;
}

int test_random_allocation(MemoryManagement mm, int cycles)
{
    std::vector<void*> test_array;
    int j_max = 0;
    int k_max = 0;
    int element = 0;

    srand (time(NULL));

    for(int i=0; i < cycles; i++)
    {
        //findout a total number of intial allocation based on number of blocks availiable.
        j_max = rand() % (CHUNK_SIZE/BLOCK_SIZE);
        for(int j=0; j < j_max; j++)
        {
            test_array.push_back(mm.allocate());
        }

        //findout number of elements should be freed based on a total number of the initial allocation.
        k_max = rand()%test_array.size();

        for(int k=0; k < k_max; k++)
        {
            element = rand() % test_array.size();
            mm.free(test_array[element]);
            printf("%d: RandomAllocate - Free: %p\n", k, test_array[element]);
            test_array.erase(test_array.begin() + element);            
        }
    } 
}

int main()
{
    MemoryManagement mm = MemoryManagement(malloc(CHUNK_SIZE*sizeof(char)), CHUNK_SIZE, BLOCK_SIZE);
    
    test_random_number(mm, 100);

    test_invalid_pointers(mm);

    test_stack_overflow(mm);

    test_random_allocation(mm, 100);
    return 0;
}
