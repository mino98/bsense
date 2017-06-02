all:   ITGLog
OBJS = ../common/ITG.o ../common/timestamp.o ../$(THOBJS) channel.o

ITGLog: ../common/ITG.h ITGLog.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) ITGLog.cpp -o $@$(SUFFIX) $(LDFLAGS)
	$(MV) $@$(SUFFIX) "$(BIN)"

clean:
	$(RM) *.o ITGLog$(SUFFIX) "$(BIN)"ITGLog$(SUFFIX)

