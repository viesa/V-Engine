#include "Drawable.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Drawable::Drawable(const glm::mat4 &baseTranslation)
        : m_baseTranslation(baseTranslation),
          m_translation(1),
          m_rotation(1),
          m_projection(1)
{
}

void Drawable::Update(const Mouse &mouse)
{

}

void Drawable::Draw(Graphics &gfx)
{
    for (auto &bindable : m_bindables)
    {
        bindable->BindTo(gfx);
    }
    if (m_indexBuffer.has_value())
    {
        gfx.DrawIndexed(m_indexBuffer.value()->GetIndices());
    }
    else
    {
        gfx.DrawArrays();
    }
}

void Drawable::AddBind(std::shared_ptr<Bindable> bindable)
{
    if (dynamic_cast<Bind::IndexBuffer *>(bindable.get()))
    {
        m_indexBuffer = std::make_optional(std::static_pointer_cast<Bind::IndexBuffer>(bindable));
    }
    if (dynamic_cast<Bind::UniformBinder *>(bindable.get()))
    {
        m_uniformBinder = std::make_optional(std::static_pointer_cast<Bind::UniformBinder>(bindable));
    }
    m_bindables.push_back(std::move(bindable));
}

void Drawable::SetTranslation(const glm::vec3 &translate)
{
    m_translation = glm::translate(translate);
}

void Drawable::SetRotation(float yaw, float pitch, float roll)
{
    m_rotation = glm::yawPitchRoll(yaw, pitch, roll);
}

void Drawable::SetProjection(const glm::mat4 &projection)
{
    m_projection = projection;
}

void Drawable::SetExtraColor(const glm::vec3 &color)
{
    m_extraColor = color;
}

void Drawable::Translate(const glm::vec3 &translate)
{
    m_translation *= glm::translate(translate);
}

void Drawable::Rotate(float yaw, float pitch, float roll)
{
    m_rotation *= glm::yawPitchRoll(yaw, pitch, roll);
}
