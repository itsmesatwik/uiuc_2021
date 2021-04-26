Homework 0
----------

### Watch the videos and write up your answers to the following questions

**Important!**

The virtual machine-in-your-browser and the videos you need for HW0 are here:

<http://cs-education.github.io/sys/>

Questions? Comments? Use Piazza: <https://piazza.com/illinois/spring2019/cs241>

The in-browser virtual machine runs entirely in Javascript and is fastest in Chrome. Note the VM and any code you write is reset when you reload the page, **so copy your code to a separate document.** The post-video challenges are not part of homework 0 but you learn the most by doing rather than just passively watching - so we suggest you have some fun with each end-of-video challenge.

HW0 questions are below. Please use this document to write the answers. This will be hand graded.

### Chapter 1

In which our intrepid hero battles standard out, standard error, file descriptors and writing to files

1.  **Hello, World! (system call style)** Write a program that uses `write()` to print out “Hi! My name is &lt;Your Name&gt;”.

```c
// Your code here
#include <unistd.h>

int main() {
    write(1, "Hi! My name is Satwik", 21);
    return 0;
}
```

2.  **Hello, Standard Error Stream!** Write a function to print out a triangle of height `n` to standard error. Your function should have the signature `void write_triangle(int n)` and should use `write()`. The triangle should look like this, for n = 3:

```
*
**
***
```

```c
// Your code here
void write_triangle(int n) {
    int count = 1;
    int count2 = 1;
    for (count; count <= n; count++) {
        for (count2; count2 <= count; count2++) {
            write(2, "*", 1);
        }
        count2 = 1;
        write(2,"\n",1);
    }
}
```

3.  **Writing to files** Take your program from “Hello, World!” modify it write to a file called `hello_world.txt`. Make sure to to use correct flags and a correct mode for `open()` (`man 2 open` is your friend).

```c

// Your code here
#include <unistd.h>
#include <sys/types.h>
#include <sys.stat.h>
#include <fcnt1.h>


int main() {

    mode_t file_mode = S_IRUSR | S_IWUSR;
    int output_filedes = open("hello_world.txt", O_CREAT | O_RDWR | O_TRUNC, file_mode);
    write(output_filedes, "Hi! My name is Satwik", 21);
    close(output_filedes);
    return 0;
}

```

5. **Not everything is a system call** Take your program from “Writing to files” and replace `write()` with `printf()`. *Make sure to print to the file instead of standard out!*

```c
// Your code here
#include <unistd.h>
#include <sys/types.h>
#include <sys.stat.h>
#include <fcnt1.h>
#include <stdio.h>


int main() {

    mode_t file_mode = S_IRUSR | S_IWUSR;
    close(1);
    int output_filedes = open("hello_world.txt", O_CREAT | O_RDWR | O_TRUNC, file_mode);
    printf(output_filedes, "Hi! My name is Satwik");
    close(output_filedes);
    return 0;
}

```

6.  What are some differences between `write()` and `printf()`?

```c
// Your code here
write() is a system call whereas printf() is standard library function provided with the environment.
write() needs a filedescriptor as an argument to write to whereas prinf() writes to the filedes 1 by design and can't be changed unless we make something outher than the output stream point to the filedes 1.
write needs the number of bytes that you want to write whereas printf does that on it's own.
printf inherently calls write as well but allows an extra level of abstraction.
```

### Chapter 2

Sizing up C types and their limits, `int` and `char` arrays, and incrementing pointers

1.  How many bits are there in a byte?

```c
// Your answer here
at least 8 bits
```

2.  How many bytes are there in a `char`?

```c
// Your answer here
1 byte
```

3.  How many bytes the following are on your machine? 

* `int`: 4
* `double`: 8
* `float`: 4
* `long`: 8
* `long long`: 8 

4.  On a machine with 8 byte integers, the declaration for the variable `data` is `int data[8]`. If the address of data is `0x7fbd9d40`, then what is the address of `data+2`?

```c
// Your answer here
data+2 = 0x7fbd9d50
```

5.  What is `data[3]` equivalent to in C? Hint: what does C convert `data[3]` to before dereferencing the address? Remember, the type of a string constant `abc` is an array.

```c
// Your answer here
*(data + 3)
or
3[data]

```

6.  Why does this segfault?

