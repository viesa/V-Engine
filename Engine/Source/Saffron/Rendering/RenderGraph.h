﻿#pragma once

#include "Saffron/Base.h"
#include "Saffron/Rendering/Stream/Input.h"
#include "Saffron/Rendering/Stream/Output.h"

namespace Se
{
class RenderPass;
class Framebuffer;

class RenderGraph
{
public:
	RenderGraph();
	virtual ~RenderGraph() = default;

	virtual void Execute() = 0;

	void OnViewportResize(uint width, uint height);

	void AddPass(Unique<RenderPass> pass);
	void AddInput(const Stream::Input& input);
	void AddOutput(const Stream::Output& output);

	void AddGlobalInput(Unique<Stream::Input> input);
	void AddGlobalOutput(Unique<Stream::Output> output);
	void SetGlobalStreamLinkage(const String& outputName, const String& inputName);
	
	const Shared<Framebuffer>& GetOutput() const;
	const RenderPass& GetRenderPass(const String& name) const;
	const RenderPass& GetFinalRenderPass() const;

	void LinkGlobalInputs();

private:
	void LinkInputs(const RenderPass& renderPass);

protected:
	List<Unique<RenderPass>> _passes;

	List<Stream::Input> _inputs;
	List<Stream::Output> _outputs;
	List<Unique<Stream::Input>> _globalInputs;
	List<Unique<Stream::Output>> _globalOutputs;
	
	Shared<Framebuffer> _backBuffer;
};
}
