#include <glm/glm.hpp>
#include <vector>

class Cube
{
    glm::vec3 position, scale, rotation, color;
    std::vector<float> vertices;

    void create_vertices()
    {
        // Define the vertices for a cube centered at the origin (0, 0, 0)
        // Vertices are adjusted based on position, scale, and rotation
        float halfSizeX = scale.x / 2.0f;
        float halfSizeY = scale.y / 2.0f;
        float halfSizeZ = scale.z / 2.0f;

        vertices = {
            // Front face
            position.x - halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x + halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x - halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x - halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Bottom-left

            // Back face
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x + halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x + halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x + halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x - halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left

            // Left face
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x - halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x - halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x - halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x - halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left

            // Right face
            position.x + halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x + halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x + halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x + halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left

            // Top face
            position.x - halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x + halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x + halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x - halfSizeX, position.y + halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x - halfSizeX, position.y + halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left

            // Bottom face
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
            position.x + halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-right
            position.x + halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right

            position.x + halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-right
            position.x - halfSizeX, position.y - halfSizeY, position.z + halfSizeZ, color.r, color.g, color.b, // Top-left
            position.x - halfSizeX, position.y - halfSizeY, position.z - halfSizeZ, color.r, color.g, color.b, // Bottom-left
        };
    }

public:
    // Constructor
    Cube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color)
        : position(position), scale(scale), rotation(rotation), color(color)
    {
        create_vertices();
    }

    // Getter methods
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_scale() const { return scale; }
    glm::vec3 get_rotation() const { return rotation; }
    glm::vec3 get_color() const { return color; }
    const std::vector<float> &get_vertices() const { return vertices; }

    void set_position(const glm::vec3 &new_position)
    {
        position = new_position;
        create_vertices();
    }
    void set_scale(const glm::vec3 &new_scale)
    {
        scale = new_scale;
        create_vertices();
    }
    void set_rotation(const glm::vec3 &new_rotation)
    {
        rotation = new_rotation;
        create_vertices();
    }
    void set_color(const glm::vec3 &new_color)
    {
        color = new_color;
        create_vertices();
    }
};
