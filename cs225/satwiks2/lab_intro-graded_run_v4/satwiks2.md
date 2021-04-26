


## Score: 8/8 (100.00%)


### ✓ - [0/0] - Output from `make`

- **Points**: 0 / 0

```
lab_intro.cpp:59:23: warning: comparison of integers of different signs: 'int' and 'unsigned int' [-Wsign-compare]
    for (int x = 0; x < image.width(); x++) {
                    ~ ^ ~~~~~~~~~~~~~
lab_intro.cpp:61:27: warning: comparison of integers of different signs: 'int' and 'unsigned int' [-Wsign-compare]
        for (int y = 0; y < image.height(); y++) {
                        ~ ^ ~~~~~~~~~~~~~~
2 warnings generated.

```
```
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic tests/catchmain.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic tests/part1.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic tests/part2.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/PNG.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/HSLAPixel.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/lodepng/lodepng.cpp
clang++  -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic lab_intro.cpp
clang++ catchmain.o part1.o part2.o PNG.o HSLAPixel.o lodepng.o lab_intro.o -std=c++1y -stdlib=libc++ -lpthread -lm -o test

```


### ✓ - [1/1] - HSLAPixel's default constructor creates a white pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel's default constructor creates an opaque pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel's HSL constructor creates a pixel with provided HSL values

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel's HSL constructor creates an opaque pixel

- **Points**: 1 / 1





### ✓ - [1/1] - HSLAPixel's HSLA constructor creates a pixel with provided HSLA values

- **Points**: 1 / 1





### ✓ - [1/1] - createSpotlight

- **Points**: 1 / 1





### ✓ - [1/1] - illinify

- **Points**: 1 / 1





### ✓ - [1/1] - watermark

- **Points**: 1 / 1





---

This report was generated for **satwiks2** using **3ffc2217b493ccaa5575390b4c0d7c7c2202c4bb** (from **September 3rd 2018, 12:15:00 am**)
