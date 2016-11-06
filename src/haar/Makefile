####################################################################################
# 编译相关目录及编译参数设定

# bin文件生成目录
BIN_DIR=../../bin
# obj文件生成目录(属于临时文件)
OBJ_DIR=/tmp/obj
# 编译依赖关系文件目录(属于临时文件)
DEPS_DIR=/tmp/deps
# 编译参数(调试信息、编译等级等参数)
CFLAGS=-g -O2 -fopenmp 
# lib文件路径及链接的lib
LINK_LIB=-lMagick++ -lMagickCore -lMagick++ -lMagickCore
# 头文件路径
LINK_INC=-I/usr/include/ImageMagick -I/usr/include/ImageMagick

TARGETS=test_haar2d test_haar_finger

####################################################################################
# 以下不要改动

# 所有的obj文件
ALL_OBJ=$(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))
# 所有带main函数的obj文件
MAIN_OBJ=$(addsuffix .o, $(TARGETS))
# 所有通用的obj文件
COMM_OBJ=$(filter-out $(MAIN_OBJ), $(ALL_OBJ))
# 所有bin文件的名称
BIN_NAME=$(patsubst %.o, %, $(MAIN_OBJ))
# 所有bin文件的路径
ALL_BIN=$(addprefix $(BIN_DIR)/, $(BIN_NAME))

all: $(ALL_BIN)

# 自动推导编译规则
$(BIN_DIR)/%:$(COMM_OBJ) %.o
	mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $(patsubst %.o, $(OBJ_DIR)/%.o, $^) $(LINK_LIB)
	
.cpp.o:
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) ${LINK_INC} -c $< -o $(OBJ_DIR)/$@

.c.o:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) ${LINK_INC} -c $< -o $(OBJ_DIR)/$@

#生成对头文件依赖关系文件
$(DEPS_DIR)/%.d:%.cpp
	mkdir -p $(DEPS_DIR)
	@set -e; rm -f $@;
	-$(CC) -MM ${LINK_INC} $<   > $@.tmp;
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' <  $@.tmp  > $@;
	rm -f $@.tmp

#包含所有依赖关系的文件
include $(patsubst %.cpp,$(DEPS_DIR)/%.d,$(wildcard *.cpp))

clean:
	rm -f $(OBJ_DIR)/*.o $(ALL_BIN) $(DEPS_DIR)/*.d
	rmdir $(DEPS_DIR)
	rmdir $(OBJ_DIR)

