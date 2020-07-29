.PHONY: all clean test

all: NMM-csim.bin 

NMM-csim.bin: NMM-cachesim.c csim.c utils.c YOURCODEHERE.c utils.h csim.h NMM.h YOURCODEHERE.h
	gcc -std=gnu99 -O3 -g NMM-cachesim.c csim.c utils.c YOURCODEHERE.c -o NMM-csim.bin

clean:
	rm  NMM-csim.bin NMM-csim.testout

test: all
	echo "./NMM-csim.bin 2 1 MainMem 1 1 1 " > NMM-csim.testout
	./NMM-csim.bin 2 1 MainMem 1 1 1 >> NMM-csim.testout
	echo "./NMM-csim.bin 2 1 MainMem 1 1 1 L2 128 1 32 L1 64 1 32" >> NMM-csim.testout
	./NMM-csim.bin 2 1 MainMem 1 1 1 L2 128 1 32 L1 64 1 32 >> NMM-csim.testout
	echo "./NMM-csim.bin 2 23 MainMem 1 1 1 L2 128 1 32 L1 64 1 32" >> NMM-csim.testout
	./NMM-csim.bin 2 23 MainMem 1 1 1 L2 128 1 32 L1 64 1 32 >> NMM-csim.testout
	echo "./NMM-csim.bin 2 3 MainMem 1 1 1 L2 128 1 32 L1 64 2 16" >> NMM-csim.testout
	./NMM-csim.bin 2 3 MainMem 1 1 1 L2 128 1 32 L1 64 2 16 >> NMM-csim.testout
	echo "./NMM-csim.bin 8 1 MainMem 1 1 1 L2 512 8 32 L1 128 2 16" >> NMM-csim.testout
	./NMM-csim.bin 8 1 MainMem 1 1 1 L2 512 8 32 L1 128 2 16 >> NMM-csim.testout
	echo "./NMM-csim.bin 16 1 MainMem 1 1 1 L1 16384 1 64" >> NMM-csim.testout
	./NMM-csim.bin 16 1 MainMem 1 1 1 L1 16384 1 64 >> NMM-csim.testout
	echo "./NMM-csim.bin 64 1 MainMem 1 1 1 L1 16384 1 64" >> NMM-csim.testout
	./NMM-csim.bin 64 1 MainMem 1 1 1 L1 16384 1 64 >> NMM-csim.testout
	echo "./NMM-csim.bin 64 100 MainMem 1 1 1 L1 16384 1 64" >> NMM-csim.testout
	./NMM-csim.bin 64 100 MainMem 1 1 1 L1 16384 1 64 >> NMM-csim.testout
	echo "./NMM-csim.bin 256 3 MainMem 1 1 1 L3 2097152 16 128 L2 262144 4 128 L1 32768 2 128" >> NMM-csim.testout
	./NMM-csim.bin 256 3 MainMem 1 1 1 L3 2097152 16 128 L2 262144 4 128 L1 32768 2 128 >> NMM-csim.testout
