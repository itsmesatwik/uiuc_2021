############### R Language

# Let's write R Code!
# The first piece of code, we just wrote, is a comment.

# Comments are prefixed on a line by the pound sign #

############### Explaining RStudio Windows

# (Top-Left) is the home of the program editor for writing RScripts

# (Bottom-Left) is where commands are submitted to console.

# (Top-Right)
#   Environment listing active objects in memory
#   History shows a list of executed R commands
#   Connections provides access to databases
#   Build tools to create packages
#   Git allows for version control

# (Bottom-Right)
#   Mini file manager for project files.
#   Plots containing rendered graphs
#   Packages list installed libraries
#   Help displays command documentation
#   Viewer is an active HTML/Shiny previewer

############### Evaluating Expressions

1 + 1

10/2

sqrt(4)

2^3

abs(-5)

# Trignometric functions in R 
# use radians not degrees as input
cos(pi)

# Can we do the rest?

############### Evaluation of Expressions

42 + 13       # Expression
# [1] 55      # Output minus the #

############### Naming Values with Variables

age = 42     # let age be 42

age          # Show value of age

# Each variable can be bound to 1 expression

age = 36     # Let age now be 36

age          # Show value of age

# age now has the value of 36 instead of 42


# Instead of the "Run" button to run code, use a keyboard shortcut 
# 
# MacOS: CMD + Enter
# Windows: Cntrl + Enter
#
# Typically, will be shown with: Cntrl / Cmd + Enter

############### Using Variable Names

# Variables names on the left (←) of = mean assign.

# Let weight be 210
weight = 210

weight

# Variables names on the right (→) of = mean use this as a value (e.g. weight).

# Let weight_after_lunch be weight plus 5
weight_after_lunch = weight + 5

weight_after_lunch

# Let diet_weight be weight minus 10
diet_weight = weight - 10

diet_weight

############### Variable Names as Case Sensitive

# Number of Students
n_students = 97
n_students

# If we capitalize `n` and `s` in `n_student` to get `N_Student`,
# R won't know where the variable is!

N_Students
# Error: object 'N_Students' not found

############### Name your variables!

# What does this mean?
a = 3
b = 10.5
d = a*b

# Summer Time Job
hours = 3
pay_rate_per_hour = 10.5
takehome_pay = hours * pay_rate_per_hour
takehome_pay

############### Creating a function

2 + 5

x = 1

y = x

f = function(x) {
  x^2
}

f(2)

linear_eq = function(m, b, x) {
  y = m*x + b
  
  y
}

linear_eq(m = 1, b = 0, x = 3)

# Create a collection filled with a range of values
# from 1 to 9
x = 1:9

# Compute the responses for a linear line from 1 to 9
linear_eq(m = 1, b = 0, x = x)

y = linear_eq(m = 2, b = 0, x = x)

plot(x, y)

############### Environments

# In the upper-right of RStudio notice that
# the environment pane now contains x
# under the "global environment"

# Variables in global environments
# can be retrieved with
ls()

############### Order of Operations

# The `add` function from the lecture slides
# with the addition of `message()`, which
# outputs text to the console (lower left)
add = function(a, b) {
  message("R reached add")
  summed = a + b
  return(summed)
}

# Subtraction function
subtract = function(a, b) {
  message("R reached subtract")
  subtraction = a - b
  return(subtraction)
}

# Multiplication function
multiply = function(a, b) {
  message("R reached multiply")
  times = a*b
  return(times)
}

# Expression with built-in operators
2 * (4 + 5) - (1 + 1)

# Translation of 2 * (4 + 5) - (1 + 1)
# into expressionary form
subtract(multiply(2, add(4, 5)), add(1, 1))

# Notice the output shows when a function is reached.
# This is notably different than how a human evaluates
# mathematics.

message("Hi!")

multiply(2, add(1, 1))
