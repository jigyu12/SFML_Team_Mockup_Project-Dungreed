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

namespace FileDialog
{
	HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv);

	class CDialogEventHandler : public IFileDialogEvents,
		public IFileDialogControlEvents
	{
	public:
		// IUnknown methods
		IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
		{
			static const QITAB qit[] = {
				QITABENT(CDialogEventHandler, IFileDialogEvents),
				QITABENT(CDialogEventHandler, IFileDialogControlEvents),
				{ 0 },
	#pragma warning(suppress:4838)
			};
			return QISearch(this, qit, riid, ppv);
		}

		IFACEMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement(&_cRef);
		}

		IFACEMETHODIMP_(ULONG) Release()
		{
			long cRef = InterlockedDecrement(&_cRef);
			if (!cRef)
				delete this;
			return cRef;
		}

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


#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

	const COMDLG_FILTERSPEC c_rgSaveTypes[] =
	{
		{L"Map Data (*.json)",			 L"*.json"},
		{L"All Files (*.*)",         L"*.*"}
	};



	/* Common File Dialog Snippets ***************************************************************************************************/

	// This code snippet demonstrates how to work with the common file dialog interface
	HRESULT OpenDialog(bool open = true, std::function<void(const std::wstring& wpath)> pathfunction = [](std::wstring wpath) {});
	
}