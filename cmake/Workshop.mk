# Shared compatibility wrapper. CMake owns the actual dependencies and output.
BUILD_DIR ?= $(ROOT)/build$(if $(filter ON,$(GRAPHICS)),-graphics)
.PHONY: all build clean $(TARGET) $(LEGACY_TARGET)
all build $(TARGET) $(LEGACY_TARGET):
	cmake -S "$(ROOT)" -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=Release -DPLAYGROUND_BUILD_GRAPHICS=$(GRAPHICS)
	cmake --build "$(BUILD_DIR)" --config Release --target $(TARGET)
clean:
	cmake --build "$(BUILD_DIR)" --config Release --target clean
