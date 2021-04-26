*insert the travis-ci badge here*

# hw02: Data Sphinx!

The objective behind this homework assignment is to give graded practice working
with reading data, summarizing data, vectorization, writing functions, and using R packages.

## Overview

Please use the `hw02-assign.Rmd` document as a template for your assignment.
**Note:** You must _rename_ `hw02-assign.Rmd` to `hw02-netid.Rmd`, where `NetID`
is your `NetID` infront of `NetID@illinois.edu`. 

Make sure to commit your _rendered_ files (e.g. `hw02-netid.html` or `hw02-netid.pdf`) 
into this directory as well. 

## Files Included

### Common files

- `README.md`: a file that provides guidance as to what the assignment is and 
  what is contained inside the repository. 
    - You are reading this file right now!
- `hw02-assign.Rmd`: the RMarkdown file containing the homework assignment.
- `.gitignore`: an optional file that _RStudio_ may generate, which prevents 
  certain files from being **committed** into the repository as they have no value.
- `<REPO-NAME>.Rproj`: an optional file that is generated when an _RStudio_ 
  project is created. As we are using RStudio Cloud, this file will likely be
  generated on import. 

### Infrastructure for Testing

- `.travis.yml`: provides a foundation for performing 
   [continuous integration](https://travis-ci.com) checks, or verify that we
   can render/create output from your RMarkdown file. If your RMarkdown file 
   is correct, then "Build Status" badge that should be inserted inside this
   `README` will be green and show say "build success." Otherwise, the badge 
   will be red to indicate that a "build failure" occurred. Build failures 
   occur if: 
    - Image files have not been committed or the path to the image file is
      inappropriate. _Hint:_ Use relative paths!
    - You added an additional package that was not listed in the `DESCRIPTION` 
      file and, thus, is not available when travis tries to build the `.Rmd` file.
- `DESCRIPTION`: a file associated with _R Packages_ that indicates 
   to **Travis** what dependencies must be installed in order to build the `.Rmd` files.
- `test/render_rmds.R`: a short _R_ script that finds all instances of
   the `.Rmd` files and attempts to build them! 

# Acknowledgement

The testing apparatus (e.g. `/test`) and portions of the README were based off
of an example done by [Carl Boettiger](http://carlboettiger.info).

