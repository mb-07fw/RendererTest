#pragma once

#include "Windows.h"
#include "Geometry.hpp"
#include "Graphics.hpp"
#include "Event/EventManager.hpp"

#include <thread> // std::thread
#include <atomic> // std::atomic
#include <mutex>  // std::mutex
#include <condition_variable> // std::condition_variable

namespace Renderer::Window
{
	class Window
	{
	public:
		Window(Event::EventManager& eventManagerRef, const unsigned int targetFPS, unsigned int width = 800, unsigned int height = 600);
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&&) = delete;
		Window& operator=(Window&&) = delete;
	public:
		void Start() noexcept;
		void HandleMessages(BOOL& result, MSG& msg) noexcept;
		void DoFrame() noexcept;
		void SetTitle(std::wstring title) noexcept;
	public:
		[[nodiscard]] inline bool IsInitialized() { return m_IsInitialized.load(std::memory_order_acquire); }
		[[nodiscard]] inline bool IsShown()		  { return m_IsShown.load(std::memory_order_acquire); }
		[[nodiscard]] inline bool IsRunning()	  { return m_IsRunning.load(std::memory_order_acquire); }
	private:
		void Init();
	private:
		static LRESULT CALLBACK WndProcSetup(HWND windowHandle, UINT msgCode, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK WndProcThunk(HWND windowHandle, UINT msgCode, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK WndProc(HWND windowHandle, UINT msgCode, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		static constexpr const wchar_t* SP_WINDOW_CLASS_NAME = L"TestWindow"; // SMP = static member pointer.
		static constexpr const wchar_t* SP_WINDOW_TITLE = L"Test Window";
	private:
		Event::EventManager& m_EventManagerRef;
		const unsigned int m_TargetFPS;
		Geometry::WindowArea m_WindowArea;
		Graphics::Graphics m_Graphics;
		HWND m_WindowHandle;
		std::atomic_bool m_IsInitialized;
		std::atomic_bool m_IsShown;
		std::atomic_bool m_IsRunning;
		std::mutex m_Mutex;
		std::condition_variable m_CV;
	};
}