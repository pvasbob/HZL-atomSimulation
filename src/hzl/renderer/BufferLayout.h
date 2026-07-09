#pragma once

#include <cstddef>
#include <vector>

namespace hzl
{
    struct BufferElement
    {
        unsigned int index = 0;
        int componentCount = 0;
        std::size_t offset = 0;
    };

    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(elements)
        {
            for (const BufferElement& element : m_elements)
            {
                m_stride += static_cast<std::size_t>(element.componentCount) * sizeof(float);
            }
        }

        const std::vector<BufferElement>& elements() const
        {
            return m_elements;
        }

        std::size_t stride() const
        {
            return m_stride;
        }

    private:
        std::vector<BufferElement> m_elements;
        std::size_t m_stride = 0;
    };
}