```c
char *ptr = "hello";
*ptr = 'J';
//constants are read only and the hardware is not able to differentiate between the different 
//memories with different access types hence tries to write to a read only memory and gets a segmentation fault.
```

7.  What does `sizeof("Hello\0World")` return?

```c
// Your answer here
12
```

8.  What does `strlen("Hello\0World")` return?

```c
// Your answer here
5
```

9.  Give an example of X such that `sizeof(X)` is 3.

```c
// Your code here
the text "ab"
```

10. Give an example of Y such that `sizeof(Y)` might be 4 or 8 depending on the machine.

```c
// Your code here
int
```

### Chapter 3

Program arguments, environment variables, and working with character arrays (strings)

1.  What are two ways to find the length of `argv`?

2.  What does `argv[0]` represent?

3.  Where are the pointers to environment variables stored (on the stack, the heap, somewhere else)?

4.  On a machine where pointers are 8 bytes, and with the following code:

    ``` c
    char *ptr = "Hello";
    char array[] = "Hello";
    ```

    What are the values of `sizeof(ptr)` and `sizeof(array)`? Why?

```c
// Your answer here
1. Using the argc (number of arguments) or using a loop to count the number of non null entries in it.
2. argv[0] represents the execution name of the program.
3. At a high address above the stack i.e kernel.
4. sizeof(ptr) is 8 because it returns the size of a char pointer whereas sizeof(array) gives the size of the character array (i.e number of chars + 1 for the null char) which is 6.

```

5.  What data structure manages the lifetime of automatic variables?
   A Stack.

### Chapter 4

Heap and stack memory, and working with structs

1.  If I want to use data after the lifetime of the function it was created in ends, where should I put it? How do I put it there?
    Ans: On the heap, using malloc().

2.  What are the differences between heap and stack memory?
    Ans: heap memory is dynamic, stack is static stack memory has a high address whereas heap has a low address. Heap memory once allocated, must be deallocated to prevent memory leaks, whereas stack memory is freed up once the scope is over/the process ends. 

3.  Are there other kinds of memory in a process?
    Ans: apart from heap and stack memory there is the text segment that contains the code and the initialized variables and kernel memory.

4.  Fill in the blank: “In a good C program, for every malloc, there is a \_\_\_”.
    Ans: Consequent free and assigning the pointer to NULL to prevent a dangling pointer.

5.  What is one reason `malloc` can fail?
    Ans: If there isn't the required amount of memory available on the heap.

6.  What are some differences between `time()` and `ctime()`?
    Ans: time gives us the number of seconds since 1970-01-01 00:00:00 +0000 (UTC), whereas ctime() takes an argument of typedef time_t and transform it to broken-down time pr ASCII.

7.  What is wrong with this code snippet?

``` c
free(ptr);
free(ptr);
```
Ans: Freeing the same pointer twice, which could cause problems if the heap is using that memory for its own purposes.

8.  What is wrong with this code snippet?

``` c
free(ptr);
printf("%s\n", ptr);
```
Ans: Using a pointer to a memory that has been freed.

9.  How can one avoid the previous two mistakes?
Ans: after freeing the pointer setting it to NULL (`ptr = NULL;`) to avoid dangling pointers.

10. Use the following space for the next four questions

```c
#include <stdlib.h>
#include <string.h>
// 10
struct Person {
    char* name;
    int age;
    struct Person** friends;

};

typedef struct Person person_t;

// 12
person_t* create(char* name, int age) {
    person_t* new_ = (person_t*) malloc(sizeof(person_t));
    new_->name = malloc(sizeof(char)*30);
    strcpy(new_->name, name);
    new_->friends = malloc(sizeof(person_t*)*10);
    int i = 0;
    for(; i< 10; i++) {
        new_->friends[i] = NULL;
    }
    new_->age = age;
    return new_;
}
// 13
void destroy(person_t* person) {
    free(person->name);
    free(person->friends);
    int i = 0;
    for(; i<10; i++) {
        if(person->friends[i] != NULL)
            person->list[i] = NULL;
    }
    free(person);
}

int main() {
// 11
    person_t* person1 = (person_t*) malloc(sizeof(person_t));
    person_t* person2 = (person_t*) malloc(sizeof(person_t));
    strcpy(person1->name, "Agent Smith");
    strcpy(person2->name, "Sonny Moore");
    person1->age = 128; 
    person2->age = 256;
    person1->friends[0] = person2;
    person2->friends[0] = person1;

}
```

