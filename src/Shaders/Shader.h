#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "Bindable.h"
#include "FileIO.h"
#include "NonCopyable.h"

class Shader : public FileIO, NonCopyable
{
protected:
    enum Type
    {
        Vertex,
        Geometry,
        Fragment
    };

public:
    explicit Shader(Type shaderType);
    Shader(Type shaderType, const std::string &filepath);
    ~Shader();
    Shader(Shader&& other) noexcept;

    bool LoadFromFile(const std::string &path) override;

    [[nodiscard]] const std::string &GetName() const noexcept;
    [[nodiscard]] unsigned int GetShaderID() const noexcept;

private:
    void ConfigureName() noexcept;
    static GLenum MapShaderType(Type type) noexcept;

private:
    Type m_type;
    unsigned int m_shaderID;
    std::string m_name;
};