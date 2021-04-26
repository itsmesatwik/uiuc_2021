


## Score: 0/0 (0.00%)


### ✗ - [0/0] - Output from `make`

- **Points**: 0 / 0

```
tests/basic.cpp:114:5: warning: delete called on 'Drawable' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
    delete truck;
    ^
tests/basic.cpp:134:5: warning: delete called on 'Drawable' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
    delete flower;
    ^
2 warnings generated.
shape.o: In function `Shape':
/tmp/tmp-568xc6wO0LgXZOh/shape.cpp:20: undefined reference to `vtable for Shape'
/tmp/tmp-568xc6wO0LgXZOh/shape.cpp:26: undefined reference to `vtable for Shape'
truck.o: In function `Shape':
/tmp/tmp-568xc6wO0LgXZOh/./shape.h:19: undefined reference to `vtable for Shape'
rectangle.o:(.rodata+0x60): undefined reference to `typeinfo for Shape'
triangle.o:(.rodata+0x60): undefined reference to `typeinfo for Shape'
circle.o:(.rodata+0x60): undefined reference to `typeinfo for Shape'
clang-3.5.real: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [test] Error 1

```
```
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o cs225/PNG.o cs225/PNG.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o cs225/lodepng/lodepng.o cs225/lodepng/lodepng.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o cs225/HSLAPixel.o cs225/HSLAPixel.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o vector2.o vector2.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o line.o line.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o shape.o shape.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o rectangle.o rectangle.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o triangle.o triangle.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o circle.o circle.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o truck.o truck.cpp
clang++ -std=c++1y -stdlib=libc++ -g -O0 -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-delete-non-virtual-dtor -MMD -MP -c   -c -o flower.o flower.cpp
clang++ -std=c++1y -stdlib=libc++ -lpthread tests/basic.cpp cs225/PNG.o cs225/lodepng/lodepng.o cs225/HSLAPixel.o vector2.o line.o shape.o rectangle.o triangle.o circle.o truck.o flower.o -o test

```


---

This report was generated for **satwiks2** using **ea96a5faf71298998d77131ffd2c2751a87b9067** (from **September 24th 2018, 1:00:00 am**)
