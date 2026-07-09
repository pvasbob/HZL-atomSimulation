#pragma once

#include <cstddef>

namespace hzl
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int* indices, std::size_t count);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        void bind() const;
        void unbind() const;

        std::size_t count() const;

    private:
        unsigned int m_handle = 0;
        std::size_t m_count = 0;
    };
}
