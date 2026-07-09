#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace hzl
{
    struct Transform
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        glm::mat4 matrix() const
        {
            glm::mat4 transform(1.0f);
            transform = glm::translate(transform, position);
            transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
            transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
            transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
            transform = glm::scale(transform, scale);

            return transform;
        }
    };
}
