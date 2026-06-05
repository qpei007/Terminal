#include "Terminal.h"
#include "SingleLineFile.h"
#include "MultiLineFile.h"
#include <iostream>
#include <sstream>

// Конструктор: создаёт корневую директорию 
// и устанавливает cwd в корень
Terminal::Terminal() {
    root = new Directory("/");
    cwd = root;
    running = true;
}

// Деструктор: удаляет корневую директорию, 
// что рекурсивно освобождает всё дерево 
// через Directory::~Directory()
Terminal::~Terminal() {
    delete root;
}

// Формирует строку промпта вида "/path/dir> "
// Текущий путь хранится как вектор имён 
// директорий (pathParts).
// Авторитативное решение: путь хранится через 
// vector<string>, а не через вычисление из 
// цепочки parent-указателей.
std::string Terminal::getPrompt() const {
    std::string path = "/";
    for (const std::string& part : pathParts) {
        path += part + "/";
    }
    return path + "> ";
}

// Основной цикл терминала: читает строки и 
// передаёт их в executeCommand. Завершается 
// при вводе exit или EOF (Ctrl+D).
void Terminal::run() {
    std::cout << "==========================="
        << "=============" << std::endl;
    std::cout << "    Virtual Terminal v1.0  "
        << "              " << std::endl;
    std::cout << "==========================="
        << "=============" << std::endl;
    std::cout << "Type 'help' for a list of "
        << "commands." << std::endl;
    std::cout << std::endl;

    std::string input;
    while (running) {
        std::cout << getPrompt();
        if (!std::getline(std::cin, input)) {
            // EOF — завершаем работу
            std::cout << std::endl;
            break;
        }

        // Пропускаем пустой ввод
        // Убираем ведущие и замыкающие пробелы
        size_t start = input.find_first_not_of(
            " \t"
        );

        if (start == std::string::npos) {
            // Строка пустая или только пробелы
            continue;
        }

        size_t end = input.find_last_not_of(" \t");
        input = input.substr(
            start,
            end - start + 1
        );

        executeCommand(input);
    }
}

// Разбирает введённую строку на команду и 
// аргументы, затем вызывает соответствующий 
// обработчик. Диспетчеризация команд через 
// if-else (это единственное место, где 
// допустимы проверки строковых значений).
void Terminal::executeCommand(
    const std::string& input
) {
    // Разделяем ввод на команду и аргументы
    size_t spacePos = input.find(' ');
    std::string command;
    std::string args;

    if (spacePos != std::string::npos) {
        command = input.substr(0, spacePos);
        // Пропускаем пробелы после команды
        size_t argStart = input.find_first_not_of(
            ' ',
            spacePos
        );
        if (argStart != std::string::npos) {
            args = input.substr(argStart);
        }
    }
    else {
        command = input;
    }

    // Диспетчеризация команд
    if (command == "help") {
        cmdHelp();
    }
    else if (command == "exit") {
        cmdExit();
    }
    else if (command == "ls") {
        cmdLs();
    }
    else if (command == "cd") {
        cmdCd(args);
    }
    else if (command == "cat") {
        cmdCat(args);
    }
    else if (command == "touch") {
        cmdTouch(args);
    }
    else if (command == "mkdir") {
        cmdMkdir(args);
    }
    else if (command == "rm") {
        cmdRm(args);
    }
    else if (command == "restore") {
        cmdRestore(args);
    }
    else if (command == "mv") {
        cmdMv(args);
    }
    else if (command == "edit") {
        cmdEdit(args);
    }
    else {
        std::cout << "Unknown command: '"
            << command << "'"
            << std::endl;
        std::cout << "Type 'help' for a list "
            << "of available commands."
            << std::endl;
    }
}

// ============================================
// Реализация команд 
// ============================================

// help — выводит список доступных команд
void Terminal::cmdHelp() {
    std::cout << "Available commands:"
        << std::endl;
    std::cout << "  help     - Show this help "
        << "message" << std::endl;
    std::cout << "  exit     - Exit the "
        << "terminal" << std::endl;
    std::cout << "  ls       - List contents "
        << "of current directory"
        << std::endl;
    std::cout << "  cd <dir> - Change "
        << "directory (use '..' to go "
        << "up, '/' for root)"
        << std::endl;
    std::cout << "  cat <f>  - Display file "
        << "contents" << std::endl;
    std::cout << "  touch <f>- Create a new "
        << "file" << std::endl;
    std::cout << "  mkdir <d>- Create a new "
        << "directory" << std::endl;
    std::cout << "  rm <n>   - Remove (hide) "
        << "a file or directory"
        << std::endl;
    std::cout << "  restore  - Restore a "
        << "hidden entry (no args = "
        << "show hidden list)"
        << std::endl;
    std::cout << "  mv <o> <n>- Rename a "
        << "file or directory"
        << std::endl;
    std::cout << "  edit <f> - Edit file "
        << "contents" << std::endl;
}

