
all: conway clean

conway: conway.o math2Dlib.o
	g++ -o Conway $^ -g -Wall -std=c++11 -lfreeglut -lglu32 -lopengl32

clean:
	del *.o