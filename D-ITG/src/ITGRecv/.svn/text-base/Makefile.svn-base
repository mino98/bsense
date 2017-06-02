all:   ITGRecv

OBJS = ../common/ITG.o ../common/timestamp.o ../common/serial.o ../common/pipes.o ../$(THOBJS) data.o

ITGRecv: ../common/ITG.h ITGRecv.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) ITGRecv.cpp -o $@$(SUFFIX) $(LDFLAGS)
	$(MV) $@$(SUFFIX) "$(BIN)"

clean:
	$(RM) *.o ITGRecv$(SUFFIX) "$(BIN)"ITGRecv$(SUFFIX)