// exit — завершает цикл терминала
void Terminal::cmdExit() {
    std::cout << "Goodbye!" << std::endl;
    running = false;
}

// ls — отображает содержимое текущей директории.
// Вызывает полиморфный метод cwd->display(), 
// который фильтрует скрытые сущности и выводит 
// displayName() каждого видимого потомка.
void Terminal::cmdLs() {
    cwd->display();
}

// cd — переход в другую директорию.
// Поддерживает: cd <имя>, cd .., cd /
// Попытка подняться выше корня обрабатывается 
// отдельно.
void Terminal::cmdCd(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: cd <directory>"
            << std::endl;
        return;
    }

    // Переход в корень
    if (arg == "/") {
        cwd = root;
        pathParts.clear();
        return;
    }

    // Переход на уровень вверх
    if (arg == "..") {
        if (cwd == root) {
            std::cout << "Already at root "
                << "directory."
                << std::endl;
            return;
        }
        // getParent() возвращает Directory* // (родитель всегда директория),
        // asDirectory() приводит Entry* к 
        // Directory*
        Directory* parentDir = cwd->getParent();
        if (parentDir != nullptr) {
            cwd = parentDir;
            if (!pathParts.empty()) {
                pathParts.pop_back();
            }
        }
        return;
    }

    // Переход в дочернюю директорию
    Entry* child = cwd->findChild(arg);
    if (child == nullptr) {
        std::cout << "Not found: '" << arg
            << "'" << std::endl;
        return;
    }

    if (!child->isDirectory()) {
        std::cout << "'" << arg
            << "' is not a directory."
            << std::endl;
        return;
    }

    cwd = child->asDirectory();
    pathParts.push_back(arg);
}

// cat — показывает содержимое файла.
// Для файлов вызывает полиморфный 
// entry->display().
// Для директорий выводит ошибку.
void Terminal::cmdCat(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: cat <file>"
            << std::endl;
        return;
    }

    Entry* entry = cwd->findChild(arg);
    if (entry == nullptr) {
        std::cout << "File not found: '" << arg
            << "'" << std::endl;
        return;
    }

    if (entry->isDirectory()) {
        std::cout << "'" << arg << "' is a "
            << "directory. Use 'ls' "
            << "to view its contents."
            << std::endl;
        return;
    }

    // Полиморфный вызов: 
    // SingleLineFile::display() или 
    // MultiLineFile::display()
    entry->display();
}

// touch — создаёт новый файл в текущей 
// директории. Пользователь выбирает тип файла 
// (однострочный или многострочный).
// Проверяется уникальность имени 
// (включая скрытые сущности).
void Terminal::cmdTouch(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: touch <filename>"
            << std::endl;
        return;
    }

    // Проверяем, не существует ли сущность с 
    // таким именем (includeHidden = true, чтобы 
    // избежать конфликта имён со скрытыми)
    Entry* existing = cwd->findChild(arg, true);
    if (existing != nullptr) {
        std::cout << "Entry '" << arg
            << "' already exists."
            << std::endl;
        return;
    }

    // Предлагаем выбрать тип файла
    std::cout << "Select file type:" << std::endl;
    std::cout << "  1 - Single-line file"
        << std::endl;
    std::cout << "  2 - Multi-line file"
        << std::endl;
    std::cout << "Choice (1/2): ";

    std::string choice;
    std::getline(std::cin, choice);

    Entry* newFile = nullptr;
    if (choice == "2") {
        newFile = new MultiLineFile(arg, cwd);
    }
    else {
        // По умолчанию создаём однострочный файл
        if (choice != "1") {
            std::cout << "Invalid choice. "
                << "Creating single-line "
                << "file by default."
                << std::endl;
        }
        newFile = new SingleLineFile(arg, cwd);
    }

    cwd->addChild(newFile);
    std::cout << "Created " << newFile->getType()
        << ": '" << arg << "'"
        << std::endl;
}

