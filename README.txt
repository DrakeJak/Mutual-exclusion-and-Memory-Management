	Student Information
=============================
Name: Arvids Jakobsons
Student ID: 0953757
Assignment #3


	Program Information
==============================

PART1:
Implementing a solution to the dining philosopher's problem. program takes 2 arguments, the first is the number of philisophers and the second is the amount of times each one eats. I
Implemented it using semaphores and mutexes. I took the way we learned in class and made functions based on that. the function philosopher will run each of the other functions until the amount of times eaten is met.


PART2:
Program holes.c is a Simulation of how memory works for processes. the file works by taking in a file name from argv[1] and parsing the processes into a linked list. If any errors occur in the file 
parsing process the program won't run. Once the file is parsed the program will begin and run though the simulation. It starts with FirstFit then BestFit,WorstFit, and then NextFit. When using the linked list for each I have a copy of the linked list made so that I will never affect each simulation. This will allow for me to run the simulations as seperate functions. Then once all the simulations are finished the list is deleted to free the memory. when using the ready list it's distributed to a running list, which goes back to the ready list once new processes need to be entered.

		Test Cases
==============================
part1:
make the file and
run as follows:
./run

$dine 3 2


part2:
make the file and
run as follows:
./run test.txt 
where test is the text file that you want to use

		Assumptions
==============================
-memory is 128mb
-when doing the next fit simulation it will loop before freeing memory
-allowed to do each fit as its own function


		limitations
==============================
part1:
sometimes the program will continue past the eating number with thinking and picking up the forks, but they wont eat again so it may not matter.


part2:
-Some of the decimal numbers are slightly off by 0.000001 to 0.000005 sometimes, Irenaeus Chan said it did not matter as long as there wasn't a whole percentage difference in the answer.

-also for the printing of the ID's, I could not get the first one to print out so I left it blank.
