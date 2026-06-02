#include "SingleLineFile.h"

// Конструктор: создаёт однострочный файл с пустым содержимым
SingleLineFile::SingleLineFile(const std::string& name, Directory* parent)
    : Entry(name, parent), content("") {
}

SingleLineFile::~SingleLineFile() {}

std::string SingleLineFile::getContent() const {
    return content;
}

void SingleLineFile::setContent(const std::string& newContent) {
    content = newContent;
}

// --- Переопределённые полиморфные методы ---

std::string SingleLineFile::getType() const {
    return "single-line file";
}

// display() для однострочного файла: выводит единственную строку содержимого.
// Если файл пуст, выводит соответствующее сообщение.
void SingleLineFile::display() const {
    if (content.empty()) {
        std::cout << "(file is empty)" << std::endl;
    }
    else {
        std::cout << content << std::endl;
    }
}

// edit() для однострочного файла: запрашивает одну строку текста.
// SingleLineFile::edit() запрашивает одну строку,
// MultiLineFile::edit() — несколько строк.
// Terminal вызывает entry->edit() — и нужный метод выбирается автоматически.
void SingleLineFile::edit() {
    std::cout << "Current content: ";
    if (content.empty()) {
        std::cout << "(empty)" << std::endl;
    }
    else {
        std::cout << content << std::endl;
    }
    std::cout << "Enter new content (single line): ";
    std::string line;
    std::getline(std::cin, line);
    content = line;
    std::cout << "File '" << name << "' updated." << std::endl;
}

std::string SingleLineFile::displayName() const {
    return name;
}
