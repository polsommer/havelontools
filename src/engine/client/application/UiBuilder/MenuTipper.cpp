////////////////////////////////////////////////////////////////
// MSDN Magazine -- November 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#include "FirstUiBuilder.h"
#include "MenuTipper.h"
#include <afxpriv.h> // for AfxLoadString

#include "UiBuilder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

extern CUiBuilderApp theApp;

/////////////////////////////////////////////////////////////////////////////

//////////////////
// This is more or less copied from CFrameWnd::GetMessageString, but
// I want a static function that doesn't require a frame window.
//
CString CMenuTipManager::GetResCommandPrompt(UINT nID)
{
	// load appropriate string
	CString s;
	if (s.LoadString(nID)) {
		LPTSTR lpsz = s.GetBuffer(255);
		// first newline terminates prompt
		lpsz = _tcschr(lpsz, '\n');
		if (lpsz != NULL)
			*lpsz = '\0';
		s.ReleaseBuffer();
	}
	return s;
}

// Get the prompt for given command ID
CString CMenuTipManager::OnGetCommandPrompt(UINT nID)
{
	if (nID>=ID_FILE_MRU_FILE1 && nID<=ID_FILE_MRU_FILE16)
	{
		CString fileName;
		if (theApp.getRecentFileName(fileName, nID-ID_FILE_MRU_FILE1))
		{
			return fileName;
		}
	}
	return GetResCommandPrompt(nID);
}

//////////////////
// Override CSubclassWnd::WindowProc to hook messages on behalf of
// main window.
//
LRESULT CMenuTipManager::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg==WM_MENUSELECT) {
		OnMenuSelect(LOWORD(wp), HIWORD(wp), (HMENU)lp);
	} else if (msg==WM_ENTERIDLE) {
		OnEnterIdle(wp, (HWND)lp);
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}

//////////////////
// Got WM_MENUSELECT: show tip.
//
void CMenuTipManager::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hMenu)
{
	if (!m_wndTip.m_hWnd) {
		m_wndTip.Create(CPoint(0,0), CWnd::FromHandle(m_hWnd));
		m_wndTip.m_szMargins = CSize(4,0);
	}

	if ((nFlags & 0xFFFF)==0xFFFF) {
		m_wndTip.Cancel();	// cancel/hide tip
		m_bMouseSelect = FALSE;
		m_bSticky = FALSE;

	} else if (nFlags & MF_POPUP) {
		m_wndTip.Cancel();	// new popup: cancel
		m_bSticky = FALSE;

	} else if (nFlags & MF_SEPARATOR) {
		// separator: hide tip but remember sticky state
		m_bSticky = m_wndTip.IsWindowVisible();
		m_wndTip.Cancel();

	} else if (nItemID && hMenu) {
		// if tips already displayed, keep displayed
		m_bSticky = m_wndTip.IsWindowVisible() || m_bSticky;

		// remember if mouse used to invoke menu
		m_bMouseSelect = (nFlags & MF_MOUSESELECT)!=0;

		// get prompt and display tip (with or without timeout)
		CString prompt = OnGetCommandPrompt(nItemID);
		if (prompt.IsEmpty())
			m_wndTip.Cancel(); // no prompt: cancel tip

		else {
			CRect rc = GetMenuTipRect(hMenu, nItemID);
			m_wndTip.SetWindowPos(&CWnd::wndTopMost, rc.left, rc.top,
				rc.Width(), rc.Height(), SWP_NOACTIVATE);
			m_wndTip.SetWindowText(prompt);
			m_wndTip.ShowDelayed(m_bSticky ? 0 : m_iDelay);
		}
	}
}

//////////////////
// Calculate position of tip: next to menu item.
//
CRect CMenuTipManager::GetMenuTipRect(HMENU hmenu, UINT nID)
{
	CWnd* pWndMenu = GetRunningMenuWnd(); //CWnd::WindowFromPoint(pt);
	ASSERT(pWndMenu);

	CRect rcMenu;
	pWndMenu->GetWindowRect(rcMenu); // whole menu rect

	// add heights of menu items until i reach nID
	int count = ::GetMenuItemCount(hmenu);
	int cy = rcMenu.top + GetSystemMetrics(SM_CYEDGE)+1;
	for (int i=0; i<count; i++) {
		CRect rc;
		::GetMenuItemRect(m_hWnd, hmenu, i, &rc);
		if (::GetMenuItemID(hmenu,i)==nID) {
			// found menu item: adjust rectangle to right and down
			rc += CPoint(rcMenu.right - rc.left, cy - rc.top);
			return rc;
		}
		cy += rc.Height(); // add height
	}
	return CRect(0,0,0,0);
}

//////////////////
// Note that windows are enumerated in top-down Z-order, so the menu
// window should always be the first one found.
//
static BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM lParam)
{
	char buf[16];
	GetClassName(hwnd,buf,sizeof(buf));
	if (strcmp(buf,"#32768")==0) { // special classname for menus
		*((HWND*)lParam) = hwnd;	 // found it
		return FALSE;
	}
	return TRUE;
}

//////////////////
// Get running menu window.
//
CWnd* CMenuTipManager::GetRunningMenuWnd()
{
	HWND hwnd = NULL;
	EnumWindows(MyEnumProc,(LPARAM)&hwnd);
	return CWnd::FromHandle(hwnd);
}

//////////////////
// Need to handle WM_ENTERIDLE to cancel the tip if the user moved
// the mouse off the popup menu. For main menus, Windows will send a
// WM_MENUSELECT message for the parent menu when this happens, but for
// context menus there's no other way to know the user moved the mouse
// off the menu.
//
void CMenuTipManager::OnEnterIdle(WPARAM nWhy, HWND hwndWho)
{
	if (m_bMouseSelect && nWhy==MSGF_MENU) {
		CPoint pt;
		GetCursorPos(&pt);
		if (hwndWho != ::WindowFromPoint(pt)) {
			m_wndTip.Cancel();
		}
	}
}
