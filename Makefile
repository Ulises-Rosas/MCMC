
CC       = g++
OBJFILES = main.o RandomVariable.o
TARGET   = mcmc

all: $(TARGET)

$(TARGET): RandomVariable main
	$(CC) $(OBJFILES) -o $(TARGET)

main:
	$(CC) -c src/main.cpp 

RandomVariable:
	$(CC) -c src/RandomVariable.cpp

clean:
	rm -f $(OBJFILES) $(TARGET) *~


