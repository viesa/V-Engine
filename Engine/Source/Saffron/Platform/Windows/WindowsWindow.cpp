#include "SaffronPCH.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Saffron/Core/Events/KeyboardEvent.h"
#include "Saffron/Core/Events/MouseEvent.h"
#include "Saffron/Core/Events/WindowEvent.h"
#include "Saffron/Core/Run.h"
#include "Saffron/Core/ScopedLock.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Platform/Windows/WindowsWindow.h"
#include "Saffron/Rendering/Renderer.h"

namespace Se
{
static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
	Log::CoreError("GLFW Error ({0}): {1}", error, description);
}

WindowsWindow::WindowsWindow(const Properties& props) :
	Window(props),
	_nativeWindow(nullptr),
	_vsync(false)
{
	static Mutex mutex;
	ScopedLock lock(mutex);

	// Initialize GLFW
	if (!s_GLFWInitialized)
	{
		const int success = glfwInit();
		Debug::Assert(success, "Failed to initialize GLFW");
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	// Create GLFW Window
	{
#ifdef SE_DEBUG
		if (RendererApi::Current() == RendererApiType::OpenGL) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		_nativeWindow = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), _title.c_str(),
		                                 nullptr, nullptr);
		glfwMakeContextCurrent(_nativeWindow);
	}

	// Initialize glad
	{
		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		Debug::Assert(status, "Failed to initialize Glad");
	}

	Log::CoreInfo("Creating Window \"{0}\" ({1:d}x{2:d})", _title, _width, _height);
		
	Profiler::BeginSession("Test", "results.json");
	SetupGLFWCallbacks();
	Profiler::EndSession();

	// Initialization events
	{
		double x, y;
		glfwGetCursorPos(_nativeWindow, &x, &y);
		PushEvent<MouseMovedEvent>(Vector2(x, y));
	}
	{
		glfwSetWindowPos(_nativeWindow, static_cast<int>(_position.x), static_cast<int>(_position.y));
	}

	WindowsWindow::SetVSync(false);

	_imGuiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	_imGuiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	_imGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	_imGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	_imGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	_imGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	_imGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	_imGuiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	Resized += SE_EV_FUNCTION(WindowsWindow::OnResize);
	Moved += SE_EV_FUNCTION(WindowsWindow::OnMove);
	GainedFocus += SE_EV_FUNCTION(WindowsWindow::OnGainFocus);
	LostFocus += SE_EV_FUNCTION(WindowsWindow::OnLostFocus);
	Closed += SE_EV_FUNCTION(WindowsWindow::OnClose);
	NewTitle += SE_EV_FUNCTION(WindowsWindow::OnNewTitle);
	NewIcon += SE_EV_FUNCTION(WindowsWindow::OnNewIcon);
}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(WindowsWindow::GetNativeWindow()));
}

void WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(_nativeWindow);

	const ImGuiMouseCursor ImGuiCursor = ImGui::GetMouseCursor();
	glfwSetCursor(_nativeWindow, _imGuiMouseCursors[ImGuiCursor]
		                             ? _imGuiMouseCursors[ImGuiCursor]
		                             : _imGuiMouseCursors[ImGuiMouseCursor_Arrow]);
	glfwSetInputMode(_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WindowsWindow::OnEvent(const Event& event)
{
}

void WindowsWindow::Close()
{
	glfwSetWindowShouldClose(_nativeWindow, GLFW_TRUE);
}

void WindowsWindow::Focus()
{
	glfwRequestWindowAttention(_nativeWindow);
}

void* WindowsWindow::GetNativeWindow() const
{
	return _nativeWindow;
}

void WindowsWindow::SetVSync(bool enabled)
{
	glfwSwapInterval(static_cast<int>(_vsync = enabled));
}

bool WindowsWindow::IsVSync() const
{
	return _vsync;
}

void WindowsWindow::Minimize()
{
	Run::Later([this]
	{
		if (_maximized)
		{
			Restore();
			glfwIconifyWindow(_nativeWindow);
			_minimized = true;
		}
		else if (_minimized)
		{
			Restore();
		}
		else
		{
			glfwIconifyWindow(_nativeWindow);
			_minimized = true;
		}
	});
}

void WindowsWindow::Maximize()
{
	Run::Later([this]
	{
		if (_minimized)
		{
			Restore();
			glfwMaximizeWindow(_nativeWindow);
			_maximized = true;
		}
		else if (_maximized)
		{
			Restore();
		}
		else
		{
			glfwMaximizeWindow(_nativeWindow);
			_maximized = true;
		}
	});
}

bool WindowsWindow::IsMinimized() const
{
	return _minimized;
}

bool WindowsWindow::IsMaximized() const
{
	return _maximized;
}

bool WindowsWindow::OnResize(const WindowResizedEvent& event)
{
	_width = event.GetWidth();
	_height = event.GetHeight();

	auto* instance = this;
	Renderer::Submit([=]() { glViewport(0, 0, instance->_width, instance->_height); });
	auto& fbs = FramebufferPool::GetGlobal()->GetAll();
	for (auto& fb : fbs)
	{
		if (!fb->GetSpecification().NoResize) fb->Resize(instance->_width, instance->_height);
	}


	return false;
}

bool WindowsWindow::OnMove(const WindowMovedEvent& event)
{
	_position = event.GetPosition();
	return false;
}

bool WindowsWindow::OnGainFocus(const WindowGainedFocusEvent& event)
{
	return false;
}

bool WindowsWindow::OnLostFocus(const WindowLostFocusEvent& event)
{
	return false;
}

bool WindowsWindow::OnClose(const WindowClosedEvent& event)
{
	return false;
}

bool WindowsWindow::OnNewTitle(const WindowNewTitleEvent& event)
{
	glfwSetWindowTitle(_nativeWindow, event.GetTitle().c_str());
	_title = event.GetTitle();
	return true;
}

bool WindowsWindow::OnNewIcon(const WindowNewIconEvent& event)
{
	GLFWimage images[1];
	const String fullFilepath = "Assets/Textures/" + event.GetFilepath().string();
	images[0].pixels = stbi_load(fullFilepath.c_str(), &images[0].width, &images[0].height, nullptr, 4);
	//rgba channels
	if (!images[0].pixels)
	{
		Log::CoreWarn("Failed to load window icon. Filepath: {0}", event.GetFilepath().string());
		stbi_image_free(images[0].pixels);
		return false;
	}
	glfwSetWindowIcon(_nativeWindow, 1, images);
	stbi_image_free(images[0].pixels);
	return true;
}

void WindowsWindow::SetupGLFWCallbacks()
{
	Profiler::Function(PF_FUNC_SIG);

	glfwSetWindowUserPointer(_nativeWindow, this);


	// ----- Keyboard events -----
	glfwSetKeyCallback(_nativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS) pWnd->PushEvent<KeyPressedEvent>(static_cast<KeyCode>(key));
		else if (action == GLFW_RELEASE) pWnd->PushEvent<KeyReleasedEvent>(static_cast<KeyCode>(key));
		else if (action == GLFW_REPEAT) pWnd->PushEvent<KeyRepeatedEvent>(static_cast<KeyCode>(key));
	});

	// ----- Mouse events -----
	glfwSetMouseButtonCallback(_nativeWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS) pWnd->PushEvent<MouseButtonPressedEvent>(static_cast<MouseButtonCode>(button));
		else if (action == GLFW_RELEASE) pWnd->PushEvent<MouseButtonReleasedEvent
		>(static_cast<MouseButtonCode>(button));
	});
	glfwSetScrollCallback(_nativeWindow, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<MouseWheelScrolledEvent>(MouseWheelCode::Vertical, static_cast<float>(yoffset));
	});
	glfwSetCursorPosCallback(_nativeWindow, [](GLFWwindow* window, double xpos, double ypos)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<MouseMovedEvent>(Vector2(xpos, ypos));
	});
	glfwSetCursorEnterCallback(_nativeWindow, [](GLFWwindow* window, int enter)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		if (enter == GLFW_TRUE) pWnd->PushEvent<CursorEnteredEvent>();
		else pWnd->PushEvent<CursorLeftEvent>();
	});

	// ----- Window events -----
	glfwSetWindowSizeCallback(_nativeWindow, [](GLFWwindow* window, int width, int height)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<WindowResizedEvent>(width, height);
	});
	glfwSetWindowPosCallback(_nativeWindow, [](GLFWwindow* window, int xpos, int ypos)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<WindowMovedEvent>(Vector2(xpos, ypos));
	});
	glfwSetWindowFocusCallback(_nativeWindow, [](GLFWwindow* window, int focus)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		if (focus == GLFW_TRUE) pWnd->PushEvent<WindowGainedFocusEvent>();
		else pWnd->PushEvent<WindowLostFocusEvent>();
	});
	glfwSetWindowCloseCallback(_nativeWindow, [](GLFWwindow* window)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<WindowClosedEvent>();
	});
	glfwSetDropCallback(_nativeWindow, [](GLFWwindow* window, int count, const char** paths)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		List<Path> filepaths(count);
		for (int i = 0; i < count; i++)
		{
			filepaths[i] = paths[i];
		}
		pWnd->PushEvent<WindowDroppedFilesEvent>(filepaths);
	});
	glfwSetWindowIconifyCallback(_nativeWindow, [](GLFWwindow* window, int restored)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<WindowMinimizedEvent>(static_cast<bool>(restored));
	});
	glfwSetWindowMaximizeCallback(_nativeWindow, [](GLFWwindow* window, int restored)
	{
		auto* pWnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		pWnd->PushEvent<WindowMaximizedEvent>(static_cast<bool>(restored));
	});
}

void WindowsWindow::Restore()
{
	glfwRestoreWindow(_nativeWindow);
	_minimized = false;
	_maximized = false;
}
}
