#pragma once

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

        void addFloatAttribute(
            unsigned int index,
            int componentCount,
            int strideInBytes,
            const void* offset) const;

    private:
        unsigned int m_handle = 0;
    };
}
