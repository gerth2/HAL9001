#######################################################################################################################
#######################################################################################################################
### HAL9001 Rubix Cube Solving Robot - main.py
### Chris Gerth - Summer 2014
###
### Description: Main entry point for solver robot
###
#######################################################################################################################
#######################################################################################################################

import os
import sys
import subprocess
import threading
import time
import platform

#timekeeping functions, which run in a separate thread on a separate processor (gotta use multicores, man!)
total_time = 0.00
timer_process_continue_flag = 0;

class time_keeper_thread (threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
    def run(self):
        print 'Timer started!'
        total_time = 0.00
        start_time = time.clock()
        while(timer_process_continue_flag == 1):
            time.sleep(0.3) #sleep at least for a bit so that we don't flood the displays with updates
            total_time = time.clock() - start_time;
            temp_str = "Seconds Elapsed: " + str(total_time)
            print temp_str
            sys.stdout.flush() #flush standard out buffer to ensure things actually get printed, sometimes doesn't in cygwin
        total_time = time.clock() - start_time;
        print 'Timer stoped.'
        print "Total time to solve: " + str(total_time)

#######################################################################################################################
####START OF MAIN FUNCTION
#######################################################################################################################

print '====================================================================='
print '====================================================================='
print '===               HAL9001 RUBIK\'S CUBE SOLVING ROBOT              ==='
print '===                                                               ==='
print '===                    Hardware: Matt Birkel                      ==='
print '===                    Software: Chris Gerth                      ==='
print '===                        Summer 2014                            ==='
print '====================================================================='
print '====================================================================='
print ''
print 'Running Initalization...'
sys.stdout.flush()

### Initialize code

#set up temp working directory if not there yet
if not os.path.exists('temp/'):
    os.makedirs('temp/')

#set up GPIOs

#set up motor control

#set up I2C display (?? maybe ??)

#initialize motor configuration to load position



print 'Done Initalizing!'
print ' '
sys.stdout.flush()

### Main loop
exit_flag = 0
while(exit_flag == 0):
    #wait for keypress (substitute with GPIO giant button of "SOLVE! awesomeness" eventually)
    print '!!!LOAD CUBE NOW!!!'
    print 'Enter 1 to solve, 0 to quit: '
    sys.stdout.flush() #flush standard out buffer to ensure things actually get printed, sometimes doesn't in cygwin
    response = int(raw_input())
    if(response == 0):
        exit_flag = 1;
        break;
    
    #move from load position to home position
    print 'Setting up cube...'
    sys.stdout.flush()
    
    #start timer
    timer = time_keeper_thread(1, "Timing Thread")
    timer_process_continue_flag = 1
    timer.start();
    
    #get initial state. outputs to temp/ folder.
    print 'Determining Cube Initial State...'
    sys.stdout.flush()
    if(platform.system() == 'Windows'): #cross-platform compatibility
        if(os.system("python scripts\\gen_init_state.py")):
            timer_process_continue_flag = 0
            timer.join()
            print 'ERROR running gen_init_state.py'
            sys.exit(-1)
    else:
        if(os.system("python scripts/gen_init_state.py")):
            timer_process_continue_flag = 0
            timer.join()
            print 'ERROR running gen_init_state.py'
            sys.exit(-1)
    sys.stdout.flush()
    
    

    
    #report solution initializing phase complete
    
    #generate a solution. outputs to temp/ folder.
    print 'Generating Solution...'
    if(platform.system() == 'Windows'): #cross-platform compatibility
        if(os.system("bin\\solve_V2.exe")): 
            timer_process_continue_flag = 0
            timer.join()
            print 'ERROR running solution .exe file'
            sys.exit(-2)
    else:
        if(os.system("./bin/solve_V2.a")):
            timer_process_continue_flag = 0
            timer.join()
            print 'ERROR running solution .exe file'
            sys.exit(-2)
    sys.stdout.flush()
    
    
    #report solution analysis phase complete
    
    #manipulate the cube
    print 'Executing Solution...'
    sys.stdout.flush()
    
    #report cube solved
    
    print 'Done solving Cube!'
    sys.stdout.flush()
    #stop the timer
    timer_process_continue_flag = 0
    timer.join()
    
    #move cube back to load position (so the client may bask in the glory of the solved cube)
    print 'Returning cube to loading position...'
    sys.stdout.flush()
    
    
#tear down any data structures, shut down the robot in a safe state



print 'Exiting...'
sys.exit(0)