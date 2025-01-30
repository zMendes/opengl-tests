#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Cube
{
    glm::vec3 init_position, position, scale, rotation, color, ambient, diffuse, specular;
    float shininess;
    std::vector<float> vertices;

    void create_vertices()
    {
        vertices = {
            // Positions              // Normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    }

public:
    // Constructor
    Cube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color)
        : position(position), scale(scale), rotation(rotation), color(color)
    {
        init_position = position;
        ambient = glm::vec3(0.25f, 0.f, 0.f);
        diffuse = glm::vec3(1.0f, 0.f, 0.f);
        specular = glm::vec3(0.5f, 0.5f, 0.5f);
        shininess = 32.0f;
        create_vertices();
    }
    Cube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        : position(position), scale(scale), rotation(rotation), color(color), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {
        create_vertices();
    }

    // Getter methods
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_scale() const { return scale; }
    glm::vec3 get_rotation() const { return rotation; }
    glm::vec3 get_color() const { return color; }
    glm::vec3 get_ambient() const { return ambient; }
    glm::vec3 get_diffuse() const { return diffuse; }
    glm::vec3 get_specular() const { return specular; }
    float get_shininess() const { return shininess; }
    const std::vector<float> &get_vertices() const { return vertices; }
    glm::mat4 get_model_matrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void set_position(const glm::vec3 &new_position)
    {
        position = new_position;
    }
    void set_scale(const glm::vec3 &new_scale)
    {
        scale = new_scale;
    }
    void set_rotation(const glm::vec3 &new_rotation)
    {
        rotation = new_rotation;
    }
    void set_color(const glm::vec3 &new_color)
    {
        color = new_color;
        create_vertices();
    }

    void translate(const glm::vec3 &translation)
    {
        position += translation;
    }
    void rotate(const glm::vec3 &rotation)
    {
        this->rotation += rotation;
    }
    void reset_position()
    {
        position = init_position;
    }
};
class FacelessCube
{
    glm::vec3 init_position, position, scale, rotation, color, ambient, diffuse, specular;
    float shininess;
    std::vector<float> vertices;

    void create_vertices()
    {
        vertices = {
            // Positions              // Normals
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    }

public:
    // Constructor
    FacelessCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color)
        : position(position), scale(scale), rotation(rotation), color(color)
    {
        init_position = position;
        ambient = glm::vec3(0.25f, 0.f, 0.f);
        diffuse = glm::vec3(1.0f, 0.f, 0.f);
        specular = glm::vec3(0.5f, 0.5f, 0.5f);
        shininess = 32.0f;
        create_vertices();
    }
    FacelessCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        : position(position), scale(scale), rotation(rotation), color(color), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {
        create_vertices();
    }

    // Getter methods
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_scale() const { return scale; }
    glm::vec3 get_rotation() const { return rotation; }
    glm::vec3 get_color() const { return color; }
    glm::vec3 get_ambient() const { return ambient; }
    glm::vec3 get_diffuse() const { return diffuse; }
    glm::vec3 get_specular() const { return specular; }
    float get_shininess() const { return shininess; }
    const std::vector<float> &get_vertices() const { return vertices; }
    glm::mat4 get_model_matrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void set_position(const glm::vec3 &new_position)
    {
        position = new_position;
    }
    void set_scale(const glm::vec3 &new_scale)
    {
        scale = new_scale;
    }
    void set_rotation(const glm::vec3 &new_rotation)
    {
        rotation = new_rotation;
    }
    void set_color(const glm::vec3 &new_color)
    {
        color = new_color;
        create_vertices();
    }

    void translate(const glm::vec3 &translation)
    {
        position += translation;
    }
    void rotate(const glm::vec3 &rotation)
    {
        this->rotation += rotation;
    }
    void reset_position()
    {
        position = init_position;
    }
};
