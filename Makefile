CC = gcc

OBJ1 = ./simulation/simulation.c $(OBJ4)
OBJ1FN = sim

OBJ2 = ./constructionDep/constructionDep.c $(OBJ4)
OBJ2FN = constructionDp

OBJ3 = ./paintDep/paintDep.c $(OBJ4)
OBJ3FN = paintDp

OBJ5 = ./checkDep/checkDep.c $(OBJ4)
OBJ5FN = checkDp

OBJ6 = ./assemblyDep/assemblyDep.c $(OBJ4)
OBJ6FN = assemblyDp

OBJ4 = ./semaphores/semCtrl.c ./sharedMemory/shmCtrl.c ./queue/queue.c

KEYS = sem.Paintkey shmem.Paintkey sem.Checkkey shmem.Checkkey1 shmem.Checkkey2 shmem.Checkkey3 sem.Assemblykey shmem.Assemblykey1 shmem.Assemblykey2 shmem.Assemblykey3 shmem.Timekey

CLC = $(OBJ1FN) $(OBJ2FN) $(OBJ3FN) $(OBJ5FN) $(OBJ6FN) $(KEYS)

make: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ5) $(OBJ6)
	make clean
	$(CC) -o $(OBJ6FN) $(OBJ6)
	$(CC) -o $(OBJ5FN) $(OBJ5)
	$(CC) -o $(OBJ3FN) $(OBJ3)
	$(CC) -o $(OBJ2FN) $(OBJ2)
	$(CC) -o $(OBJ1FN) $(OBJ1)

	

.PHONY: clean	
	
clean:
	rm -f $(CLC)
