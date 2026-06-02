#include "Directory.h"
#include <algorithm>

// Конструктор: создаёт директорию с заданным именем и родителем
Directory::Directory(const std::string& name, Directory* parent)
    : Entry(name, parent) {
}

// Деструктор: рекурсивно удаляет все дочерние сущности.
// Благодаря виртуальному деструктору Entry, каждый потомок
// корректно освобождает свои ресурсы.
Directory::~Directory() {
    for (Entry* child : children) {
        delete child;
    }
}

// Добавляет дочернюю сущность и устанавливает текущую директорию как родителя
void Directory::addChild(Entry* child) {
    child->setParent(this);
    children.push_back(child);
}

// Удаляет сущность из вектора детей (без вызова delete).
// Используется если нужно перенести сущность в другое место.
void Directory::removeChild(Entry* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}

// Поиск дочерней сущности по имени.
// Параметр includeHidden определяет, учитывать ли скрытые сущности:
//   - false (по умолчанию): поиск только среди видимых (для ls, cd, cat, edit, rm)
//   - true: поиск включая скрытые (для restore, проверки уникальности имени при touch/mkdir)
Entry* Directory::findChild(const std::string& childName, bool includeHidden) const {
    for (Entry* child : children) {
        if (child->getName() == childName) {
            if (includeHidden || !child->isHidden()) {
                return child;
            }
        }
    }
    return nullptr;
}

// Возвращает вектор всех дочерних сущностей (включая скрытые)
const std::vector<Entry*>& Directory::getChildren() const {
    return children;
}

// --- Переопределённые полиморфные методы ---

std::string Directory::getType() const {
    return "directory";
}

// display() для директории: выводит список видимых дочерних сущностей.
// Фильтрация скрытых происходит здесь — это авторитативное решение:
// ответственность за фильтрацию лежит на Directory::display().
void Directory::display() const {
    bool hasVisible = false;
    for (const Entry* child : children) {
        if (!child->isHidden()) {
            std::cout << "  " << child->displayName() << std::endl;
            hasVisible = true;
        }
    }
    if (!hasVisible) {
        std::cout << "  (empty)" << std::endl;
    }
}

// Директории нельзя редактировать — выводим сообщение об ошибке
void Directory::edit() {
    std::cout << "Error: '" << name << "' is a directory and cannot be edited." << std::endl;
}

bool Directory::isDirectory() const {
    return true;
}

Directory* Directory::asDirectory() {
    return this;
}

// Имя директории отображается с '/' на конце для визуального отличия от файлов
std::string Directory::displayName() const {
    return name + "/";
}
