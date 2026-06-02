# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iheader

# Имя итогового бинарника
TARGET = terminal

# Исходные файлы
SRCS = main.cpp \
       cpp/Entry.cpp \
       cpp/Directory.cpp \
       cpp/SingleLineFile.cpp \
       cpp/MultiLineFile.cpp \
       cpp/Terminal.cpp

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
