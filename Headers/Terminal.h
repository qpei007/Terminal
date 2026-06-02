#ifndef TERMINAL_H
#define TERMINAL_H

#include "Directory.h"
#include <vector>
#include <string>

// Terminal — основной класс приложения, реализующий цикл ввода команд.
// Хранит корень виртуальной файловой системы (root), текущую директорию (cwd)
// и путь (pathParts) как вектор имён директорий для отображения промпта.
// Поддерживает 11 команд: help, exit, ls, cd, cat, touch, mkdir, rm, restore, mv, edit.
class Terminal {
private:
    Directory* root;                    // Корневая директория ("/")
    Directory* cwd;                     // Текущая рабочая директория
    std::vector<std::string> pathParts; // Части пути для отображения промпта
    bool running;                       // Флаг работы цикла терминала

    // Разбирает и выполняет введённую команду
    void executeCommand(const std::string& input);

    // Формирует строку промпта с текущим путём
    std::string getPrompt() const;

    // --- Обработчики команд ---
    void cmdHelp();
    void cmdExit();
    void cmdLs();
    void cmdCd(const std::string& arg);
    void cmdCat(const std::string& arg);
    void cmdTouch(const std::string& arg);
    void cmdMkdir(const std::string& arg);
    void cmdRm(const std::string& arg);
    void cmdRestore(const std::string& arg);
    void cmdMv(const std::string& args);
    void cmdEdit(const std::string& arg);

public:
    Terminal();
    ~Terminal();

    // Запускает основной цикл терминала
    void run();
};

#endif // TERMINAL_H
