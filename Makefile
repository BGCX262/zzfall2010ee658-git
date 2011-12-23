fpgaTest.exe: Command.o Node.o atpg.o fileRead.o fileWrite.o global.o logicSim.o main.o preprocess.o
	g++ Command.o Node.o atpg.o fileRead.o fileWrite.o global.o logicSim.o main.o preprocess.o -o fpgaTest.exe
Command.o: Command.cpp Command.h
	g++ -c Command.cpp
Node.o: Node.cpp Node.h atpg.h
	g++ -c Node.cpp
atpg.o: atpg.cpp Node.h Command.h global.h type.h preprocess.h atpg.h fileRead.h fileWrite.h
	g++ -c atpg.cpp
fileRead.o: fileRead.cpp Node.h Command.h global.h type.h fileRead.h preprocess.h
	g++ -c fileRead.cpp
fileWrite.o: fileWrite.cpp Node.h Command.h global.h type.h fileWrite.h
	g++ -c fileWrite.cpp
global.o: global.cpp Node.h Command.h global.h
	g++ -c global.cpp
logicSim.o: logicSim.cpp Node.h Command.h global.h type.h logicSim.h fileRead.h fileWrite.h
	g++ -c logicSim.cpp
main.o: main.cpp Node.h Command.h fileRead.h global.h type.h logicSim.h preprocess.h fileWrite.h atpg.h
	g++ -c main.cpp
preprocess.o: preprocess.cpp Node.h Command.h global.h type.h preprocess.h fileRead.h fileWrite.h
	g++ -c preprocess.cpp

clean:
	rm -f *.o *.exe

