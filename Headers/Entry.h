#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <iostream>

class Directory;

// Entry — корневой класс иерархии сущностей виртуальной файловой системы.
// Все сущности (Directory, SingleLineFile, MultiLineFile) наследуются от Entry.
// Класс НЕ абстрактный: все виртуальные методы имеют реализацию по умолчанию.
// Полиморфизм обеспечивается через переопределение виртуальных методов в потомках.
class Entry {
protected:
    std::string name;     // Имя сущности
    bool hidden;          // Флаг скрытости (мягкое удаление)
    Directory* parent;    // Указатель на родительскую директорию

public:
    Entry(const std::string& name, Directory* parent = nullptr);
    virtual ~Entry();

    // --- Геттеры и сеттеры ---
    std::string getName() const;
    void setName(const std::string& newName);

    bool isHidden() const;
    void hide();
    void restore();

    Directory* getParent() const;
    void setParent(Directory* newParent);

    // --- Полиморфные методы ---
    // Возвращает строковый тип сущности для отображения
    virtual std::string getType() const;

    // Отображает содержимое сущности (для cat)
    virtual void display() const;

    // Редактирует содержимое сущности (для edit)
    virtual void edit();

    // Проверяет, является ли сущность директорией
    virtual bool isDirectory() const;

    // Приводит сущность к Directory* (возвращает nullptr для не-директорий)
    virtual Directory* asDirectory();

    // Возвращает имя для отображения в ls (директории — с '/', файлы — без)
    virtual std::string displayName() const;
};

#endif // ENTRY_H
