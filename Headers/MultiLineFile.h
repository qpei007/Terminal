#ifndef MULTILINEFILE_H
#define MULTILINEFILE_H

#include "Entry.h"
#include <vector>
#include <string>

// MultiLineFile — многострочный текстовый файл.
// Хранит вектор строк (lines).
// При edit() пользователь вводит несколько строк, завершая ввод командой ":wq".
// При display() выводит все строки с нумерацией.
// Это ключевая точка полиморфизма: поведение edit() и display()
// отличается от SingleLineFile через переопределение виртуальных методов.
class MultiLineFile : public Entry {
private:
    std::vector<std::string> lines;  // Содержимое файла (набор строк)

public:
    MultiLineFile(const std::string& name, Directory* parent = nullptr);
    ~MultiLineFile() override;

    const std::vector<std::string>& getLines() const;
    void setLines(const std::vector<std::string>& newLines);

    // --- Переопределённые полиморфные методы ---
    std::string getType() const override;
    void display() const override;
    void edit() override;
    std::string displayName() const override;
};

#endif // MULTILINEFILE_H
