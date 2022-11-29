.PHONY: clean
CXX := g++
CXXFLAGS := #-g #-g -fsanitize=address -fno-omit-frame-pointer
LINKFLAGS := -lSDL2 -lSDL2_image
OBJS_DIR := .
OUT_DIR := .
SOURCE_DIR := .
INCLUDE_DIR := .
EXEC := main
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
SOURCE_HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
OBJS := $(subst $(SOURCE_DIR),$(OBJS_DIR), $(patsubst %.cpp,%.o,$(SOURCE_FILES)))

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -Iinclude -Llib
	LINKFLAGS := -lmingw32 -lSDL2main $(LINKFLAGS)
else
	CXXFLAGS += -I$(INCLUDE_DIR)
endif

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LINKFLAGS) -o $(OUT_DIR)/$(EXEC)

$(OBJS_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o : $(INCLUDE_DIR)/*/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Files to be compiled
$(OBJS_DIR)/main.o: $(SOURCE_FILES) $(SOURCE_HEADERS)
$(OBJS_DIR)/Tile.o: $(SOURCE_DIR)/Tile.cpp $(SOURCE_DIR)/Tile.h $(SOURCE_DIR)/Sprite.cpp  $(SOURCE_DIR)/Sprite.h
$(OBJS_DIR)/Sprite.o: $(SOURCE_DIR)/Sprite.cpp  $(SOURCE_DIR)/Sprite.h

clean:
	rm $(OBJS_DIR)/*.o 
	rm $(OUT_DIR)/$(EXEC)
