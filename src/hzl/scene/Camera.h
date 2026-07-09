#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace hzl
{
    class Camera
    {
    public:
        Camera(float aspectRatio, float verticalFovRadians, float nearPlane, float farPlane)
            : m_aspectRatio(aspectRatio),
              m_verticalFovRadians(verticalFovRadians),
              m_nearPlane(nearPlane),
              m_farPlane(farPlane)
        {
        }

        const glm::vec3& position() const
        {
            return m_position;
        }

        void setPosition(const glm::vec3& position)
        {
            m_position = position;
        }

        glm::mat4 viewProjection() const
        {
            const glm::mat4 view = glm::lookAt(
                m_position,
                m_target,
                {0.0f, 1.0f, 0.0f});

            const glm::mat4 projection = glm::perspective(
                m_verticalFovRadians,
                m_aspectRatio,
                m_nearPlane,
                m_farPlane);

            return projection * view;
        }

    private:
        glm::vec3 m_position = {0.0f, 0.0f, 2.0f};
        glm::vec3 m_target = {0.0f, 0.0f, 0.0f};
        float m_aspectRatio = 16.0f / 9.0f;
        float m_verticalFovRadians = 0.7853982f;
        float m_nearPlane = 0.1f;
        float m_farPlane = 100.0f;
    };
}
