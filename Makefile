# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -IHeaders

# Имя итогового бинарника
TARGET = terminal

# Исходные файлы
SRCS = main.cpp \
       Source/Entry.cpp \
       Source/Directory.cpp \
       Source/SingleLineFile.cpp \
       Source/MultiLineFile.cpp \
       Source/Terminal.cpp

# Объектные файлы (автоматическая замена .cpp -> .o)
OBJS = $(SRCS:.cpp=.o)

# Цель по умолчанию
all: $(TARGET)

# Линковка: собирает все объектные файлы в итоговый бинарник
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Общее правило компиляции .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
