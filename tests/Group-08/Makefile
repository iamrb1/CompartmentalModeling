# Top-level test folders
TEST_NAMES := AnnotatedWrapperTest AuditedPointerTest DynamicStringTest RandomAccessSetTest TagManagerTest
ADVANCED_DIR := AdvancedTests

CXX = g++

default: test

# Run basic test suites
test-%:
	cd $(@:test-%=%) && make test

# Run all tests
test: $(addprefix test-, $(TEST_NAMES)) test-advanced

# Run advanced tests
test-advanced:
	cd $(ADVANCED_DIR) && make test

# Coverage rules
coverage-%:
	cd $(@:coverage-%=%) && make coverage

coverage: coverage_conversion $(addprefix coverage-, $(TEST_NAMES)) coverage-advanced

coverage-advanced:
	cd $(ADVANCED_DIR) && make coverage

coverage_conversion:
	./convert_for_tests.sh

# Optimized builds
opt-%:
	cd $(@:opt-%=%) && make opt

opt: $(addprefix opt-, $(TEST_NAMES)) opt-advanced

opt-advanced:
	cd $(ADVANCED_DIR) && make opt

# Full debug builds
fulldebug-%:
	cd $(@:fulldebug-%=%) && make fulldebug

fulldebug: $(addprefix fulldebug-, $(TEST_NAMES)) fulldebug-advanced

fulldebug-advanced:
	cd $(ADVANCED_DIR) && make fulldebug

# Strictest build
cranky-%:
	cd $(@:cranky-%=%) && make cranky

cranky: $(addprefix cranky-, $(TEST_NAMES)) cranky-advanced

cranky-advanced:
	cd $(ADVANCED_DIR) && make cranky

# Clean everything
clean: $(addprefix clean-, $(TEST_NAMES)) clean-advanced
	rm -f *.out *.o *.gcda *.gcno *.info *.gcov
	rm -f ./Coverage*
	rm -rf ./temp

clean-%:
	cd $(@:clean-%=%) && make clean

clean-advanced:
	cd $(ADVANCED_DIR) && make clean