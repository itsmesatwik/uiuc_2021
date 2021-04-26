


## Score: 10/10 (100.00%)


### ✓ - [0/0] - Output from &#x60;make&#x60;

- **Points**: 0 / 0


```clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic sketchify.cpp -o sketchify.o
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/HSLAPixel.cpp -o cs225/HSLAPixel.o
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/PNG.cpp -o cs225/PNG.o
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/lodepng/lodepng.cpp -o cs225/lodepng/lodepng.o
clang++ -std&#x3D;c++1y -stdlib&#x3D;libc++ -c -g -O0 -Wall -Wextra -pedantic   -c -o tests/unit_tests.o tests/unit_tests.cpp
clang++ sketchify.o cs225/HSLAPixel.o cs225/PNG.o cs225/lodepng/lodepng.o tests/unit_tests.o -std&#x3D;c++1y -stdlib&#x3D;libc++ -lpthread -lm -o test
```


### ✓ - [2/2] - Runs without a seg fault

- **Points**: 2 / 2





### ✓ - [1/1] - Produces an output PNG file

- **Points**: 1 / 1





### ✓ - [2/2] - Pixels that are part of the sketch are your favorite color

- **Points**: 2 / 2





### ✓ - [1/1] - Pixels that are NOT part of the sketch are white

- **Points**: 1 / 1





### ✓ - [2/2] - in_01.png produces a correct sketch

- **Points**: 2 / 2





### ✓ - [1/1] - in_02.png produces a correct sketch

- **Points**: 1 / 1





### ✓ - [1/1] - in_03.png produces a correct sketch

- **Points**: 1 / 1





---

This report was generated for **satwiks2** using **3a5318e2083847cd87444f591694dfe28c553ff6** (from **September 12th 2018, 1:00:00 am**)
