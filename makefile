CXX=g++
CXXFLAGS=-Wall -Wextra -fpermissive
employee.o:employee.c
	$(CXX) $(CXXFLAGS) -g -c $< -o $@
app.o:app.c
	$(CXX) $(CXXFLAGS) -g -c $< -o $@	
main:main.c employee.o app.o
	$(CXX) $(CXXFLAGS) -g $^ -lpqxx -lpq -o $@

