
First Section

1.CSE 532 Fall 2013 Lab 1
 
2. Names of the Team Members:
Amit Jha	   ajha@wustl.edu
Li Zeng          zengli@wustl.edu
Di Yan            diyan@wustl.edu


3. Overview of how the program was designed

As per the lab instructions we have created two classes - Player and Play.
The main program starts which is similar to previous lab, however now the thread management logic has moved to Player objects which are constructed from main function. 

 The main function was designed for the following steps --
			a.It reads  and validates the correctness of the configuration file provided and exits the program if the input is incorrect.
            b. Constructs the Play object with play name provided in the input.
            c. Iterates over the content of the configuration file and populates vectors for character names and fstream of the play fragment files.
            d. Once the iteration over input file is done, Play objects are initialized and pushed into a vector. The arguments for the player object constructor is taken from the vectors that was populated with data read from the configuration file (name of characters and streams associated with the file names).
			e. Post initialization of Play objects the main function invokes enter and exit functions on each of the objects in a separate loop.

The Player class was designed with following steps -
           a. It has private instance variables which get assigned values from the constructor. It has instance variable for Play object, name of the character and stream of the file from which lines of play are to be read. Apart from that it has internal vector to store the lines of play being read and the internal thread object which does the actual processing.
		   b. The constructors initialize the instance variables with the passed arguments. The default constructor has been overloaded.
           c. The read function reads lines from the input stream file sequentially and after validation for the correctness of the line read, it either pushes the line to the vector or in case of wrongly formatted line ignores it and continues on to next line. Once the reading from the file is done; the vector holding the lines it sorted using using the local struct which compares the line number for sorting (similar to lab0).
           d. The act function iterates over the vector holding the lines and invokes recite function on Play object. The logic for waiting on condition variable is encapsulated in the recite function of the Play class.

           e. The enter function (which is invoked from main) launches a new thread which basically calls read and act function on the object. Once launched, move semantics is used to have the ownership of the thread transferred to thread instance variable.
		   f. The exit function basically joins with the thread that was spawned.

The Play class has been redesigned from the previous lab as per instructions in the lab. It still has instance variable which holds the play name which gets passed to it through constructor from main function. 
Below are the design steps for Play class --
			a. In the constructor it initializes the play name as well as counter that is used for printing line (in recite function) to 1.
			b. It still has a variable to keep track of current character. It also has instance variable for the mutex and condition variable which using the mutex for locking and synchronization. 
			c. The recite function (invoked from act in Player objects) takes the iterator of lines read as input argument. It takes over the mutex validates that the line number on the iterator is valid (i.e. counter is not greater than line number, if yes error).  The condition variable waits for the lock as well as the condition that the counter reaches the line number that is being held by the iterator. Once the condition becomes true, it checks the character name with the current character whose lines are being printed and prints new character’s name if its different. After this check he actual line of the play is printed.

4. Wrapper Facades Used
 	The whole design has become more modular with the Lab. We are still using str_op class the wrapping common string utility function. Apart from that now the Player and Play classes also using wrapper facade to handle the complexity internally i.e thread management and condition variable usage for synchronization. The whole design is much cleaner now as the Player and Play classes emulate closer to real world object oriented design.  

	Based on the feedback from lab 0, we have also implemented refactoring so that classes and implementations are moved to separate source and header files for modular design and expression. 

5. Insights

   We did the similar testing as Lab0 to test both happy path and not so ‘happy path’ for this lab as well. We think the design is much better now, and addresses a concern that we noted in lab0, i.e. the threads read their own files independently and only after they are done with the preparation of data, they invoke ‘act’, which is basically the play where the shared resource is being accessed. This design is much better where we achieve concurrency better as well as put the lock and synchronization only where we really need it; i.e. while doing recite to output the data to the console.
   We have an open issue with this lab where if a line in one the files has a line number which is non sequential (i.e. one of the files has a line with line number as 60 while the overall play length is 47), the program waits forever for the counter to reach that line number.

	Based on the feedback from previous lab assignment we have eliminated use of hard coded constants from most of the places and are using enums instead. The scramble portion for the lab was also implemented and we used the output files generated with the scrambled option to test the main program and it worked well.

	We used the following link to take an example of random shuffling of elements in a vector. This was used in the extra credit portion of the lab to randomly shuffle the lines in a file and write it back to same file.
	http://en.cppreference.com/w/cpp/algorithm/random_shuffle

Second Section

1. Download the zip file (CSE532SLab1.zip).
2. Unzip on the local drive(This should create a folder named CSE532SLab1)
3. Double click on file CSE532SLab1\Lab1\Lab1.sln to open Microsoft Visual Studio 2012 with this project.
4. In Microsoft Visual Studio 2012 you can build the solution using build solution option (F7) once the solution has been opened.
5. Below line shows an example execution of the program from command line. This assumes that the input files and configuration files are in the same directory where the executable is. If they reside in some different folder their absolute path should be given as command line parameter -

H:\Downloads\CSE532SLab1\Lab1\Debug>Lab1.exe hamlet_ii_2_config.txt


For the extra portion of the lab --

1. Download the zip file (Lab1_Extra.zip)
2. Unzip on the local drive(This should create a folder named Lab1_Extra)
3. Double click on file Lab1_Extra\Lab1_Extra.sln to open Microsoft Visual Studio 2012.
4. In Microsoft Visual Studio 2012 you can build the solution using build solution option (F7) once the solution has been opened.
5. Below line shows an example execution of the program from command line. This assumes that the input files and configuration files are in the same directory where the executable is. If they reside in some different folder their absolute path should be given as command line parameter -

C:\temp\Lab1_Extra\Debug>Lab1_Extra.exe hamlet_act_ii_scene_2.txt amitconfig This is my play

With the scramble option 

C:\temp\Lab1_Extra\Debug>Lab1_Extra.exe SCRAMBLE hamlet_act_ii_scene_2.txt amitconfig This is my play



Third Section:

Main Part:
We did many negative tests on the program by giving wrong kind of input. This was similar to the kind of testing we did in Lab0.  We tested the program by giving it empty configuration file to make sure it exits giving error. We also tested the program by giving input wrongly formatted lines in the configuration files as well as the fragment files. We also tested the situation where one of the fragment files does not exist while others do and see how the program behaves.
One of the situation where program did not perform correctly was the case where one of the fragment file contains a line number which is non sequential (line 60 while the last line before that in any of the files in line 47). This is a known bug. We did not know what should be the expected behaviour was to be in this case.

Extra Part:
The test for the extra portion was also similar to the test that we did last time. The scramble option was tested as well. Both the output files are provided with the project deliverables.
