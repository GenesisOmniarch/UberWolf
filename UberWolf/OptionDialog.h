/*
 *  File: OptionDialog.h
 *  Copyright (c) 2023 Sinflower
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#pragma once

#include <ShObjIdl.h>
#include <string>

#include "Localizer.h"
#include "WindowBase.h"
#include "resource.h"

namespace
{
// Converts a UTF-8 string to a wide string (used for the config file, which stores UTF-8)
inline std::wstring Utf8ToWide(const std::string& str)
{
	if (str.empty()) return L"";
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
	std::wstring wstr(size, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), wstr.data(), size);
	return wstr;
}

// Converts a wide string to a UTF-8 string
inline std::string WideToUtf8(const std::wstring& wstr)
{
	if (wstr.empty()) return "";
	const int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
	std::string str(size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), str.data(), size, nullptr, nullptr);
	return str;
}

// Shows a folder selection dialog, returns true if a folder was selected
inline bool SelectFolder(HWND hwndParent, const wchar_t* pTitle, std::wstring& folder)
{
	const HRESULT hrInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	bool selected        = false;

	IFileOpenDialog* pDialog = nullptr;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog))))
	{
		DWORD options = 0;
		pDialog->GetOptions(&options);
		pDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
		pDialog->SetTitle(pTitle);

		if (SUCCEEDED(pDialog->Show(hwndParent)))
		{
			IShellItem* pItem = nullptr;
			if (SUCCEEDED(pDialog->GetResult(&pItem)))
			{
				PWSTR pPath = nullptr;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath)))
				{
					folder   = pPath;
					selected = true;
					CoTaskMemFree(pPath);
				}
				pItem->Release();
			}
		}
		pDialog->Release();
	}

	if (SUCCEEDED(hrInit))
		CoUninitialize();

	return selected;
}
} // namespace

class OptionDialog : public WindowBase
{
	static const int32_t CHECKBOX_WIDTH = 20;
	static const int32_t TEXT_HEIGH     = 20;
	static const int32_t STATIC_WIDTH   = 50;

public:
	OptionDialog() = delete;

	OptionDialog(const HINSTANCE hInstance, const HWND hWndParent) :
		WindowBase(hInstance, hWndParent)
	{
		registerSlot(IDOK, BN_CLICKED, [this]() { onOKClicked(); });
		registerSlot(IDCANCEL, BN_CLICKED, [this]() { onCancelClicked(); });
		registerSlot(IDC_CHECK_OVERWRITE, BN_CLICKED, [this]() { onOverwriteClicked(); });
		registerSlot(IDC_CHECK_UNPROTECT, BN_CLICKED, [this]() { onUnprotectClicked(); });
		registerSlot(IDC_CHECK_DEC_WOLFX, BN_CLICKED, [this]() { onDecWolfXClicked(); });
		registerSlot(IDC_SELECT_OUTPUT_DIR, BN_CLICKED, [this]() { onSelectOutputDirClicked(); });
		registerSlot(IDC_OUTPUT_DIR, EN_CHANGE, [this]() { onOutputDirChanged(); });

		registerCheckBoxState(IDC_CHECK_OVERWRITE, [this]() { return m_overwrite; });
		registerCheckBoxState(IDC_CHECK_UNPROTECT, [this]() { return m_unprotect; });
		registerCheckBoxState(IDC_CHECK_DEC_WOLFX, [this]() { return m_decWolfX; });

		m_overwrite = getSaveValue<bool>(IDC_CHECK_OVERWRITE, false);
		m_unprotect = getSaveValue<bool>(IDC_CHECK_UNPROTECT, true);
		m_decWolfX  = getSaveValue<bool>(IDC_CHECK_DEC_WOLFX, false);
		m_outputDir = Utf8ToWide(getSaveValue<std::string>(IDC_OUTPUT_DIR, ""));
	}

	~OptionDialog()
	{
	}

	const bool& Unprotect() const
	{
		return m_unprotect;
	}

	const bool& Overwrite() const
	{
		return m_overwrite;
	}

	const bool& DecWolfX() const
	{
		return m_decWolfX;
	}

	// Output directory for extracted files, empty means next to the archive
	const std::wstring& OutputDir() const
	{
		return m_outputDir;
	}

	void SetParent(const HWND& hWndParent)
	{
		m_hWndParent = hWndParent;
	}

	void Show()
	{
		DialogBoxParam(m_hInstance, MAKEINTRESOURCE(IDD_OPTIONS), m_hWndParent, dlgProc, reinterpret_cast<LPARAM>(this));
	}

	void Hide(const bool& ok = true)
	{
		EndDialog(hWnd(), ok);
		unsetHandle();
	}

	void AdjustSizes()
	{
		int32_t maxWidth = 0;

		maxWidth = max(maxWidth, adjustCheckBox(IDC_CHECK_OVERWRITE));
		maxWidth = max(maxWidth, adjustCheckBox(IDC_CHECK_UNPROTECT));
		maxWidth = max(maxWidth, adjustCheckBox(IDC_CHECK_DEC_WOLFX));
		maxWidth = max(maxWidth, GetCaptionTextWidth(GetDlgItem(hWnd(), IDC_LABEL_OUTPUT_DIR)));

		// Add the static width for the dialog
		maxWidth += STATIC_WIDTH;

		// Adjust the dialog size to fit the new text
		RECT rc;
		GetWindowRect(hWnd(), &rc);
		int32_t minWidth = rc.right - rc.left;
		SetWindowPos(hWnd(), nullptr, 0, 0, max(minWidth, maxWidth), rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
	}

private:
	void onOKClicked()
	{
		Hide(true);
	}

	void onCancelClicked()
	{
		Hide(false);
	}

	void onOverwriteClicked()
	{
		m_overwrite = IsDlgButtonChecked(hWnd(), IDC_CHECK_OVERWRITE);
		updateSaveValue<bool>(IDC_CHECK_OVERWRITE, m_overwrite);
	}

	void onUnprotectClicked()
	{
		m_unprotect = IsDlgButtonChecked(hWnd(), IDC_CHECK_UNPROTECT);
		updateSaveValue<bool>(IDC_CHECK_UNPROTECT, m_unprotect);
	}

	void onDecWolfXClicked()
	{
		m_decWolfX = IsDlgButtonChecked(hWnd(), IDC_CHECK_DEC_WOLFX);
		updateSaveValue<bool>(IDC_CHECK_DEC_WOLFX, m_decWolfX);
	}

	void onSelectOutputDirClicked()
	{
		std::wstring folder;
		if (SelectFolder(hWnd(), LOCW("select_output_dir"), folder))
			SetDlgItemText(hWnd(), IDC_OUTPUT_DIR, folder.c_str()); // Triggers EN_CHANGE
	}

	void onOutputDirChanged()
	{
		wchar_t buffer[MAX_PATH * 4];
		buffer[0] = L'\0';
		GetDlgItemText(hWnd(), IDC_OUTPUT_DIR, buffer, static_cast<int>(std::size(buffer)));

		m_outputDir = buffer;

		// Trim surrounding whitespace and quotes
		const std::wstring trimChars = L" \t\"";
		const std::size_t first      = m_outputDir.find_first_not_of(trimChars);
		if (first == std::wstring::npos)
			m_outputDir.clear();
		else
			m_outputDir = m_outputDir.substr(first, m_outputDir.find_last_not_of(trimChars) - first + 1);

		updateSaveValue<std::string>(IDC_OUTPUT_DIR, WideToUtf8(m_outputDir));
	}

	void initOutputDir()
	{
		SetDlgItemText(hWnd(), IDC_OUTPUT_DIR, m_outputDir.c_str());
	}

	int32_t adjustCheckBox(const int32_t& id)
	{
		// Get the handle of the checkbox
		HWND hCheckBox = GetDlgItem(hWnd(), id);
		// Get the width of the caption text
		int32_t width = GetCaptionTextWidth(hCheckBox);
		// Set the new width of the checkbox
		SetWindowPos(hCheckBox, nullptr, 0, 0, width + CHECKBOX_WIDTH, TEXT_HEIGH, SWP_NOMOVE | SWP_NOZORDER);
		// Return the new width
		return width + CHECKBOX_WIDTH;
	}

	static INT_PTR CALLBACK dlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Check which message was passed in
		switch (uMsg)
		{
			case WM_COMMAND:
				if (WindowBase::ProcessCommand(hWnd, wParam))
					return TRUE;
				break;
			case WM_INITDIALOG:
				WindowBase* pWnd = reinterpret_cast<WindowBase*>(lParam);
				SendDlgItemMessage(hWnd, IDC_CHECK_OVERWRITE, BM_SETCHECK, pWnd->GetCheckBoxState(IDC_CHECK_OVERWRITE), 0);
				SendDlgItemMessage(hWnd, IDC_CHECK_UNPROTECT, BM_SETCHECK, pWnd->GetCheckBoxState(IDC_CHECK_UNPROTECT), 0);
				SendDlgItemMessage(hWnd, IDC_CHECK_DEC_WOLFX, BM_SETCHECK, pWnd->GetCheckBoxState(IDC_CHECK_DEC_WOLFX), 0);

				SendDlgItemMessage(hWnd, IDC_CHECK_OVERWRITE, WM_SETTEXT, 0, (LPARAM)LOCW("overwrite_label"));
				SendDlgItemMessage(hWnd, IDC_CHECK_UNPROTECT, WM_SETTEXT, 0, (LPARAM)LOCW("unprotect_label"));
				SendDlgItemMessage(hWnd, IDC_CHECK_DEC_WOLFX, WM_SETTEXT, 0, (LPARAM)LOCW("dec_wolfx_label"));
				SendDlgItemMessage(hWnd, IDC_LABEL_OUTPUT_DIR, WM_SETTEXT, 0, (LPARAM)LOCW("output_dir_label"));
				SendDlgItemMessage(hWnd, IDOK, WM_SETTEXT, 0, (LPARAM)LOCW("ok"));
				SendDlgItemMessage(hWnd, IDCANCEL, WM_SETTEXT, 0, (LPARAM)LOCW("cancel"));
				SetWindowText(hWnd, LOCW("options"));

				pWnd->SetHandle(hWnd);
				static_cast<OptionDialog*>(pWnd)->initOutputDir();
				pWnd->AdjustSizes();
				return TRUE;
		}

		return FALSE;
	}

private:
	bool m_overwrite = false;
	bool m_unprotect = false;
	bool m_decWolfX  = false;
	std::wstring m_outputDir = L"";
};
