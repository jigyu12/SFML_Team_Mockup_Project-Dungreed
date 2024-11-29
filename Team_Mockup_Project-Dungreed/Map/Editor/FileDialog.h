#pragma once
#include <windows.h>
#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <knownfolders.h>
#include <propvarutil.h>
#include <propkey.h>
#include <propidl.h>
#include <strsafe.h>
#include <shtypes.h>
#include <new>

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace FileDialog
{
	HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv);
	HRESULT OpenDialog(std::function<void(const std::wstring& wpath)> pathfunction, bool open = true);

	const COMDLG_FILTERSPEC c_rgSaveTypes[] =
	{
		{L"Map Data (*.json)",			 L"*.json"},
		{L"All Files (*.*)",         L"*.*"}
	};


	class CDialogEventHandler : public IFileDialogEvents,
		public IFileDialogControlEvents
	{
	public:
		// IUnknown methods
		IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);

		IFACEMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&_cRef); }

		IFACEMETHODIMP_(ULONG) Release();

		// IFileDialogEvents methods
		IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
		IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
		IFACEMETHODIMP OnTypeChange(IFileDialog* pfd) { return S_OK; };
		IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

		// IFileDialogControlEvents methods
		IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem) { return S_OK; };
		IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
		IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
		IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

		CDialogEventHandler() : _cRef(1) {};
	private:
		~CDialogEventHandler() {};
		long _cRef;
	};
}