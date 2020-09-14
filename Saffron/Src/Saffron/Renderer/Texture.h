﻿#pragma once

#include <string>

#include "Saffron/Config.h"
#include "Saffron/Core/Buffer.h"
#include "Saffron/Renderer/RendererAPI.h"

namespace Se
{

class Texture : public RefCounted
{
public:
	enum class Format
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
		Float16 = 3
	};

	enum class Wrap
	{
		None = 0,
		Clamp = 1,
		Repeat = 2
	};

public:
	virtual ~Texture() = default;

	virtual void Bind(Uint32 slot = 0) const = 0;

	virtual Format GetFormat() const = 0;

	virtual Uint32 GetWidth() const = 0;
	virtual Uint32 GetHeight() const = 0;
	virtual Uint32 GetMipLevelCount() const = 0;

	virtual RendererID GetRendererID() const = 0;

	static Uint32 GetBPP(Format format);
	static Uint32 CalculateMipMapCount(Uint32 width, Uint32 height);

	virtual bool operator==(const Texture &other) const = 0;
};

class Texture2D : public Texture
{
public:
	static Ref<Texture2D> Create(Format format, Uint32 width, Uint32 height, Wrap wrap = Wrap::Clamp);
	static Ref<Texture2D> Create(const std::string &path, bool sRGB = false);

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	virtual void Resize(Uint32 width, Uint32 height) = 0;
	virtual Buffer &GetWriteableBuffer() = 0;

	virtual bool Loaded() const = 0;

	virtual const std::string &GetPath() const = 0;
};

class TextureCube : public Texture
{
public:
	static Ref<TextureCube> Create(Format format, Uint32 width, Uint32 height);
	static Ref<TextureCube> Create(const std::string &path);

	virtual const std::string &GetPath() const = 0;
};
}