//
// Created by adrian on 17/05/25.
//

#ifndef RESIZELISTENER_H
#define RESIZELISTENER_H
#include "events/EventListener.h"


class CameraProjection;

class ResizeListener final : public EventListener {
public:

    ResizeListener(GLFWwindow* window, CameraProjection& projection);

    void onEvent(Event& e) override;

private:
    GLFWwindow* m_window;
    CameraProjection& m_projection;
};



#endif //RESIZELISTENER_H
