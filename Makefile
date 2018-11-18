CC = gcc
OBJ1 = ./simulation/simulation.c $(OBJ4)
OBJ1FN = sim
OBJ2 = ./constructionDep/constructionDep.c $(OBJ4)
OBJ2FN = constructionDp
OBJ3 = ./paintDep/paintDep.c $(OBJ4)
OBJ3FN = paintDp
OBJ4 = ./semaphores/semCtrl.c ./sharedMemory/shmCtrl.c
OBJ5 = ./checkDep/checkDep.c $(OBJ4)
OBJ5FN = checkDp
KEYS = sem.Paintkey shmem.Paintkey sem.Checkkey shmem.Checkkey
CLC = $(OBJ1FN) $(OBJ2FN) $(OBJ3FN) $(OBJ5) $(KEYS)

make: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ5)
	make clean
	$(CC) -o $(OBJ5FN) $(OBJ5)
	$(CC) -o $(OBJ3FN) $(OBJ3)
	$(CC) -o $(OBJ2FN) $(OBJ2)
	$(CC) -o $(OBJ1FN) $(OBJ1)

	

.PHONY: clean	
	
clean:
	rm -f $(CLC)
