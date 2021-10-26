#include "text_editor.h"

void TextEditor::ShiftLeft() {
    if (cursor.line == 0 && cursor.place == 0) {
        return;
    } else if (cursor.place != 0) {
        --cursor.place;
    } else {
        cursor.place = text[cursor.line - 1].length() - 1;
        --cursor.line;
    }
}

void TextEditor::ShiftRight() {
    if (cursor.line == text.size() - 1 && cursor.place == text[cursor.line].length() - 1) {
        return;
    } else if (cursor.place != text[cursor.line].length() - 1) {
        ++cursor.place;
    } else {
        cursor.place = 0;
        ++cursor.line;
    }
}

void TextEditor::ShiftUp() {
    if (cursor.line == 0) {
        return;
    } else if (text[cursor.line].length() > text[cursor.line - 1].length()) {
        cursor.place = text[cursor.line - 1].length() - 1;
        --cursor.line;
    } else {
        --cursor.line;
    }
}

void TextEditor::ShiftDown() {
    if (cursor.line == text.size() - 1) {
        return;
    } else if (text[cursor.line].length() > text[cursor.line + 1].length()) {
        cursor.place = text[cursor.line + 1].length() - 1;
        ++cursor.line;
    } else {
        ++cursor.line;
    }
}

void TextEditor::PasteNewLine() {
    ClearStack();
    auto action = std::make_unique<NewLineAction>();
    action->Do(this);
    stack_do.push(std::move(action));
}

void TextEditor::Delete() {
    // std::cout << "DEL func DO START" << std::endl;
    //   std::cout << "cursor line: " << cursor.line << std::endl;
    //   std::cout << "cursor place: " << cursor.place << std::endl;
    //    std::cout << "text size: " << text.size() << std::endl;
    //    std::cout << "string size: " << text[cursor.line].size() << std::endl;
    if (cursor.line == text.size() - 1 && cursor.place == text[cursor.line].size() - 1) {
        //    std::cout << "DEL func finish DO START" << std::endl;
        return;
    }
    ClearStack();
    auto action = std::make_unique<DelAction>();
    action->Do(this);
    stack_do.push(std::move(action));
    // std::cout << "DEL func 2 finish DO START" << std::endl;
}

void TextEditor::Type(char symbol) {
    ClearStack();
    auto action = std::make_unique<TypeAction>(symbol);
    action->Do(this);
    stack_do.push(std::move(action));
}

void TextEditor::Undo() {
    stack_undo.push(std::move(stack_do.top()));
    stack_do.pop();
    stack_undo.top()->Undo(this);
}

void TextEditor::Redo() {
    if (stack_undo.empty()) {
        return;
    } else {
        stack_do.push(std::move(stack_undo.top()));
        stack_undo.pop();
        stack_do.top()->Do(this);
    }
}

void TextEditor::Print(std::ostream& os) const {
    std::string print_mipt;
    for (size_t i = 0; i < text.size(); ++i) {
        print_mipt += text[i];
    }
    print_mipt.pop_back();
    os << print_mipt;
}

void TextEditor::ClearStack() {
    while (!stack_undo.empty()) {
        stack_undo.pop();
    }
}
