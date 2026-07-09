#include "hzl/renderer/Renderer.h"

#include <glad/gl.h>

namespace hzl
{
    void Renderer::beginFrame()
    {
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::endFrame()
    {
    }
}
