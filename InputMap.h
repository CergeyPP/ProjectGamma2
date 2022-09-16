#pragma once
#include "Asset.h"
#include "TEvent.h"

#include <glm/glm.hpp>
#include <set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct InputValue
{
    glm::vec2 axis;
    int action;
    int mods;
};

class InputAction 
{
public:

    virtual ~InputAction(){
    }

    virtual void keyAction(int key, int action, int mods) = 0;
    virtual void mouseButtonAction(int button, int action, int mods) = 0;
    virtual void mouseMovedAction(double xoffset, double yoffset) = 0;
    virtual void scrollAction(double xoffset, double yoffset) = 0;
    virtual void closeAction() = 0;
    
    extend::TEvent<const InputValue&> event;

protected:

    InputAction() {
    }

};

class KeyInputAction : public InputAction {
public:

    KeyInputAction(int key) {
        m_key = key;
    }
    ~KeyInputAction() override{
    }

    void keyAction(int key, int action, int mods) override {
        if (key != m_key) return;
        InputValue value;
        value.action = action;
        value.mods = mods;

        event(value);
    }
    void mouseButtonAction(int button, int action, int mods) override { }
    void mouseMovedAction(double xoffset, double yoffset) override { }
    void scrollAction(double xoffset, double yoffset) override { }
    void closeAction() override { }

private:
    int m_key;
};

class AxisKeyInputAction : public InputAction {
public:

    AxisKeyInputAction(int keyxPositive, int keyxNegative, int keyyPositive, int keyyNegative) {
        keyX[0] = keyxPositive;
        keyX[1] = keyxNegative;
        keyY[0] = keyyPositive;
        keyY[1] = keyyNegative;
    }

    void keyAction(int key, int action, int mods) override {
        if (action == GLFW_REPEAT) return;
        bool continuef = 0;
        for (int i = 0; i < 4; i++) {
            if (key == keyX[i]) continuef = 1;
        }
        if (continuef) {

            switch (action) {
            case GLFW_PRESS:
                if (key == keyX[0]) axis.x += 1;
                if (key == keyX[1]) axis.x -= 1;
                if (key == keyY[0]) axis.y += 1;
                if (key == keyY[1]) axis.y -= 1;
                break;
            case GLFW_RELEASE:
                if (key == keyX[0]) axis.x -= 1;
                if (key == keyX[1]) axis.x += 1;
                if (key == keyY[0]) axis.y -= 1;
                if (key == keyY[1]) axis.y += 1;
            }

            InputValue value;
            value.axis = axis;

            event(value);
        }
    }
    void mouseButtonAction(int button, int action, int mods) override { }
    void mouseMovedAction(double xoffset, double yoffset) override { }
    void scrollAction(double xoffset, double yoffset) override { }
    void closeAction() override { }

private:

    glm::vec2 axis = glm::vec2(0,0);
    int keyX[2];
    int keyY[2];
};

class AxisMouseMovedInputAction : public InputAction{
public:

    AxisMouseMovedInputAction() {}
    ~AxisMouseMovedInputAction() override {}

    void keyAction(int key, int action, int mods) override { }
    void mouseButtonAction(int button, int action, int mods) override { }
    void mouseMovedAction(double xoffset, double yoffset) override { 
        InputValue value;
        value.axis = glm::vec2(xoffset, yoffset);

        event(value);
    }
    void scrollAction(double xoffset, double yoffset) override { }
    void closeAction() override { }
};

class AxisScrollInputAction : public InputAction {
public:

    AxisScrollInputAction() {}
    ~AxisScrollInputAction() override {}

    void keyAction(int key, int action, int mods) override { }
    void mouseButtonAction(int button, int action, int mods) override { }
    void mouseMovedAction(double xoffset, double yoffset) override { }
    void scrollAction(double xoffset, double yoffset) override {
        InputValue value;
        value.axis = glm::vec2(xoffset, yoffset);

        event(value);
    }
    void closeAction() override { }
};

class MouseButtonAction : public InputAction {
public:

    MouseButtonAction(int button) {
        m_button = button;
    }
    ~MouseButtonAction() override {}

    void keyAction(int key, int action, int mods) override { }
    void mouseButtonAction(int button, int action, int mods) override { 
        if (m_button != button) return;
        InputValue value;
        value.action = action;
        value.mods = mods;

        event(value);
    }
    void mouseMovedAction(double xoffset, double yoffset) override { }
    void scrollAction(double xoffset, double yoffset) override { }
    void closeAction() override { }
private:

    int m_button;
};

class InputMap :
    public IAsset
{
public:

    ~InputMap(); 

    void load(const std::string& filePath) override;

    void broadcastKeyEvent(int key, int action, int mods);
    void broadcastMouseButtonEvent(int button, int action, int mods);
    void broadcastMouseMovedEvent(double xoffset, double yoffset);
    void broadcastScrollEvent(double xoffset, double yoffset);
    void broadcastCloseEvent();

    std::map<const std::string, InputAction*> actionMap;
};