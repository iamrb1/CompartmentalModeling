# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -g -pthread -Wall -Wextra
INCLUDES := -I../ -I../third-party/Catch/single_include/ -I../third-party/cereal/include/
DEFINES := -DCATCH_CONFIG_MAIN
LDFLAGS := -static-libgcc -static-libstdc++

# Targets
TEST_NAMES := AuditedPointerAdvancedTest AnnotatedWrapperAdvancedTest
TEST_SRCS := $(addsuffix .cpp, $(TEST_NAMES))
TEST_EXES := $(addsuffix .out, $(TEST_NAMES))

# Default build flags
BUILD_FLAGS := $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(LDFLAGS)

# Special builds
COV_FLAGS := -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
OPT_FLAGS := -DNDEBUG -O3
FULLDEBUG_FLAGS := -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -pedantic
CRANKY_FLAGS := $(FULLDEBUG_FLAGS) -Wconversion -Weffc++

.PHONY: all test coverage opt fulldebug cranky clean test-prep

all: test

# Preparation
test-prep:
	@mkdir -p temp

# Default test build
test: test-prep $(TEST_EXES)
	@for test in $(TEST_EXES); do \
		./$$test; \
	done

# Pattern rule for test builds with verbose output
%.out: %.cpp
	@$(CXX) $(BUILD_FLAGS) $< -o $@

# Coverage build
coverage: BUILD_FLAGS += $(COV_FLAGS)
coverage: test-prep $(TEST_EXES)
	@echo "Running tests with coverage..."
	@for test in $(TEST_EXES); do \
		LLVM_PROFILE_FILE="$$test.profraw" ./$$test; \
		llvm-profdata merge -sparse "$$test.profraw" -o "$$test.profdata"; \
		llvm-cov show ./$$test -instr-profile="$$test.profdata"; \
	done

# Optimized build
opt: BUILD_FLAGS += $(OPT_FLAGS)
opt: test

# Full debug build
fulldebug: BUILD_FLAGS += $(FULLDEBUG_FLAGS)
fulldebug: test

# Cranky (extra warnings) build
cranky: BUILD_FLAGS += $(CRANKY_FLAGS)
cranky: test

# Clean up
	@rm -f *.out *.o *.gcda *.gcno *.info *.gcov *.profraw *.profdata
	@rm -rf ./Coverage* ./temp

# Ensure catch.hpp is available
../third-party/Catch/single_include/catch2/catch.hpp:
	@git submodule update --init

# Dependencies
$(TEST_EXES): ../third-party/Catch/single_include/catch2/catch.hpp