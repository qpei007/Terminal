#ifndef SINGLELINEFILE_H
#define SINGLELINEFILE_H

#include "Entry.h"
#include <string>

// SingleLineFile — однострочный текстовый файл.
// Хранит одну строку текста (content).
// При edit() пользователь вводит одну строку для замены содержимого.
// При display() выводит содержимое файла.
class SingleLineFile : public Entry {
private:
    std::string content;  // Содержимое файла (одна строка)

public:
    SingleLineFile(const std::string& name, Directory* parent = nullptr);
    ~SingleLineFile() override;

    std::string getContent() const;
    void setContent(const std::string& newContent);

    // --- Переопределённые полиморфные методы ---
    std::string getType() const override;
    void display() const override;
    void edit() override;
    std::string displayName() const override;
};

#endif // SINGLELINEFILE_H
