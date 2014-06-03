=======================================================================================================================
=======================================================================================================================
===                                     HAL 9001 - A Rubix Cube Solving Robot
===                                     =====================================
===                                          Project Lead - Matt Birkel
===                                         Software Lead - Chris Gerth
===                                              Summer-Fall 2014
===                                     =====================================
===                                           READ THE LICENSE FILE!!!!!            
=======================================================================================================================
=======================================================================================================================

This Repository contains the source code for the System.

The robot is based on a Raspberry Pi system with the camera module.

Version 1 solution:
The solution algorithm is heavily based on Richard E. Korf's  "Finding Optimal Solutions to Rubik's Cube Using Pattern 
Databases" (1997), with modifications to the heuristic function to match our hardware capabilities. The paper can be 
found at http://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf  .

Version 2 solution:
The solution algorithm comes from Jaap Scherphuis, as part of a competition. It averaged around 17 moves per solution,
and runs in under .5 seconds.
Cube Contest Code source: http://tomas.rokicki.com/cubecontest/winners.html
Thistlethwaite's Algorithm : http://en.wikipedia.org/wiki/Morwen_Thistlethwaite#Thistlethwaite.27s_algorithm
Result testing online: http://tomas.rokicki.com/cgi-bin/cube-gen.pl

=======================================================================================================================
Install Prerequisites (In this order):

Windows 7 x64:
-presumably you already have git if you're reading this.
-cygwin (for gcc/make - be sure these are checked when installing) (https://www.cygwin.com/setup-x86_64.exe)
-python 2.7 x86 VERSION ONLY (https://www.python.org/ftp/python/2.7/python-2.7.msi)
--will have to add this to cygwin's path by putting "export PATH=$PATH:/cygdrive/c/Python27/" into the <cygwin install>/home/<user>/.bashrc file.
-PIL (Python Imaging Library) - (http://effbot.org/media/downloads/PIL-1.1.7.win32-py2.7.exe)


=======================================================================================================================

The main python script should be run as part of the Pi's boot process to start the robot.
The script will initialize all the required hardware and wait for the start button to be pushed.
Once started, the script will run python code to rotate the cube and capture image files of each face.
That same Python script will do basic analyses on each image to determine which face it is, and what the initial
state of the cube is. This state is placed in a text file.
C code for the tree search to determine the solution steps is included, and can be compiled into the solve.exe 
executable with the command "make".
The C code generates another text file containing the solution moves. This is used by manipulate.py to figure out 
how to move the servos and solve the cube.

=======================================================================================================================

Key Files:

main.py - Initializes the hardware, waits for the start command, and then solves the cube. Performs all peripheral
    hardware interfacing except servos (network? bluetooth? timer display? features unknown as of now...)

scripts/get_init_state.py - captures images of all six faces of the cube, and generates a .txt file with the initial state
    of the scrambled cube.

bin/solve_V1.exe - high-speed executable to take the initial cube state and generate a series of face rotations required to
    solve the scrambled cube. Outputs to text file.
    Source: solve.c/.h - main algorithm for searching for solution
            structures.h - header with the data structures used to represent the cube states
            moves.c/.h - functions that describe the result of a face rotation
            heuristic.h - heuristic function lookup table

bin/solve_V2.exe - high-speed executable to take the initial cube state and generate a series of face rotations required to
    solve the scrambled cube. Outputs to text file. 
    Source: jaap_sol.c - Jaap Scherphuis's solution, with added comments and ported to C
            
scripts/manipulate.py - Turns output from solve.exe into a series of movements for the hardware to execute to solve the cube.

Support files:
gen_heuristic.exe (.c/.h) - starts with a solved cube, performs virtual moves on each face, records each resulting
    configuration along with an integer cost from that node to the root. These are used to create the heuristic.h text
    file, which is compiled into the solver algorithm.