#include "SaffronPCH.h"

#include <glad/glad.h>

#include "Saffron/Platform/OpenGL/OpenGLPipeline.h"
#include "Saffron/Rendering/Renderer.h"

namespace Se
{
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float: return GL_FLOAT;
	case ShaderDataType::Float2: return GL_FLOAT;
	case ShaderDataType::Float3: return GL_FLOAT;
	case ShaderDataType::Float4: return GL_FLOAT;
	case ShaderDataType::Mat3: return GL_FLOAT;
	case ShaderDataType::Mat4: return GL_FLOAT;
	case ShaderDataType::Int: return GL_INT;
	case ShaderDataType::Int2: return GL_INT;
	case ShaderDataType::Int3: return GL_INT;
	case ShaderDataType::Int4: return GL_INT;
	case ShaderDataType::Bool: return GL_BOOL;
	}

	SE_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

OpenGLPipeline::OpenGLPipeline(PipelineSpecification specification) :
	m_Specification(Move(specification))
{
	OpenGLPipeline::Invalidate();
}

OpenGLPipeline::~OpenGLPipeline()
{
	GLuint rendererID = m_VertexArrayRendererID;
	Renderer::Submit([rendererID]()
	{
		glDeleteVertexArrays(1, &rendererID);
	});
}

void OpenGLPipeline::Bind()
{
	Shared<OpenGLPipeline> instance = this;
	Renderer::Submit([instance]()
	{
		glBindVertexArray(instance->m_VertexArrayRendererID);

		const auto& layout = instance->m_Specification.Layout;
		Uint32 attribIndex = 0;
		for (const auto& element : layout)
		{
			auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
			glEnableVertexAttribArray(attribIndex);
			if (glBaseType == GL_INT)
			{
				glVertexAttribIPointer(attribIndex, element.GetComponentCount(), glBaseType, layout.GetStride(),
				                       reinterpret_cast<const void*>(static_cast<intptr_t>(element.Offset)));
			}
			else
			{
				glVertexAttribPointer(attribIndex, element.GetComponentCount(), glBaseType,
				                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
				                      reinterpret_cast<const void*>(static_cast<intptr_t>(element.Offset)));
			}
			attribIndex++;
		}
	});
}

void OpenGLPipeline::Invalidate()
{
	SE_CORE_ASSERT(m_Specification.Layout.GetElements().size(), "Layout is empty!");

	Shared<OpenGLPipeline> instance = this;
	Renderer::Submit([instance]() mutable
	{
		auto& vertexArrayRendererID = instance->m_VertexArrayRendererID;

		if (vertexArrayRendererID)
			glDeleteVertexArrays(1, &vertexArrayRendererID);

		glGenVertexArrays(1, &vertexArrayRendererID);
		glBindVertexArray(vertexArrayRendererID);

#if 0
			const auto& layout = instance->m_Specification.Layout;
			Uint32 attribIndex = 0;
			for (const auto& element : layout)
			{
				auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
				glEnableVertexAttribArray(attribIndex);
				if (glBaseType == GL_INT)
				{
					glVertexAttribIPointer(attribIndex,
						element.GetComponentCount(),
						glBaseType,
						layout.GetStride(),
						(const void*)(intptr_t)element.Offset);
				}
				else
				{
					glVertexAttribPointer(attribIndex,
						element.GetComponentCount(),
						glBaseType,
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(intptr_t)element.Offset);
		}
				attribIndex++;
}
#endif
		glBindVertexArray(0);
	});
}

PipelineSpecification& OpenGLPipeline::GetSpecification()
{
	return m_Specification;
}

const PipelineSpecification& OpenGLPipeline::GetSpecification() const
{
	return m_Specification;
}
}
