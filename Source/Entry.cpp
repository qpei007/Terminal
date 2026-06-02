#include "Entry.h"

// Конструктор: инициализирует имя, флаг скрытости (false) и родителя
Entry::Entry(const std::string& name, Directory* parent)
    : name(name), hidden(false), parent(parent) {
}

// Виртуальный деструктор — необходим для корректного
// полиморфного удаления объектов через указатель Entry*
Entry::~Entry() {}

// --- Геттеры и сеттеры ---

std::string Entry::getName() const {
    return name;
}

void Entry::setName(const std::string& newName) {
    name = newName;
}

bool Entry::isHidden() const {
    return hidden;
}

void Entry::hide() {
    hidden = true;
}

void Entry::restore() {
    hidden = false;
}

Directory* Entry::getParent() const {
    return parent;
}

void Entry::setParent(Directory* newParent) {
    parent = newParent;
}

// --- Реализация виртуальных методов по умолчанию ---
// Эти методы переопределяются в наследниках (Directory, SingleLineFile, MultiLineFile).
// Базовые реализации обеспечивают безопасное поведение «по умолчанию».

std::string Entry::getType() const {
    return "entry";
}

void Entry::display() const {
    std::cout << "[Entry] " << name << std::endl;
}

void Entry::edit() {
    std::cout << "Error: this entry cannot be edited." << std::endl;
}

bool Entry::isDirectory() const {
    return false;
}

Directory* Entry::asDirectory() {
    return nullptr;
}

std::string Entry::displayName() const {
    return name;
}