* Create a `struct` that represents a `Person`. Then make a `typedef`, so that `struct Person` can be replaced with a single word. A person should contain the following information: their name (a string), their age (an integer), and a list of their friends (stored as a pointer to an array of pointers to `Person`s). 

*  Now, make two persons on the heap, “Agent Smith” and “Sonny Moore”, who are 128 and 256 years old respectively and are friends with each other. Create functions to create and destroy a Person (Person’s and their names should live on the heap).

* `create()` should take a name and age. The name should be copied onto the heap. Use malloc to reserve sufficient memory for everyone having up to ten friends. Be sure initialize all fields (why?).

* `destroy()` should free up not only the memory of the person struct, but also free all of its attributes that are stored on the heap. Destroying one person should not destroy any others.


### Chapter 5

Text input and output and parsing using `getchar`, `gets`, and `getline`.

1.  What functions can be used for getting characters from `stdin` and writing them to `stdout`?
Ans:  getchar() and putchar()

2.  Name one issue with `gets()`.
Ans: If not carefully used it can cause a buffer overflow and corrupt variable values.

3.  Write code that parses the string “Hello 5 World” and initializes 3 variables to “Hello”, 5, and “World”.

```c
// Your code here
#include <stdio.h>
int main() {
    char* data = "Hello 5 World";
    char buffer1[20];
    char buffer2[20];
    int mid_num = 0;
    sscanf(data, "%s %d %s", buffer1, &mid_num, buffer2);
    return 0;
}
```

4.  What does one need to define before including `getline()`?
Ans: #define _GNU_SOURCE

5.  Write a C program to print out the content of a file line-by-line using `getline()`.

```c
// Your code here
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* file = fopen(argv[0], "r");
    if (file == NULL)
        exit(EXIT_FAILURE);


    char* buffer = NULL;
    size_t capacity = 0;
    ssize_t result;
    while ((result = getline(&buffer, &capacity, file)) != -1) {
        puts(buffer);
    }
    free(buffer);
    fclose(file);
    exit(EXIT_SUCCESS);
}
```

### C Development

These are general tips for compiling and developing using a compiler and git. Some web searches will be useful here


1.  What compiler flag is used to generate a debug build?

Ans:     -g

2.  You fix a problem in the Makefile and type `make` again. Explain why this may be insufficient to generate a new build.
Ans: `make` uses timestamps to check whether a target is up to date or not, and if the timestamp 
is recent enough make doesn't recompile that target which is why `make clean` should be used.

3.  Are tabs or spaces used to indent the commands after the rule in a Makefile?
Ans: Commands or Recipes are differentiated from the rest of makefile by the use of TABS for indentation.

4.  What does `git commit` do? What’s a `sha` in the context of git?
Ans: `git commit` records the changes to the repository, hence after it is recommended to 
routinely commit the changes to keep track of the versioning. sha refers to the SHA1 hash that is 
used by git to identify commits made to the repository.

5.  What does `git log` show you?
Ans: `git log` shows a history of all the commits in a chronological order that were made to the repository.

6.  What does `git status` tell you and how would the contents of `.gitignore` change its output?
Ans: `git status` shows all the files that have been modified and have/have not been staged for a 
commit. `.gitignore` contains a list of files that are to be ignored when tracking changes in the repositories.

7.  What does `git push` do? Why is it not just sufficient to commit with `git commit -m ’fixed all bugs’ `?
Ans: it sends all the changes necessary to bring the remote up to date with local repo. it is not just sufficient to commit because you want those changes to be reflected in the remote so that everyone else who is on the project can update their local repos to the new changes. 

8.  What does a non-fast-forward error `git push` reject mean? What is the most common way of dealing with this?
Ans: It means someone pushed new commits between the last `git fetch` and `git push`. In this case we need to fetch the new changes and rebase our current branch to reflect those changes before we can push our changes.


### Optional: Just for fun

-   Convert your a song lyrics into System Programming and C code covered in this wiki book and share on Piazza.

-   Find, in your opinion, the best and worst C code on the web and post the link to Piazza.

-   Write a short C program with a deliberate subtle C bug and post it on Piazza to see if others can spot your bug.

-   Do you have any cool/disastrous system programming bugs you’ve heard about? Feel free to share with your peers and the course staff on piazza.
