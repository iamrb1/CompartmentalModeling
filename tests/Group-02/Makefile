APPLICATION_TEST_FOLDER = Application

# Test names
TEST_NAMES = AuditedVectorDebugTest AuditedVectorReleaseTest DataMapTest StateGridTest StateGridPositionTest ActionMapTest AdvDataMapTest AdvStateGridTest

# Directories and sources
GROUP02 = ../../Group-02
LIB_INCLUDE = -I$(GROUP02)/include/
LIB_DIR = $(GROUP02)/src/cse
APP_DIR = $(GROUP02)/src/app
SRC_FILES = $(wildcard $(LIB_DIR)/*.cpp) \
            $(APP_DIR)/Simulation.cpp \
            $(APP_DIR)/Components/Compartment.cpp \
            $(APP_DIR)/Components/Connection.cpp
CATCH2_INCLUDE = -I../../third-party/Catch/single_include/

# Qt settings
QT_DIR = /mnt/c/Qt/6.8.3/msvc2022_64
QT_INCLUDE = -I$(QT_DIR)/include \
             -I$(QT_DIR)/include/QtQuick \
             -I$(QT_DIR)/include/QtCharts \
             -I$(QT_DIR)/include/QtCore \
             -I$(QT_DIR)/include/QtQml \
             -I$(QT_DIR)/include/QtWidgets

QT_LIBS = -L$(QT_DIR)/lib -lQt6Core -lQt6Quick -lQt6Charts -lQt6Qml -lQt6Widgets

# Compiler flags
FLAGS = -std=c++20 -g -pthread -Wall -Wextra -I../../ $(LIB_INCLUDE) $(CATCH2_INCLUDE) $(QT_INCLUDE) -DCATCH_CONFIG_MAIN

# Linking for Qt libraries
LINK_FLAGS = $(QT_LIBS) -Wl,-rpath,$(QT_DIR)/lib

default: test

cov-%: %.cpp ../../third-party/Catch/single_include/catch2/catch.hpp
	$(CXX) $(FLAGS) $(SRC_FILES) $< -o $@.out $(LINK_FLAGS)
	#echo "running $@.out"
	# execute test
	./$@.out
	llvm-profdata merge default.profraw -o default.profdata
	llvm-cov show ./$@.out -instr-profile=default.profdata > coverage_$@.txt
	python ../../third-party/force-cover/fix_coverage.py coverage_$@.txt

test-prep:
	mkdir -p temp

test-%: %.cpp ../../third-party/Catch/single_include/catch2/catch.hpp
	$(CXX) $(FLAGS) $(SRC_FILES) $< -o $@ $(LINK_FLAGS)
	./$@.out

# Test in debug mode without pointer tracker
test: test-prep $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

coverage: FLAGS := -std=c++20 -pthread -g -Wall -Wno-unused-function -I../../coverage_source/ -I../../ -I../../third-party/cereal/include/ -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0 -DCATCH_CONFIG_MAIN
coverage: test-prep $(addprefix cov-, $(TEST_NAMES))

# Test optimized version without debug features
opt: FLAGS := -std=c++20 -pthread -DNDEBUG -O3 -Wno-unused-function -I../../include/ -I../../third-party/cereal/include/ -I../../ -DCATCH_CONFIG_MAIN
opt: test-prep $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++20 -pthread -g -Wall -Wno-unused-function -I../../include/ -I../../third-party/cereal/include/ -I../../ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -DCATCH_CONFIG_MAIN # -Wmisleading-indentation
fulldebug: test-prep $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++20 -pthread -g -Wall -Wno-unused-function -I../../include/ -I../../third-party/cereal/include/ -I../../ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++ -DCATCH_CONFIG_MAIN
cranky: test-prep $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

../../third-party/Catch/single_include/catch2/catch.hpp:
	git submodule init
	git submodule update

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
