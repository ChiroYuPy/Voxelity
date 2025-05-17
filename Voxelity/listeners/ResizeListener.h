//
// Created by adrian on 17/05/25.
//

#ifndef RESIZELISTENER_H
#define RESIZELISTENER_H
#include "events/EventListener.h"


class ResizeListener final : public EventListener {
public:

    ResizeListener(GLFWwindow* window, glm::mat4& projection);

    void onEvent(Event& e) override;

private:
    GLFWwindow* m_window;
    glm::mat4& m_projection;
};



#endif //RESIZELISTENER_H
