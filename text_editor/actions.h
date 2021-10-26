#ifndef TEXT_EDITOR_ACTIONS_H
#define TEXT_EDITOR_ACTIONS_H

#include <iostream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

struct Cursor {
    size_t place = 0;
    size_t line = 0;
};
class TextEditor;

class IAction {
public:
    Cursor crs;

    virtual ~IAction() = default;

    virtual void Do(TextEditor*) = 0;
    virtual void Undo(TextEditor*) = 0;
};

class TypeAction : public IAction {
public:
    char type_me;

    explicit TypeAction(const char&);
    ~TypeAction() override = default;

    void Do(TextEditor*) override;
    void Undo(TextEditor*) override;
};

class DelAction : public IAction {
public:
    char delete_me;

    ~DelAction() override = default;

    void Do(TextEditor*) override;
    void Undo(TextEditor*) override;
};

class NewLineAction : public IAction {
public:
    char el;

    ~NewLineAction() override = default;

    void Do(TextEditor*) override;
    void Undo(TextEditor*) override;
};

#endif  // TEXT_EDITOR_ACTIONS_H
