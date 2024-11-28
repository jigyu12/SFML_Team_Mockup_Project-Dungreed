#include "stdafx.h"
#include "FileDialog.h"

// Instance creation helper
HRESULT FileDialog::CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
{
	*ppv = NULL;
	CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
	HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr))
	{
		hr = pDialogEventHandler->QueryInterface(riid, ppv);
		pDialogEventHandler->Release();
	}
	return hr;
}

HRESULT FileDialog::OpenDialog(bool open, std::function<void(const std::wstring& wpath)> pathfunction)
{
	// CoCreate the File Open Dialog object.
	IFileDialog* pfd = NULL;
	HRESULT hr;
	if (open)
	{
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	else
	{
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	if (SUCCEEDED(hr))
	{
		// Create an event handling object, and hook it up to the dialog.
		IFileDialogEvents* pfde = NULL;
		hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
		if (SUCCEEDED(hr))
		{
			// Hook up the event handler.
			DWORD dwCookie;
			hr = pfd->Advise(pfde, &dwCookie);
			if (SUCCEEDED(hr))
			{
				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order not to override existing options.
				hr = pfd->GetOptions(&dwFlags);
				if (SUCCEEDED(hr))
				{
					// In this case, get shell items only for file system items.
					hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
					if (SUCCEEDED(hr))
					{
						// Set the file types to display only. Notice that, this is a 1-based array.
						hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
						if (SUCCEEDED(hr))
						{
							hr = pfd->SetDefaultExtension(L"json");
							if (SUCCEEDED(hr))
							{
								// Show the dialog
								hr = pfd->Show(NULL);
								if (SUCCEEDED(hr))
								{
									// Obtain the result, once the user clicks the 'Open' button.
									// The result is an IShellItem object.
									IShellItem* psiResult;

									hr = pfd->GetResult(&psiResult);
									if (SUCCEEDED(hr))
									{
										// We are just going to print out the name of the file for sample sake.
										PWSTR pszFilePath = NULL;
										hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
										if (SUCCEEDED(hr))
										{
											if (pathfunction)
											{
												pathfunction(pszFilePath);
											}
											psiResult->Release();
											CoTaskMemFree(pszFilePath);
										}
									}
								}
							}
						}
					}
				}
				// Unhook the event handler.
				pfd->Unadvise(dwCookie);
			}
			pfde->Release();
		}
		pfd->Release();
	}
	return hr;
}