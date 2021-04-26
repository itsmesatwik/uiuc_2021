


## Score: 10/10 (100.00%)


### ✓ - [0/0] - Output from `make`

- **Points**: 0 / 0


```
clang++ -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic sketchify.cpp -o sketchify.o
clang++ -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/HSLAPixel.cpp -o cs225/HSLAPixel.o
clang++ -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/PNG.cpp -o cs225/PNG.o
clang++ -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic cs225/lodepng/lodepng.cpp -o cs225/lodepng/lodepng.o
clang++ -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic   -c -o tests/unit_tests.o tests/unit_tests.cpp
clang++ sketchify.o cs225/HSLAPixel.o cs225/PNG.o cs225/lodepng/lodepng.o tests/unit_tests.o -std=c++1y -stdlib=libc++ -lpthread -lm -o test

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

This report was generated for **satwiks2** using **1e71ac9db09f763e8cbb4e9e8119369baf7a7c20** (from **September 10th 2018, 12:15:00 am**)
