


## Score: 100/100 (100.00%)


### ✓ - [0/0] - Output from `make`

- **Points**: 0 / 0


```
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  tests/catchlib.cpp -o tests/catchlib.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  tests/unit_tests.cpp -o tests/unit_tests.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  tests/catch_main.cpp -o tests/catch_main.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  letter.cpp -o .objs/letter.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  room.cpp -o .objs/room.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  allocator.cpp -o .objs/allocator.o
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -MMD -MP -c  fileio.cpp -o .objs/fileio.o
clang++ tests/catchlib.o tests/unit_tests.o tests/catch_main.o .objs/letter.o .objs/room.o .objs/allocator.o .objs/fileio.o -std=c++1y -stdlib=libc++ -lpthread  -o test

```


### ✓ - [5/5] - Test the constructor (valgrind)

- **Points**: 5 / 5

```
==1190== Memcheck, a memory error detector
==1190== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1190== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==1190== Command: ./test -r xml "Test\ the\ constructor\ (valgrind)"
==1190== 
==1190== 
==1190== HEAP SUMMARY:
==1190==     in use at exit: 0 bytes in 0 blocks
==1190==   total heap usage: 2,522 allocs, 2,522 frees, 279,888 bytes allocated
==1190== 
==1190== All heap blocks were freed -- no leaks are possible
==1190== 
==1190== For counts of detected and suppressed errors, rerun with: -v
==1190== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```
```
<?xml version="1.0" encoding="UTF-8"?>
<Catch name="test">
  <Group name="test">
    <TestCase name="Test the constructor (valgrind)" tags="[valgrind][weight=5]" filename="tests/unit_tests.cpp" line="17">
      <OverallResult success="true"/>
    </TestCase>
    <OverallResults successes="0" failures="0" expectedFailures="0"/>
  </Group>
  <OverallResults successes="0" failures="0" expectedFailures="0"/>
</Catch>

```


### ✓ - [5/5] - Test allocate() (valgrind)

- **Points**: 5 / 5

```
==1220== Memcheck, a memory error detector
==1220== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1220== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==1220== Command: ./test -r xml "Test\ allocate()\ (valgrind)"
==1220== 
==1220== 
==1220== HEAP SUMMARY:
==1220==     in use at exit: 0 bytes in 0 blocks
==1220==   total heap usage: 2,522 allocs, 2,522 frees, 277,616 bytes allocated
==1220== 
==1220== All heap blocks were freed -- no leaks are possible
==1220== 
==1220== For counts of detected and suppressed errors, rerun with: -v
==1220== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```
```
<?xml version="1.0" encoding="UTF-8"?>
<Catch name="test">
  <Group name="test">
    <TestCase name="Test allocate() (valgrind)" tags="[valgrind][weight=5]" filename="tests/unit_tests.cpp" line="24">
      <OverallResult success="true"/>
    </TestCase>
    <OverallResults successes="0" failures="0" expectedFailures="0"/>
  </Group>
  <OverallResults successes="0" failures="0" expectedFailures="0"/>
</Catch>

```


### ✓ - [20/20] - Test printRooms() (valgrind)

- **Points**: 20 / 20

```
==1232== Memcheck, a memory error detector
==1232== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1232== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==1232== Command: ./test -r xml "Test\ printRooms()\ (valgrind)"
==1232== 
==1232== 
==1232== HEAP SUMMARY:
==1232==     in use at exit: 0 bytes in 0 blocks
==1232==   total heap usage: 2,526 allocs, 2,526 frees, 278,336 bytes allocated
==1232== 
==1232== All heap blocks were freed -- no leaks are possible
==1232== 
==1232== For counts of detected and suppressed errors, rerun with: -v
==1232== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```
```
<?xml version="1.0" encoding="UTF-8"?>
<Catch name="test">
  <Group name="test">
    <TestCase name="Test printRooms() (valgrind)" tags="[valgrind][weight=20]" filename="tests/unit_tests.cpp" line="32">
      <OverallResult success="true"/>
    </TestCase>
    <OverallResults successes="0" failures="0" expectedFailures="0"/>
  </Group>
  <OverallResults successes="0" failures="0" expectedFailures="0"/>
</Catch>

```


### ✓ - [30/30] - Test printRooms() for overall output

- **Points**: 30 / 30





### ✓ - [40/40] - Test printRooms() for individual output

- **Points**: 40 / 40





---

This report was generated for **satwiks2** using **246f0dedc4169573bf068613dd210f55b83a4fc7** (from **September 17th 2018, 1:00:00 am**)
