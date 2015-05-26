generator: generator.cpp
	g++ -std=c++11 -Wall -O2 $< -o $@

run:
	./generator < character.in

clean:
	rm ./generator
