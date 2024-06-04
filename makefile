all: client main
CC = gcc
LD = gcc
LIBRARYFOLDER = library
OBJECTFOLDER = object
CLIENT = source/client
FUNCTIONS = $(LIBRARYFOLDER)/_functions
SEMAPHORE = $(LIBRARYFOLDER)/_semaphore
MEMORY = $(LIBRARYFOLDER)/_shared_memory
VALIDATIONS = $(LIBRARYFOLDER)/_validations
SDL = $(LIBRARYFOLDER)/_SDL

OBJCLIENT = $(OBJECTFOLDER)/client.o
OBJFUNCTIONS = $(OBJECTFOLDER)/_functions.o
OBJSEMAPHORE = $(OBJECTFOLDER)/_semaphore.o
OBJMEMORY = $(OBJECTFOLDER)/_shared_memory.o
OBJVALIDATIONS = $(OBJECTFOLDER)/_validations.o
OBJSDL = $(OBJECTFOLDER)/_SDL.o

OBJECTS = $(OBJCLIENT) $(OBJSDL) $(OBJFUNCTIONS) $(OBJSEMAPHORE) $(OBJMEMORY) $(OBJVALIDATIONS)
SOURCE = $(CLIENT).c $(SDL).c $(FUNCTIONS).c $(SEMAPHORE).c $(MEMORY).c $(VALIDATIONS).c
CFLAGS = -c
LFLAGS = -lpthread -lrt -lSDL -lSDL_ttf

main:
	echo "Compilacion Terminada"

client: $(OBJECTS)
	$(LD) $(OBJECTS) -o client $(LFLAGS)

object/client.o: $(SOURCE)
	$(CC) $(CFLAGS) $(CLIENT).c -o $(OBJCLIENT) $(LFLAGS)

object/_SDL.o: $(SDL).c
	$(CC) $(CFLAGS) $(SDL).c -o $(OBJSDL) -lSDL -lSDL_tff

object/_functions.o: $(FUNCTIONS).c $(SEMAPHORE).c $(SEMAPHORE).c $(VALIDATIONS).c
	$(CC) $(CFLAGS) $(FUNCTIONS).c -o $(OBJFUNCTIONS) $(LFLAGS)

object/_semaphore.o: $(SEMAPHORE).c
	$(CC) $(CFLAGS) $(SEMAPHORE).c -o $(OBJSEMAPHORE) $(LFLAGS)

object/_shared_memory.o: $(MEMORY).c
	$(CC) $(CFLAGS) $(MEMORY).c -o $(OBJMEMORY) $(LFLAGS)
	
object/_validations.o: $(VALIDATIONS).c
	$(CC) $(CFLAGS) $(VALIDATIONS).c -o $(OBJVALIDATIONS) $(LFLAGS)
	
.PHONY : clean
clean:
	rm client $(OBJECTFOLDER)/*.o
