#include "SaffronPCH.h"

#include <string>

#include "Saffron/Renderer/Renderer.h"
#include "Saffron/Renderer/Texture.h"
#include "Saffron/Platform/OpenGL/OpenGLTexture.h"

namespace Se
{

Uint32 Texture::GetBPP(Format format)
{
	switch ( format )
	{
	case Format::RGB:    return 3;
	case Format::RGBA:   return 4;
	}
	return 0;
}

Uint32 Texture::CalculateMipMapCount(Uint32 width, Uint32 height)
{
	Uint32 levels = 1;
	while ( (width | height) >> levels )
		levels++;

	return levels;
}

Shared<Texture2D> Texture2D::Create(Format format, Uint32 width, Uint32 height, Wrap wrap)
{
	switch ( RendererAPI::Current() )
	{
	case RendererAPI::Type::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::Type::OpenGL:	return Shared<OpenGLTexture2D>::Create(format, width, height, wrap);
	default:						SE_CORE_ASSERT(false, "Unknown RendererAPI"); return nullptr;
	}
}

Shared<Texture2D> Texture2D::Create(const std::string &path, bool sRGB)
{
	switch ( RendererAPI::Current() )
	{
	case RendererAPI::Type::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::Type::OpenGL:	return Shared<OpenGLTexture2D>::Create(path, sRGB);
	default:						SE_CORE_ASSERT(false, "Unknown RendererAPI"); return nullptr;
	}
}

Shared<TextureCube> TextureCube::Create(Format format, Uint32 width, Uint32 height)
{
	switch ( RendererAPI::Current() )
	{
	case RendererAPI::Type::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::Type::OpenGL: return Shared<OpenGLTextureCube>::Create(format, width, height);
	default:						SE_CORE_ASSERT(false, "Unknown RendererAPI"); return nullptr;
	}
}

Shared<TextureCube> TextureCube::Create(const std::string &path)
{
	switch ( RendererAPI::Current() )
	{
	case RendererAPI::Type::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::Type::OpenGL: return Shared<OpenGLTextureCube>::Create(path);
	default:						SE_CORE_ASSERT(false, "Unknown RendererAPI"); return nullptr;
	}
}
}
