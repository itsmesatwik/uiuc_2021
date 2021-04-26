


## Score: 13/13 (100.00%)


### ✓ - [0/0] - Output from &#x60;make&#x60;

- **Points**: 0 / 0


```clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic main.cpp mp1.cpp
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/PNG.cpp
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/HSLAPixel.cpp
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/lodepng/lodepng.cpp
clang++ mp1.o PNG.o HSLAPixel.o lodepng.o main.o -std&#x3D;c++1y -stdlib&#x3D;libc++ -lpthread -lm -o mp1
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic tests/unit_tests.cpp
clang++ unit_tests.o PNG.o HSLAPixel.o lodepng.o mp1.o -std&#x3D;c++1y -stdlib&#x3D;libc++ -lpthread -lm -o test
./mp1 800
Calling myArt(800, 800) and saving it as &#x60;art.png&#x60;.
```


### ✓ - [1/1] - HSLAPixel&#x27;s default constructor creates a white pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel&#x27;s default constructor creates an opaque pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel&#x27;s HSL constructor creates a pixel with provided HSL values

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel&#x27;s HSL constructor creates an opaque pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel&#x27;s HSLA constructor creates a pixel with provided HSLA values

- **Points**: 1 / 1





### ✓ - [1/1] - Rotate in_01.png

- **Points**: 1 / 1





### ✓ - [1/1] - Rotate in_02.png

- **Points**: 1 / 1





### ✓ - [1/1] - Rotate in_03.png

- **Points**: 1 / 1





### ✓ - [5/5] - Creative artwork contains at least three unique colors

- **Points**: 5 / 5





---

This report was generated for **satwiks2** using **7ecfe504ae76e4d155871febda67f25751cadc93** (from **Invalid date**)
