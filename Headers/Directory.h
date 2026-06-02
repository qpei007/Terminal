#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Entry.h"
#include <vector>
#include <string>

// Directory — директория виртуальной файловой системы.
// Хранит вектор дочерних сущностей (Entry*).
// Отвечает за поиск дочерних элементов по имени и управление ими.
// Переопределяет полиморфные методы: display, edit, isDirectory, asDirectory, displayName.
class Directory : public Entry {
private:
    std::vector<Entry*> children;  // Дочерние сущности

public:
    Directory(const std::string& name, Directory* parent = nullptr);
    ~Directory() override;

    // Добавляет дочернюю сущность и устанавливает ей родителя
    void addChild(Entry* child);

    // Удаляет дочернюю сущность из вектора (без delete)
    void removeChild(Entry* child);

    // Поиск дочерней сущности по имени.
    // includeHidden = true позволяет находить скрытые сущности (для restore)
    Entry* findChild(const std::string& childName, bool includeHidden = false) const;

    // Доступ к списку всех дочерних сущностей (для итерации)
    const std::vector<Entry*>& getChildren() const;

    // --- Переопределённые полиморфные методы ---
    std::string getType() const override;
    void display() const override;
    void edit() override;
    bool isDirectory() const override;
    Directory* asDirectory() override;
    std::string displayName() const override;
};

#endif // DIRECTORY_H
