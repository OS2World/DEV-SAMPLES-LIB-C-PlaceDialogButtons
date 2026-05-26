# nmake makefile
#
# Tools used:
#   Compile::GNU C (GCC 9)
#   Linker:: GCC with OMF output
#   Make: nmake or GNU make

all : btnplc.dll

btnplc.dll : btnplc.obj btnplc.def
	gcc -Zomf -Zdll btnplc.obj btnplc.def -o btnplc.dll

btnplc.obj : btnplc.c 
	gcc -Wall -Zomf -Zdll -c -O2 btnplc.c -o btnplc.obj

clean :
	rm -f *.dll *.exe *.RES *.obj
