#include "text_editor.h"

TypeAction::TypeAction(const char& symbol) {
    type_me = symbol;
}

void TypeAction::Do(TextEditor* editor) {
    std::cout << "TYPE DO START" << std::endl;
    crs = editor->cursor;
    auto start = editor->text[crs.line].begin();
    editor->text[crs.line].insert(start + crs.place, type_me);
    ++editor->cursor.place;
    std::cout << "TYPE DO FINISH" << std::endl;
}

void TypeAction::Undo(TextEditor* editor) {
    std::cout << "TYPE UNDO START" << std::endl;
    editor->cursor = crs;
    auto start = editor->text[crs.line].begin();
    editor->text[crs.line].erase(start + crs.place);
    std::cout << "TYPE UNDO FINISH" << std::endl;
}

void DelAction::Do(TextEditor* editor) {
    std::cout << "DEL DO START" << std::endl;
    crs = editor->cursor;
    delete_me = editor->text[crs.line][crs.place];
    auto start = editor->text[crs.line].begin();
    editor->text[crs.line].erase(start + crs.place);
    if (delete_me == '\n') {
        editor->text[crs.line] += editor->text[crs.line + 1];
        auto start_vec = editor->text.begin();
        editor->text.erase(start_vec + crs.line + 1);
    }
    std::cout << "DEL DO FINISH" << std::endl;
}

void DelAction::Undo(TextEditor* editor) {
    std::cout << "DEL UNDO START" << std::endl;
    editor->cursor = crs;

    if (delete_me == '\n') {
        std::string first_substr, second_substr;
        first_substr = editor->text[crs.line].substr(0, crs.place) + "\n";
        second_substr = editor->text[crs.line].substr(crs.place, editor->text[crs.line].length());

        editor->text[crs.line] = first_substr;
        auto start = editor->text.begin();
        editor->text.insert(start + crs.line + 1, second_substr);
    } else {
        auto start = editor->text[crs.line].begin();
        editor->text[crs.line].insert(start + crs.place + 1, delete_me);
    }
    std::cout << "DEL UNDO FINISH" << std::endl;
}

void NewLineAction::Do(TextEditor* editor) {
    std::cout << "NL DO START" << std::endl;
    crs = editor->cursor;
    el = editor->text[crs.line][crs.place];
    if (el == '\n') {
        auto start = editor->text.begin();
        editor->text.insert(start + crs.line + 1, std::string("\n"));

    } else {
        std::string first_substr, second_substr;
        first_substr = editor->text[crs.line].substr(0, crs.place) + "\n";
        second_substr = editor->text[crs.line].substr(crs.place, editor->text[crs.line].length());

        editor->text[crs.line] = first_substr;
        auto start = editor->text.begin();
        editor->text.insert(start + crs.line + 1, second_substr);
    }
    ++editor->cursor.line;
    editor->cursor.place = 0;
    std::cout << "NL DO FINISH" << std::endl;
}

void NewLineAction::Undo(TextEditor* editor) {
    std::cout << "NL UNDO START" << std::endl;
    editor->cursor = crs;
    editor->text[crs.line].pop_back();
    editor->text[crs.line] += editor->text[crs.line + 1];
    auto start = editor->text.begin();
    editor->text.erase(start + crs.line + 1);
    std::cout << "NL UNDO FINISH" << std::endl;
}
