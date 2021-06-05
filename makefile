myProg: main.o AntColony.o BranchAndBound.o BruteForce.o ClosestNeighbour.o Dynamic.o Genetic.o SimulatedAnnealing.o
	g++ main.o AntColony.o BranchAndBound.o BruteForce.o ClosestNeighbour.o Dynamic.o Genetic.o SimulatedAnnealing.o -o myProg
AntColony.o: AntColony.cpp AntColony.h
	g++ -c AntColony.cpp
BranchAndBound.o: BranchAndBound.cpp BranchAndBound.h
	g++ -c BranchAndBound.cpp
BruteForce.o: BruteForce.cpp BruteForce.h
	g++ -c BruteForce.cpp
ClosestNeighbour.o: ClosestNeighbour.cpp ClosestNeighbour.h
	g++ -c ClosestNeighbour.cpp
Dynamic.o: Dynamic.cpp Dynamic.h
	g++ -c Dynamic.cpp
Genetic.o: Genetic.cpp Genetic.h
	g++ -c Genetic.cpp
SimulatedAnnealing.o: SimulatedAnnealing.cpp SimulatedAnnealing.h
	g++ -c SimulatedAnnealing.cpp			
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm -f myProg *.o