CXX = g++
SRC_DIR = src
BIN_DIR = bin
CPLEX_FLAGS = -I/opt/ibm/ILOG/CPLEX_Studio_Community2211/cplex/include \
              -I/opt/ibm/ILOG/CPLEX_Studio_Community2211/concert/include
CPLEX_LIBS = -L/opt/ibm/ILOG/CPLEX_Studio_Community2211/cplex/lib/x86-64_linux/static_pic \
             -L/opt/ibm/ILOG/CPLEX_Studio_Community2211/concert/lib/x86-64_linux/static_pic \
             -lilocplex -lconcert -lcplex -lpthread -lm
TARGET = $(BIN_DIR)/dckp

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Compilarei monolito
# Me perdooem engenheiros de software :(

#all:
#	$(CXX) $(CXXFLAGS) $(CPLEX_FLAGS) ./src/*.cpp -o $(TARGET) $(CPLEX_LIBS)

# Regras
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPLEX_FLAGS) $(OBJS) -o $(TARGET) $(CPLEX_LIBS)

$(BIN_DIR)/MetodosExatos.o: $(SRC_DIR)/MetodosExatos.cpp
	$(CXX) $(CXXFLAGS) $(CPLEX_FLAGS) -c $< -o $@ $(CPLEX_LIBS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(BIN_DIR)/*.o $(TARGET)
