1. Program Name:
Exercise 3 - OOP 2

2. Program Partners:
Meytal Abrahamian, 211369939
Bar Oren, 206548752

3. General Explanation:
This exercise is based on the solution of Exercise 1. In this exercise an abnormality 
treatment is used, thus it is resistant to mistakes of careless users. The change 
includes improvement of the execution of existing commands to throw errors in case 
of incorrect input. Also added is the option to read commands from a given file and 
execute them as well as limits to the number of commands that can be saved.

4. Explanation Of Program Design:
We changed the files: SetCalculator.h, SetCalculator.cpp and main.cpp.
The changes were adding execution handling cases where we were required to 
check input from the user or file and other errors that might appear.

5. Program Files:

include:
	- BinaryOperation.h
	- CMakeLists.txt
	- Comp.h
	- Difference.h
	- Identity.h
	- Intersection.h
	- NameGenerator.h
	- NonCompBinaryOperation.h
	- Operation.h
	- Product.h
	- Set.h
	- SetCalculator.h
	- Union.h

src:
	- BinaryOperation.cpp
	- CMakeLists.txt
	- Comp.cpp
	- Difference.cpp 
	- Identity.cpp
	- Intersection.cpp
	- main.cpp
	- NameGenerator.cpp
	- NonCompBinaryOperation.cpp
	- Operation.cpp
	- Product.cpp
	- Set.cpp
	- SetCalculator.cpp
	- Union.cpp

README.txt

6. Main Data Bases

7. Notable Algorithms

8. Known Bugs

9. Other Comments
- In order for the commands to work properly, all the data they must 
  receive in the same line must be recorded

