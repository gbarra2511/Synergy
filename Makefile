CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LDFLAGS = -lsqlite3

BUILD_DIR = build
SRC_BACKEND = src/backend
SRC_DATABASE = src/database
INCLUDE_DIR = include
BIN_DIR = bin

# Diretórios de teste
TEST_DIR = tests
TEST_TARGET = ${BUILD_DIR}/synergy_test

# Includes
CXXFLAGS += -I${INCLUDE_DIR} -I${INCLUDE_DIR}/backend -I${INCLUDE_DIR}/database

# ==================== ARQUIVOS OBJETO ====================

BACKEND_OBJECTS = \
	${BUILD_DIR}/Entity.o \
	${BUILD_DIR}/User.o \
	${BUILD_DIR}/Company.o \
	${BUILD_DIR}/Feed.o \
	${BUILD_DIR}/Post.o \
	${BUILD_DIR}/SocialNetwork.o \
	${BUILD_DIR}/Utils.o \
	${BUILD_DIR}/CaseSystem.o \
	${BUILD_DIR}/Case.o \
	${BUILD_DIR}/Ranking.o \
	${BUILD_DIR}/System.o

DATABASE_OBJECTS = \
	${BUILD_DIR}/DbManager.o

MAIN_OBJECT = ${BUILD_DIR}/main.o

ALL_OBJECTS = $(BACKEND_OBJECTS) $(DATABASE_OBJECTS) $(MAIN_OBJECT)

TARGET = ${BIN_DIR}/synergy

# ==================== TESTES ====================

TEST_SOURCES = $(wildcard ${TEST_DIR}/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:${TEST_DIR}/%.cpp=${BUILD_DIR}/%.o)

# Objetos para teste (sem main.o)
OBJECTS_FOR_TEST = $(BACKEND_OBJECTS) $(DATABASE_OBJECTS)

# ==================== REGRAS ====================

.PHONY: all run test coverage clean cleanall debug info

all: ${TARGET}

run: ${TARGET}
	@echo "Executando aplicação..."
	./${TARGET}

test: ${TEST_TARGET}
	@echo "Executando testes..."
	./${TEST_TARGET}

coverage: clean
	@echo "Compilando e executando testes com cobertura... *(demora em media 30seg a 1min)*"
	${CXX} ${CXXFLAGS} --coverage -o ${TEST_TARGET}_cov \
		${SRC_BACKEND}/*.cpp ${SRC_DATABASE}/*.cpp ${TEST_DIR}/*.cpp \
		${LDFLAGS}
	
	./${TEST_TARGET}_cov
	
	@echo "Gerando relatório de cobertura..."
	-gcovr -r . \
		--filter "src/backend/.*" \
		--exclude "src/database/.*" \
		--exclude "tests/.*" \
		--print-summary

info:
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Build dir: $(BUILD_DIR)"
	@echo "Bin dir: $(BIN_DIR)"
	@echo "Objetos: $(ALL_OBJECTS)"
	@echo "Executável: $(TARGET)"

# ==================== CRIAÇÃO DO BUILD DIR ====================

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BIN_DIR}:
	mkdir -p ${BIN_DIR}

# ==================== COMPILAÇÃO - BACKEND ====================

${BUILD_DIR}/Entity.o: ${SRC_BACKEND}/Entity.cpp ${INCLUDE_DIR}/backend/Entity.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Entity.cpp -o $@

${BUILD_DIR}/User.o: ${SRC_BACKEND}/User.cpp ${INCLUDE_DIR}/backend/User.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/User.cpp -o $@

${BUILD_DIR}/Company.o: ${SRC_BACKEND}/Company.cpp ${INCLUDE_DIR}/backend/Company.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Company.cpp -o $@

${BUILD_DIR}/Feed.o: ${SRC_BACKEND}/Feed.cpp ${INCLUDE_DIR}/backend/Feed.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Feed.cpp -o $@

${BUILD_DIR}/Post.o: ${SRC_BACKEND}/Post.cpp ${INCLUDE_DIR}/backend/Post.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Post.cpp -o $@

${BUILD_DIR}/SocialNetwork.o: ${SRC_BACKEND}/SocialNetwork.cpp ${INCLUDE_DIR}/backend/SocialNetwork.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/SocialNetwork.cpp -o $@

${BUILD_DIR}/Utils.o: ${SRC_BACKEND}/Utils.cpp ${INCLUDE_DIR}/backend/Utils.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Utils.cpp -o $@

${BUILD_DIR}/CaseSystem.o: ${SRC_BACKEND}/CaseSystem.cpp ${INCLUDE_DIR}/backend/CaseSystem.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/CaseSystem.cpp -o $@

${BUILD_DIR}/Case.o: ${SRC_BACKEND}/Case.cpp ${INCLUDE_DIR}/backend/Case.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Case.cpp -o $@

${BUILD_DIR}/Ranking.o: ${SRC_BACKEND}/Ranking.cpp ${INCLUDE_DIR}/backend/Ranking.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/Ranking.cpp -o $@

${BUILD_DIR}/System.o: ${SRC_BACKEND}/System.cpp ${INCLUDE_DIR}/backend/System.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_BACKEND}/System.cpp -o $@

# ==================== COMPILAÇÃO - DATABASE ====================

${BUILD_DIR}/DbManager.o: ${SRC_DATABASE}/DbManager.cpp ${INCLUDE_DIR}/database/DbManager.hpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c ${SRC_DATABASE}/DbManager.cpp -o $@

# ==================== COMPILAÇÃO - MAIN ====================

${BUILD_DIR}/main.o: src/main.cpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c src/main.cpp -o $@

# ==================== COMPILAÇÃO - TESTES ====================

${BUILD_DIR}/%.o: ${TEST_DIR}/%.cpp | ${BUILD_DIR}
	${CXX} ${CXXFLAGS} -c $< -o $@

# ==================== LINKAGEM ====================

${TARGET}: ${ALL_OBJECTS}
	${CXX} ${ALL_OBJECTS} -o ${TARGET} ${LDFLAGS}

${TEST_TARGET}: ${OBJECTS_FOR_TEST} ${TEST_OBJECTS}
	${CXX} $^ -o ${TEST_TARGET} ${LDFLAGS}

# ==================== LIMPEZA ====================

clean:
	rm -f ${BUILD_DIR}/*.o
	rm -f ${TARGET} ${TEST_TARGET} ${TEST_TARGET}_cov
	rm -f ${BUILD_DIR}/*.gcno ${BUILD_DIR}/*.gcda ${BUILD_DIR}/*.gcov

cleanall: clean
	rm -f *.db
	rm -rf ${BUILD_DIR}