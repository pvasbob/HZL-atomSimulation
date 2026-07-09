#include "hzl/renderer/MeshFactory.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace hzl
{
    namespace
    {
        constexpr float pi = 3.14159265358979323846f;

        BufferLayout positionColorLayout()
        {
            return {
                {0, 3, 0},
                {1, 3, 3 * sizeof(float)},
            };
        }
    }

    std::unique_ptr<Mesh> MeshFactory::createCube()
    {
        constexpr std::array<float, 48> vertices = {
            // position             // color
            -0.5f, -0.5f, -0.5f,    0.95f, 0.35f, 0.25f,
             0.5f, -0.5f, -0.5f,    0.20f, 0.75f, 0.45f,
             0.5f,  0.5f, -0.5f,    0.25f, 0.45f, 0.95f,
            -0.5f,  0.5f, -0.5f,    0.95f, 0.85f, 0.25f,
            -0.5f, -0.5f,  0.5f,    0.95f, 0.25f, 0.65f,
             0.5f, -0.5f,  0.5f,    0.25f, 0.85f, 0.85f,
             0.5f,  0.5f,  0.5f,    0.80f, 0.45f, 0.95f,
            -0.5f,  0.5f,  0.5f,    0.55f, 0.95f, 0.35f,
        };

        constexpr std::array<unsigned int, 36> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0,
        };

        return std::make_unique<Mesh>(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size(),
            positionColorLayout());
    }

    std::unique_ptr<Mesh> MeshFactory::createSphere(
        float radius,
        unsigned int latitudeSegments,
        unsigned int longitudeSegments)
    {
        latitudeSegments = std::max(latitudeSegments, 3u);
        longitudeSegments = std::max(longitudeSegments, 3u);

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(
            static_cast<std::size_t>(latitudeSegments + 1)
            * static_cast<std::size_t>(longitudeSegments + 1)
            * 6);

        for (unsigned int latitude = 0; latitude <= latitudeSegments; ++latitude)
        {
            const float v = static_cast<float>(latitude) / static_cast<float>(latitudeSegments);
            const float theta = v * pi;
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            for (unsigned int longitude = 0; longitude <= longitudeSegments; ++longitude)
            {
                const float u = static_cast<float>(longitude) / static_cast<float>(longitudeSegments);
                const float phi = u * 2.0f * pi;
                const float sinPhi = std::sin(phi);
                const float cosPhi = std::cos(phi);

                const float x = radius * sinTheta * cosPhi;
                const float y = radius * cosTheta;
                const float z = radius * sinTheta * sinPhi;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                vertices.push_back(0.30f + 0.70f * u);
                vertices.push_back(0.45f + 0.35f * (1.0f - v));
                vertices.push_back(0.85f);
            }
        }

        indices.reserve(
            static_cast<std::size_t>(latitudeSegments)
            * static_cast<std::size_t>(longitudeSegments)
            * 6);

        for (unsigned int latitude = 0; latitude < latitudeSegments; ++latitude)
        {
            for (unsigned int longitude = 0; longitude < longitudeSegments; ++longitude)
            {
                const unsigned int current = latitude * (longitudeSegments + 1) + longitude;
                const unsigned int next = current + longitudeSegments + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        return std::make_unique<Mesh>(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size(),
            positionColorLayout());
    }
}
