/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "vector.h"
#include <assert.h>
#include <stdio.h>

/**
 * 'INITIAL_CAPACITY' the initial size of the dynamically.
 */
const size_t INITIAL_CAPACITY = 8;
/**
 * 'GROWTH_FACTOR' is how much the vector will grow by in automatic reallocation
 * (2 means double).
 */
const size_t GROWTH_FACTOR = 2;

struct vector {
    /* The function callback for the user to define the way they want to copy
     * elements */
    copy_constructor_type copy_constructor;

    /* The function callback for the user to define the way they want to destroy
     * elements */
    destructor_type destructor;

    /* The function callback for the user to define the way they a default
     * element to be constructed */
    default_constructor_type default_constructor;

    /* Void pointer to the beginning of an array of void pointers to arbitrary
     * data. */
    void **array;

    /**
     * The number of elements in the vector.
     * This is the number of actual objects held in the vector,
     * which is not necessarily equal to its capacity.
     */
    size_t size;

    /**
     * The size of the storage space currently allocated for the vector,
     * expressed in terms of elements.
     */
    size_t capacity;
};

/**
 * IMPLEMENTATION DETAILS
 *
 * The following is documented only in the .c file of vector,
 * since it is implementation specfic and does not concern the user:
 *
 * This vector is defined by the struct above.
 * The struct is complete as is and does not need any modifications.
 *
 * The only conditions of automatic reallocation is that
 * they should happen logarithmically compared to the growth of the size of the
 * vector inorder to achieve amortized constant time complexity for appending to
 * the vector.
 *
 * For our implementation automatic reallocation happens when -and only when-
 * adding to the vector makes its new  size surpass its current vector capacity
 * OR when the user calls on vector_reserve().
 * When this happens the new capacity will be whatever power of the
 * 'GROWTH_FACTOR' greater than or equal to the target capacity.
 * In the case when the new size exceeds the current capacity the target
 * capacity is the new size.
 * In the case when the user calls vector_reserve(n) the target capacity is 'n'
 * itself.
 * We have provided get_new_capacity() to help make this less ambigious.
 */

static size_t get_new_capacity(size_t target) {
    /**
     * This function works according to 'automatic reallocation'.
     * Start at 1 and keep multiplying by the GROWTH_FACTOR untl
     * you have exceeded or met your target capacity.
     */
    size_t new_capacity = 1;
    while (new_capacity < target) {
        new_capacity *= GROWTH_FACTOR;
    }
    return new_capacity;
}

vector *vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor,
                      default_constructor_type default_constructor) {
    vector *this_ = (vector*)malloc(sizeof(vector)); 
    this_->array = calloc(INITIAL_CAPACITY, sizeof(void*));
    this_->size = 0;
    if (copy_constructor == NULL && destructor == NULL && default_constructor == NULL) {
        this_->copy_constructor = shallow_copy_constructor;
        this_->destructor = shallow_destructor;
        this_->default_constructor = shallow_default_constructor;
    }
    else {
        this_->copy_constructor = copy_constructor;
        this_->destructor = destructor;
        this_->default_constructor = default_constructor;
    }
    this_->capacity = INITIAL_CAPACITY;
    return this_;
    
    // your code here
    // Casting to void to remove complier error. Remove this line when you are
    // ready.
    //(void)INITIAL_CAPACITY;
    //(void)get_new_capacity;
    //return NULL;
}

void vector_destroy(vector *this) {
    assert(this);
    //void** temp = (this->array);
    //puts("woe");
    vector_clear(this);
    //puts("1");
    free(this->array);
    this->size = 0;
    this->capacity = 0;
    free(this);
    // your code here
}

void vector_clear(vector *this) {
    assert(this);
    size_t i = 0;
    while (i < this->size) {
        this->destructor(this->array[i]);
        this->array[i] = NULL;
        i++;
    }
    this->size = 0;
    // your code here
}

void **vector_begin(vector *this) {
    assert(this);
    return this->array + 0;
}

void **vector_end(vector *this) {
    assert(this);
    return this->array + this->size;
}

size_t vector_size(vector *this) {
    assert(this);
    return this->size;
    // your code here
    //return 0;
}

void vector_resize(vector *this, size_t n) {
    assert(this);
    size_t new_s = n;
    assert(this);
    //assert(n >= 0);
    if (n < this->size) {
        while(n < this->size) {
            this->destructor(this->array[n]);
            this->array[n] = NULL;
            n++;
        }
    }
    else if (n > this->size && n > this->capacity) {
        vector_reserve(this, n);
        size_t i = this->size;
        while (i < n) {
            this->array[i] = this->default_constructor();
            i++;
        }
        //free(this->array);
        //this->capacity = new_size;
        //this->array = new_array;
    }
    else {
        size_t i = this->size;
        while (i < n) {
            this->array[i] = this->default_constructor();
            i++;
        }
    }
    this->size = new_s;

    // your code here
}

