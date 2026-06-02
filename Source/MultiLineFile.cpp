#include "MultiLineFile.h"

// Конструктор: создаёт многострочный файл с пустым набором строк
MultiLineFile::MultiLineFile(const std::string& name, Directory* parent)
    : Entry(name, parent) {
}

MultiLineFile::~MultiLineFile() {}

const std::vector<std::string>& MultiLineFile::getLines() const {
    return lines;
}

void MultiLineFile::setLines(const std::vector<std::string>& newLines) {
    lines = newLines;
}

// --- Переопределённые полиморфные методы ---

std::string MultiLineFile::getType() const {
    return "multi-line file";
}

// display() для многострочного файла: выводит все строки с нумерацией.
// Отличается от SingleLineFile::display() — нумерованный вывод.
void MultiLineFile::display() const {
    if (lines.empty()) {
        std::cout << "(file is empty)" << std::endl;
    }
    else {
        for (size_t i = 0; i < lines.size(); ++i) {
            std::cout << (i + 1) << ": " << lines[i] << std::endl;
        }
    }
}

// edit() для многострочного файла: запрашивает несколько строк.
// Ввод завершается командой ":wq" (по аналогии с vim).
// Это КЛЮЧЕВАЯ точка полиморфизма: MultiLineFile::edit() ведёт себя
// принципиально иначе, чем SingleLineFile::edit().
// Terminal не знает конкретный тип — он вызывает entry->edit(),
// а виртуальная диспетчеризация выбирает правильную реализацию.
void MultiLineFile::edit() {
    std::cout << "Current content:" << std::endl;
    if (lines.empty()) {
        std::cout << "  (empty)" << std::endl;
    }
    else {
        for (size_t i = 0; i < lines.size(); ++i) {
            std::cout << "  " << (i + 1) << ": " << lines[i] << std::endl;
        }
    }
    std::cout << "Enter new content (type ':wq' on a separate line to finish):" << std::endl;

    std::vector<std::string> newLines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == ":wq") {
            break;
        }
        newLines.push_back(line);
    }
    lines = newLines;
    std::cout << "File '" << name << "' updated (" << lines.size() << " lines)." << std::endl;
}

std::string MultiLineFile::displayName() const {
    return name;
}
