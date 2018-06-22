cs4300ga
string size 50 = 82
string size 20 = 15

GaDriver houses most of the logic, Person is only used to abstract the bitstring into a
managble object.

There are 3 log files, master houses the general information, population houses a printout
of each person per generation along with the bit string, mutation holds a record of all
of the mutations that occured.  
#OpenMP Warning
I did use OpenMP to speed up the generation so that all 5
iterations of the for loop run at once.  If that will not work for your system it is on 
line 24 of main.cpp.