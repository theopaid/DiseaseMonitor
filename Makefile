objects = src/main.o src/helpfulFunctions.o src/menuFunctions.o src/recordList.o src/hashTable.o src/avlTree.o

diseaseMonitor : $(objects)
		cc -o bin/diseaseMonitor $(objects)
		@echo "========================================================================"
		@echo "|Try running: ./bin/diseaseMonitor -b 70 -h1 10 -h2 10 -p bin/small.txt|"
		@echo "|Validation: ./bin/validator.sh bin/small.txt 10 10 70                 |"
		@echo "========================================================================"

src/main.o : include/Interface.h
src/helpfulFunctions.o : include/Interface.h
src/menuFunctions.o : include/Interface.h
src/recordList.o : include/Interface.h
src/hashTable.o : include/Interface.h
src/avlTree.o : include/Interface.h

.PHONY: clean
clean :
		@echo "Cleaning ...";
		rm bin/diseaseMonitor $(objects)
