


## Score: 8/8 (100.00%)


### ✓ - [0/0] - Output from &#x60;make&#x60;

- **Points**: 0 / 0

```lab_intro.cpp:59:23: warning: comparison of integers of different signs: &#x27;int&#x27; and &#x27;unsigned int&#x27; [-Wsign-compare]
    for (int x &#x3D; 0; x &lt; image.width(); x++) {
                    ~ ^ ~~~~~~~~~~~~~
lab_intro.cpp:61:27: warning: comparison of integers of different signs: &#x27;int&#x27; and &#x27;unsigned int&#x27; [-Wsign-compare]
        for (int y &#x3D; 0; y &lt; image.height(); y++) {
                        ~ ^ ~~~~~~~~~~~~~~
2 warnings generated.
```
```clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic tests/catchmain.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic tests/part1.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic tests/part2.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/PNG.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/HSLAPixel.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/lodepng/lodepng.cpp
clang++  -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic lab_intro.cpp
clang++ catchmain.o part1.o part2.o PNG.o HSLAPixel.o lodepng.o lab_intro.o -std&#x3D;c++1y -stdlib&#x3D;libc++ -lpthread -lm -o test
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





### ✓ - [1/1] - createSpotlight

- **Points**: 1 / 1





### ✓ - [1/1] - illinify

- **Points**: 1 / 1





### ✓ - [1/1] - watermark

- **Points**: 1 / 1





---

This report was generated for **satwiks2** using **3a5318e2083847cd87444f591694dfe28c553ff6** (from **September 12th 2018, 1:00:00 am**)
