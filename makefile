
ALLFILE = main.o analysisIR.o keyFun.o outPut.o regAlloc.o
STD = -std=gnu++11

test:	${ALLFILE}
	g++ ${ALLFILE} -o $@ ${STD}

main.o: main.cpp analysisIR.h	
	g++ -c $< ${STD}

analysisIR.o: analysisIR.cpp analysisIR.h keyWord.h keyFun.h
	g++ -c $< ${STD}

keyFun.o: keyFun.cpp keyFun.h outPut.h regAlloc.h
	g++ -c $< ${STD}

outPut.o: outPut.cpp outPut.h
	g++ -c $< ${STD}

regAlloc.o: regAlloc.cpp regAlloc.h
	g++ -c $< ${STD}

clean:
	rm -rf ${ALLFILE}
