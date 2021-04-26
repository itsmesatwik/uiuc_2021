/**
 * Malloc Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
 * MEMORY METADATA STRUCT
 *
 * ACTUAL DATA CAN BE ACCESSED BY ADDING H_SIZE TO THE >heap_t POINTER
 */

typedef struct heap_t {
    size_t size;
    int free;
    struct heap_t *next_free;
    struct heap_t *prev;
} heap_t;


static const size_t H_SIZE = sizeof(heap_t);
static const size_t THRESHOLD = 500000000; 
static const size_t S_THRESHOLD = H_SIZE * 1024; 

static size_t allocated_mem = 0;

static heap_t *tail = NULL;

static heap_t *fhead = NULL;


void *new_memory(size_t size) {
    void *mem = sbrk(size + H_SIZE);
    if (mem == (void*)-1) {
        return NULL;
    }
    heap_t *new_mem = (heap_t*)(mem);
    new_mem->size = size;
    new_mem->free = 0;
    new_mem->next_free = NULL;
    if (tail == NULL) {
        new_mem->prev = NULL;
        tail = new_mem;
    }
    else {
        new_mem->prev = tail;
        tail = new_mem;
    }
    return (void*)(new_mem + 1);
}



void remove_from_free(heap_t *ptr) {
    heap_t *prev = NULL;
    heap_t *iter = fhead;
    while(iter!= NULL) {
        if (iter == ptr) {
            if (prev == NULL)
                fhead = fhead->next_free;
            else {
                prev->next_free = iter->next_free;
            }
            iter->free = 0; // SET FREE TO 0 
            iter->next_free = NULL;
            return;
        }
        prev = iter;
        iter = iter->next_free;
    }
}

void *first_fit(size_t size) {
    if (fhead == NULL) {
        return NULL;
    }
    heap_t *prev = NULL;
    heap_t *iter = fhead;
    while (iter != NULL) {
        if (iter->size >= size) {
            if (prev == NULL)
                fhead = fhead->next_free;
            else
                prev->next_free = iter->next_free;
            iter->free = 0;
            iter->next_free = NULL;
            return ((void*)iter + H_SIZE); // RETURN THE VOID POINTER
        }
        prev = iter;
        iter = iter->next_free;
    }

    return NULL;
    

}

void *extend_tail(size_t size) {
    size_t extension_size = size - tail->size;
    sbrk(extension_size);
    tail->size += extension_size;
    tail->free = 0;
    remove_from_free(tail);
    allocated_mem += extension_size;
    return ((void*)tail + H_SIZE);
}

void free_insert(heap_t *freed) {
    freed->next_free = fhead;
    fhead = freed;
}


void coalesce(heap_t *ptr) {
    heap_t *next = ((void*)ptr + H_SIZE + ptr->size);
    heap_t *next_next = (void*)next + H_SIZE + next->size;
    if(next != sbrk(0)) {
        if(next->free) {
            ptr->size += (next->size + H_SIZE);
            next_next->prev = ptr;
            if(next == tail)
                tail = ptr;
            remove_from_free(next);
        }
    }

    heap_t *pre = ptr->prev;
    if (pre == NULL) {
        return;
    }
    if (pre->free) {
        pre->size += (ptr->size + H_SIZE);
        next_next->prev = pre;
        if (next != sbrk(0))
            next->prev = pre;
        remove_from_free(ptr);
    }

}

void split(void *ptr, size_t size) {
    heap_t *current = ptr - H_SIZE;
    if (current->size - size < S_THRESHOLD)
        return;
    if (size < THRESHOLD )
        return;
    heap_t *new_addition = (heap_t*)(ptr + size);
    
    new_addition->size = current->size - size - H_SIZE;
    new_addition->free = 1;
    new_addition->prev = current;
    
    heap_t *next = (heap_t*)(ptr + current->size);
    if (next == sbrk(0)) {
        tail = new_addition;
    }
    else {
        next->prev = new_addition;
    }

    free_insert(new_addition);
    current->size = size;

}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    void *mem = malloc(size*num);
    if (mem != NULL)
        memset(mem, 0, size*num);
    // implement calloc!
    return mem;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    void *first = first_fit(size); // Find best fit
    if (first == NULL) {
        if (tail != NULL && tail->free) {
            void *ret = extend_tail(size);
            return ret;
        }
        allocated_mem += size;
        void *ret = new_memory(size); // SBRK new memory
        return ret;
    }
    //FOUND FIT
    split(first, size);
    return first;
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
    if (!ptr) {
        return;
    }
    heap_t *cmp = (heap_t*)(ptr - H_SIZE);
    cmp->free = 1;
    cmp->next_free = fhead;
    fhead = cmp;
    if (cmp->size > THRESHOLD )
        coalesce(cmp);
    
    return;
    // implement free!
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
    if (!ptr)
        return malloc(size);

    if (size == 0) {
        free(ptr);
        return NULL;
    }


    heap_t *data = ptr - H_SIZE;
    if (data->size >= size) {
        return ptr;
    }
    void *new_loc = malloc(size);
    if (new_loc == NULL)
        return NULL;
    memcpy(new_loc, ptr, data->size);
    free(ptr);
    return new_loc;
}