// mkdir — создаёт новую директорию в текущей 
// директории. Проверяется уникальность имени 
// (включая скрытые).
void Terminal::cmdMkdir(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: mkdir <dirname>"
            << std::endl;
        return;
    }

    Entry* existing = cwd->findChild(arg, true);
    if (existing != nullptr) {
        std::cout << "Entry '" << arg
            << "' already exists."
            << std::endl;
        return;
    }

    Directory* newDir = new Directory(arg, cwd);
    cwd->addChild(newDir);
    std::cout << "Created directory: '" << arg
        << "'" << std::endl;
}

// rm — мягкое удаление: скрывает сущность 
// (устанавливает hidden = true).
// Сущность остаётся в дереве, но не 
// отображается в ls.
// Можно восстановить через restore.
void Terminal::cmdRm(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: rm <name>"
            << std::endl;
        return;
    }

    Entry* entry = cwd->findChild(arg);
    if (entry == nullptr) {
        std::cout << "Entry not found: '" << arg
            << "'" << std::endl;
        return;
    }

    entry->hide();
    std::cout << "Removed (hidden): '" << arg
        << "'" << std::endl;
}

// restore — восстанавливает скрытую сущность.
// Без аргументов: показывает список скрытых 
// сущностей в текущей директории.
// С аргументом: восстанавливает указанную.
void Terminal::cmdRestore(
    const std::string& arg
) {
    if (arg.empty()) {
        // Показываем список скрытых сущностей 
        // для удобства пользователя
        std::cout << "Hidden entries in "
            << "current directory:"
            << std::endl;
        bool hasHidden = false;

        for (const Entry* child :
            cwd->getChildren()) {

            if (child->isHidden()) {
                std::cout << "  "
                    << child->displayName()
                    << " ["
                    << child->getType()
                    << "]" << std::endl;
                hasHidden = true;
            }
        }
        if (!hasHidden) {
            std::cout << "  (no hidden entries)"
                << std::endl;
        }
        std::cout << "Usage: restore <name>"
            << std::endl;
        return;
    }

    // Ищем сущность с учётом скрытых 
    // (includeHidden = true)
    Entry* entry = cwd->findChild(arg, true);
    if (entry == nullptr) {
        std::cout << "Entry not found: '" << arg
            << "'" << std::endl;
        return;
    }

    if (!entry->isHidden()) {
        std::cout << "'" << arg
            << "' is not hidden."
            << std::endl;
        return;
    }

    entry->restore();
    std::cout << "Restored: '" << arg << "'"
        << std::endl;
}

// mv — переименовывает файл или директорию.
// Принимает два аргумента: старое имя и 
// новое имя. Проверяет уникальность нового 
// имени (включая скрытые).
void Terminal::cmdMv(const std::string& args) {
    // Разбираем два аргумента: 
    // <old_name> <new_name>
    std::istringstream iss(args);
    std::string oldName, newName;
    iss >> oldName >> newName;

    if (oldName.empty() || newName.empty()) {
        std::cout << "Usage: mv <old_name> "
            << "<new_name>" << std::endl;
        return;
    }

    Entry* entry = cwd->findChild(oldName);
    if (entry == nullptr) {
        std::cout << "Entry not found: '"
            << oldName << "'"
            << std::endl;
        return;
    }

    // Проверяем, что новое имя не занято
    Entry* existing = cwd->findChild(
        newName,
        true
    );

    if (existing != nullptr) {
        std::cout << "Entry '" << newName
            << "' already exists."
            << std::endl;
        return;
    }

    entry->setName(newName);
    std::cout << "Renamed: '" << oldName
        << "' -> '" << newName << "'"
        << std::endl;
}

// edit — редактирует содержимое файла.
// Вызывает полиморфный метод entry->edit().
// Для SingleLineFile: запрашивает одну строку.
// Для MultiLineFile: запрашивает несколько 
// строк (до ":wq").
// Для Directory: выводит ошибку (через 
// Directory::edit()).
// Terminal НЕ проверяет конкретный тип — 
// полиморфизм делает всё сам.
void Terminal::cmdEdit(const std::string& arg) {
    if (arg.empty()) {
        std::cout << "Usage: edit <file>"
            << std::endl;
        return;
    }

    Entry* entry = cwd->findChild(arg);
    if (entry == nullptr) {
        std::cout << "File not found: '" << arg
            << "'" << std::endl;
        return;
    }

    // Полиморфный вызов: конкретный тип 
    // определяет поведение. Это главная 
    // демонстрация полиморфизма в программе.
    entry->edit();
}