size_t vector_capacity(vector *this) {
    assert(this);
    return this->capacity;
    // your code here
    //:w
    //return 0;
}

bool vector_empty(vector *this) {
    assert(this);
    // your code here
    return this->size == 0;
}

void vector_reserve(vector *this, size_t n) {
    assert(this);
    if (n <= this->capacity)
        return;
    size_t new_size = get_new_capacity(n);
    void** new_array = calloc(new_size, sizeof(void*));
    size_t i = 0;
    while (i != this->size) {
        new_array[i] = this->array[i];
        //this->destructor(this->array[i]);
        i++;
    }
    free(this->array);
    this->array = new_array;
    this->capacity = new_size;
    //if (n > this->capacity)
      //  vector_resize(this, n);
    // your code here
}

void **vector_at(vector *this, size_t position) {
    assert(this);
    assert(position < this->size);
    return this->array + position;
    // your code here
    //return NULL;
}

void vector_set(vector *this, size_t position, void *element) {
    assert(this);
    assert(position < vector_size(this));
    this->destructor(this->array[position]);
    if (element != NULL)
        this->array[position] = this->copy_constructor(element);
    else
        this->array[position] = NULL;
    // your code here
}

void *vector_get(vector *this, size_t position) {
    assert(this);
    assert(position < vector_size(this));
    //puts("lmao");
    void* ret = this->array[position];
    // your code here
    return ret;
}

void **vector_front(vector *this) {
    assert(this);
    // your code here
    return this->array;
}

void **vector_back(vector *this) {
    assert(this);
    // your code here
    return this->array + (this->size-1);
}

void vector_push_back(vector *this, void *element) {
    assert(this);
    if(this->size == this->capacity)
        vector_reserve(this, this->size + 1);
    this->array[this->size] = this->copy_constructor(element);
    this->size++;
    // your code here
}

void vector_pop_back(vector *this) {
    assert(this);
    if (this->size == 0)
        return;
    void** rem = vector_back(this);
    this->destructor(*rem);
    rem = NULL;
    this->size--;
    // your code here
}

void vector_insert(vector *this, size_t position, void *element) {
    assert(this);
    //assert(position <= vector_size(this));  
    if (position >= this->size) {
        vector_push_back(this, element);
        return;
    }
    vector_reserve(this, this->size + 1);
    size_t index = this->size;
    while(index > position) {
        this->array[index] = this->array[index - 1];
        index--;
    }
    if (element != NULL)
        this->array[position] = this->copy_constructor(element);
    else
        this->array[position] = NULL;
    this->size++;
    // your code here
}

void vector_erase(vector *this, size_t position) {
    assert(this);
    assert(position < vector_size(this));
    if(position == this->size - 1) {
        vector_pop_back(this);
        this->size--;
        return;
    }
    size_t index = position;
    free(this->array[index]);
    for(; index < this->size-1; index++) {
        this->array[index] = this->array[index + 1];
    }
    this->array[index] = NULL;
    this->size--;

    // your code here
}



// The following is code generated:
vector *shallow_vector_create() {
    return vector_create(shallow_copy_constructor, shallow_destructor,
                         shallow_default_constructor);
}
vector *string_vector_create() {
    return vector_create(string_copy_constructor, string_destructor,
                         string_default_constructor);
}
vector *char_vector_create() {
    return vector_create(char_copy_constructor, char_destructor,
                         char_default_constructor);
}
vector *double_vector_create() {
    return vector_create(double_copy_constructor, double_destructor,
                         double_default_constructor);
}
vector *float_vector_create() {
    return vector_create(float_copy_constructor, float_destructor,
                         float_default_constructor);
}
vector *int_vector_create() {
    return vector_create(int_copy_constructor, int_destructor,
                         int_default_constructor);
}
vector *long_vector_create() {
    return vector_create(long_copy_constructor, long_destructor,
                         long_default_constructor);
}
vector *short_vector_create() {
    return vector_create(short_copy_constructor, short_destructor,
                         short_default_constructor);
}
vector *unsigned_char_vector_create() {
    return vector_create(unsigned_char_copy_constructor,
                         unsigned_char_destructor,
                         unsigned_char_default_constructor);
}
vector *unsigned_int_vector_create() {
    return vector_create(unsigned_int_copy_constructor, unsigned_int_destructor,
                         unsigned_int_default_constructor);
}
vector *unsigned_long_vector_create() {
    return vector_create(unsigned_long_copy_constructor,
                         unsigned_long_destructor,
                         unsigned_long_default_constructor);
}
vector *unsigned_short_vector_create() {
    return vector_create(unsigned_short_copy_constructor,
                         unsigned_short_destructor,
                         unsigned_short_default_constructor);
}
