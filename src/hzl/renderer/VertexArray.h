#pragma once

#include "hzl/renderer/BufferLayout.h"

namespace hzl
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void bind() const;
        void unbind() const;

        void setLayout(const BufferLayout& layout) const;

    private:
        unsigned int m_handle = 0;
    };
}
