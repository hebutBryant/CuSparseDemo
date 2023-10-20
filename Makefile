# 定义编译器和标志
NVCC = nvcc
NVCC_FLAGS = -lcusparse

# 目标文件名
OUTPUT = ./test_program

# 源文件名
SRC = demo.cpp

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(NVCC) $(NVCC_FLAGS) $(SRC) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
