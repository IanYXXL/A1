CFLAGS=-g -O2 -Wall


PROGRAM_NAME= ppmf
OBJS = main.o readwriteppm.o processimage.o 

$(PROGRAM_NAME): $(OBJS)
	MPICC -o $@ $? 

clean:
	rm  *.o $(PROGRAM_NAME) *~
