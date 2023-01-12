
/**
 * @file tether.h
 * @author Woeful_Wolf
 */

#pragma once
#include <iostream>
#include <wchar.h>

#include <dxgi1_3.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace tether {
	enum class tetherResult {
		Success = 0,
		GetSystemDirectoryFail = -1,
		BufferError = -2,
		LoadLibraryError = -3,
		GetAddressError = -4
	};

	void ErrorBox(tetherResult result, const char* msg = "NoneSpecified") {
		char buffer[128];
		_snprintf_s(buffer, 128, 128, "%d (%s)", result, msg);

		MessageBoxA(GetActiveWindow(), buffer, "tether Error!", MB_OK);
	}

	HMODULE CreateTether(const wchar_t* moduleName) {
		HMODULE module;

		wchar_t buffer[MAX_PATH];

		if (GetSystemDirectoryW(buffer, MAX_PATH) == 0) {
			ErrorBox(tetherResult::GetSystemDirectoryFail, "Couldn't get system directory.");
		}

		if (wcscat_s(buffer, MAX_PATH, L"\\") != 0) {
			ErrorBox(tetherResult::BufferError, "how did this fail???");
		}

		if (wcscat_s(buffer, MAX_PATH, moduleName) != 0) {
			ErrorBox(tetherResult::BufferError, "Error copying module name to buffer.");
		}

		module = LoadLibraryW(buffer);
		if (module == NULL) {
			ErrorBox(tetherResult::LoadLibraryError, "Failed to load library when creating tether.");
		}

		return module;
	}

	extern "C" {
		// dxgi
#pragma comment(linker, "/EXPORT:CreateDXGIFactory=tether_CreateDXGIFactory")
		inline __declspec(dllexport) DWORD WINAPI tether_CreateDXGIFactory(REFIID riid, void** ppFactory) {
			void* addr = GetProcAddress(CreateTether(L"dxgi.dll"), "CreateDXGIFactory");

			if (!addr) {
				ErrorBox(tetherResult::GetAddressError, "CreateDXGIFactory");
				return NULL;
			}
			else
				return ((decltype(CreateDXGIFactory)*)addr)(riid, ppFactory);
		}

#pragma comment(linker, "/EXPORT:CreateDXGIFactory1=tether_CreateDXGIFactory1")
		inline __declspec(dllexport) DWORD WINAPI tether_CreateDXGIFactory1(REFIID riid, void** ppFactory) {
			void* addr = GetProcAddress(CreateTether(L"dxgi.dll"), "CreateDXGIFactory");

			if (!addr)
				ErrorBox(tetherResult::GetAddressError, "CreateDXGIFactory1");
			else
				return ((decltype(CreateDXGIFactory1)*)addr)(riid, ppFactory);
		}

#pragma comment(linker, "/EXPORT:CreateDXGIFactory2=tether_CreateDXGIFactory2")
		inline __declspec(dllexport) DWORD WINAPI tether_CreateDXGIFactory2(UINT flags, REFIID riid, void** ppFactory) {
			void* addr = GetProcAddress(CreateTether(L"dxgi.dll"), "CreateDXGIFactory");

			if (!addr)
				ErrorBox(tetherResult::GetAddressError, "CreateDXGIFactory2");
			else
				return ((decltype(CreateDXGIFactory2)*)addr)(flags, riid, ppFactory);
		}

		// dinput8
#pragma comment(linker, "/EXPORT:DirectInput8Create=tether_DirectInput8Create")
		inline __declspec(dllexport) DWORD WINAPI tether_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
			void* addr = GetProcAddress(CreateTether(L"dinput8.dll"), "DirectInput8Create");

			if (!addr)
				ErrorBox(tetherResult::GetAddressError, "DirectInput8Create");
			else
				return ((decltype(DirectInput8Create)*)addr)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
		}
	}

}