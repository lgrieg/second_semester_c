#ifndef TEXT_EDITOR_TEXT_EDITOR_H
#define TEXT_EDITOR_TEXT_EDITOR_H

#include "actions.h"

class TextEditor {
public:
    Cursor cursor;

    std::vector<std::string> text{"\n"};
    std::stack<std::unique_ptr<IAction>> stack_do;
    std::stack<std::unique_ptr<IAction>> stack_undo;

    void ShiftLeft();
    void ShiftRight();
    void ShiftUp();
    void ShiftDown();

    void Delete();
    void PasteNewLine();

    void Type(char);

    void Undo();
    void Redo();

    void Print(std::ostream&) const;
    void ClearStack();
};

#endif  // TEXT_EDITOR_TEXT_EDITOR_H
