/*
 * SimpleTestDlg.cpp
 *
 * Copyright (c) 1997-2020 ETEL SA. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of ETEL SA
 * ("Confidential Information"). You shall not disclose such Confidential
 * Information and shall use it only in accordance with the terms of the
 * license agreement you entered into with ETEL.
 *
 * This software is provided "AS IS" without a warranty or representations of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED WARRANTY
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT ARE HEREBY EXCLUDED. ETEL AND ITS
 * LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL ETEL OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR
 * DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 * OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF ETEL HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES. THE ENTIRE RISK ARISING OUT OF USE, PERFORMANCE OR NON-PERFORMANCE OF THE SOFTWARE
 * REMAINS WITH THE LICENSEE. IF ETEL SHOULD NEVERTHELESS BE FOUND LIABLE, WHETER DIRECTLY OR INDRECTLY, FOR ANY LOSS,
 * DAMAGE OR INJURY ARISING UNDER THIS AGREEMENT OR OTHERWISE, REGARDLESS OF CAUSE OR ORIGIN, ON ANY BASIS WHATSOEVER,
 * ITS TOTAL MAXIMUM LIABILITY IS LIMITED TO CHF 100.000 WHICH WILL BE THE COMPLETE AND EXCLUSIVE REMEDY AGAINST ETEL.

 * This software is in particular not designed or intended .for use in on-line control of aircraft, air traffic, aircraft
 * navigation or aircraft communications; or in the design, construction, Operation or maintenance of any nuclear facility.
 * Licensee represents and warrants that it will not use or redistribute the Software for such purposes.
 */

#include "SimpleTestDlg.h"
#include "SimpleTest.h"
#include "TCPIPDlg.h"
#include "stdafx.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

// CSimpleTestDlg dialog

CSimpleTestDlg::CSimpleTestDlg(CWnd *pParent /*=NULL*/) : CDialog(CSimpleTestDlg::IDD, pParent), m_sTCPIP(_T("")), m_bDoMonitoring(false), m_bHomingDone(false)
{
    m_pEDIMngr = EDIManager::getEDIMngrInstance();
    m_hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleTestDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CONNECT, CComboConnect);
    DDX_Control(pDX, IDC_BUTTON_MIN, m_CMinBtn);
    DDX_Control(pDX, IDC_BUTTON_MINUS, m_CMinusBtn);
    DDX_Control(pDX, IDC_BUTTON_PLUS, m_CPlusBtn);
    DDX_Control(pDX, IDC_BUTTON_MAX, m_CMaxBtn);
    DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_CDisconnectBtn);
    DDX_Control(pDX, IDC_BUTTON_CONNECT, m_CConnectBtn);
    DDX_Control(pDX, IDC_BUTTON_ON, m_COnBtn);
    DDX_Control(pDX, IDC_BUTTON_OFF, m_COffBtn);
    DDX_Control(pDX, IDC_BUTTON_HOME, m_CHomeBtn);
    DDX_Control(pDX, IDC_EDIT_POS, m_CPosEdit);
    DDX_Control(pDX, IDC_EDIT_VEL, m_CVelEdit);
    DDX_Control(pDX, IDC_EDIT_STATUS, m_CStatusMovingEdit);
    DDX_Control(pDX, IDC_EDIT_STATUS2, m_CStatusWarningEdit);
    DDX_Control(pDX, IDC_EDIT_STATUS3, m_CStatusErrorEdit);
    DDX_Control(pDX, IDC_EDIT_STATUS4, m_CStatusPwrOnEdit);
}

BEGIN_MESSAGE_MAP(CSimpleTestDlg, CDialog)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSimpleTestDlg::OnBnClickedButtonConnect)
ON_CBN_SELCHANGE(IDC_COMBO_CONNECT, &CSimpleTestDlg::OnCbnSelchangeComboConnect)
ON_BN_CLICKED(IDC_BUTTON_HOME, &CSimpleTestDlg::OnBnClickedButtonHome)
ON_BN_CLICKED(IDC_BUTTON_MIN, &CSimpleTestDlg::OnBnClickedButtonMin)
ON_BN_CLICKED(IDC_BUTTON_MAX, &CSimpleTestDlg::OnBnClickedButtonMax)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CSimpleTestDlg::OnBnClickedButtonDisconnect)
ON_BN_CLICKED(IDC_BUTTON_PLUS, &CSimpleTestDlg::OnBnClickedButtonPlus)
ON_BN_CLICKED(IDC_BUTTON_MINUS, &CSimpleTestDlg::OnBnClickedButtonMinus)
ON_BN_CLICKED(IDC_BUTTON_ON, &CSimpleTestDlg::OnBnClickedButtonOn)
ON_BN_CLICKED(IDC_BUTTON_OFF, &CSimpleTestDlg::OnBnClickedButtonOff)
END_MESSAGE_MAP()

// CSimpleTestDlg message handlers

BOOL CSimpleTestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Init ComboBox Connect
    CComboConnect.ResetContent();
    CComboConnect.AddString(_T("USB"));
    CComboConnect.AddString(_T("AccurET TCP/IP"));
    CComboConnect.AddString(_T("UltimET TCP/IP"));
    CComboConnect.AddString(_T("UltimET PCI"));

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    CleanMonitoring();

    CString CCurSel;
    CComboConnect.SetCurSel(3);
    CComboConnect.GetLBText(3, CCurSel);
    CComboConnect.SetWindowText(CCurSel);

    return TRUE; // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleTestDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int   cxIcon = GetSystemMetrics(SM_CXICON);
        int   cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleTestDlg::OnCbnSelchangeComboConnect()
{
    CString CCurSel;
    CComboConnect.GetLBText(CComboConnect.GetCurSel(), CCurSel);

    if ((CCurSel == "AccurET TCP/IP") || (CCurSel == "UltimET TCP/IP")) {
        TCPIPDlg dlg;

        if (dlg.DoModal() == IDOK) {
            m_sTCPIP = dlg.m_sAddress;
        }
    }
}

void CSimpleTestDlg::OnBnClickedButtonConnect()
{
    try {
        m_pEDIMngr->InitDriveSystem();

        CString CCurSel;
        CComboConnect.GetLBText(CComboConnect.GetCurSel(), CCurSel);

        CT2CA       pszConvertedAnsiString(m_sTCPIP);
        std::string strTCPIP(pszConvertedAnsiString);

        if ((CCurSel == "AccurET TCP/IP") || (CCurSel == "UltimET TCP/IP"))
            m_pEDIMngr->ConnectDriveSystem(string("ETN://") + strTCPIP);

        else if (CCurSel == "UltimET PCI")
            m_pEDIMngr->ConnectDriveSystem(string("ULTIMET"));

        else if (CCurSel == "USB")
            m_pEDIMngr->ConnectDriveSystem(string("USB"));

        else
            return;

        CWinThread *pThread;
        pThread         = AfxBeginThread(MonitoringThread, (LPVOID)this, THREAD_PRIORITY_NORMAL | THREAD_TERMINATE);
        m_bDoMonitoring = true;

        m_CDisconnectBtn.EnableWindow();
        m_CConnectBtn.EnableWindow(false);
        m_COnBtn.EnableWindow();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonDisconnect()
{
    ebool bException = false;

    m_bDoMonitoring = false;
    CleanMonitoring();
    try {
        m_pEDIMngr->QuickStop();
    }

    catch (const exception &e) {
        if (!bException)
            bException = true;

        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }

    try {
        m_pEDIMngr->PwrOffDriveSystem();
    }

    catch (const exception &e) {
        if (!bException) {
            bException = true;

            char strException[1024];
            strcpy(strException, e.what());
            AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
        }
    }

    try {
        m_pEDIMngr->DisconnectDriveSystem();
    }

    catch (const exception &e) {
        if (!bException) {
            bException = true;

            char strException[1024];
            strcpy(strException, e.what());
            AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
        }
    }

    m_CDisconnectBtn.EnableWindow(false);
    m_CHomeBtn.EnableWindow(false);
    m_CConnectBtn.EnableWindow();
    m_COnBtn.EnableWindow(false);
    m_COffBtn.EnableWindow(false);
    m_CMinBtn.EnableWindow(false);
    m_CMaxBtn.EnableWindow(false);
    m_CMinusBtn.EnableWindow(false);
    m_CPlusBtn.EnableWindow(false);

    m_bHomingDone = false;
}

void CSimpleTestDlg::OnBnClickedButtonHome()
{
    try {
        m_CMinBtn.EnableWindow(false);
        m_CMaxBtn.EnableWindow(false);
        m_CMinusBtn.EnableWindow(false);
        m_CPlusBtn.EnableWindow(false);

        m_pEDIMngr->Homing();

        m_bHomingDone = true;

        m_CMinBtn.EnableWindow();
        m_CMaxBtn.EnableWindow();
        m_CMinusBtn.EnableWindow();
        m_CPlusBtn.EnableWindow();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonMin()
{
    try {
        m_pEDIMngr->MoveMinPos();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonMax()
{
    try {
        m_pEDIMngr->MoveMaxPos();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

BOOL CSimpleTestDlg::DestroyWindow()
{
    ebool bException = false;
    try {
        m_pEDIMngr->QuickStop();
    }

    catch (const exception &e) {
        if (!bException)
            bException = true;

        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }

    try {
        m_pEDIMngr->PwrOffDriveSystem();
    }

    catch (const exception &e) {
        if (!bException) {
            bException = true;

            char strException[1024];
            strcpy(strException, e.what());
            AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
        }
    }

    try {
        m_pEDIMngr->DisconnectDriveSystem();
    }

    catch (const exception &e) {
        if (!bException) {
            bException = true;

            char strException[1024];
            strcpy(strException, e.what());
            AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
        }
    }

    return CDialog::DestroyWindow();
}

void CSimpleTestDlg::OnBnClickedButtonPlus()
{
    try {
        m_pEDIMngr->MoveStepPlus();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonMinus()
{
    try {
        m_pEDIMngr->MoveStepMinus();
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonOn()
{
    try {
        m_pEDIMngr->PwrOnDriveSystem();
        m_COnBtn.EnableWindow(false);
        m_COffBtn.EnableWindow();
        m_CHomeBtn.EnableWindow();

        if (m_bHomingDone) {
            m_CMinBtn.EnableWindow();
            m_CMaxBtn.EnableWindow();
            m_CMinusBtn.EnableWindow();
            m_CPlusBtn.EnableWindow();
        }
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::OnBnClickedButtonOff()
{
    try {
        m_pEDIMngr->PwrOffDriveSystem();
        m_COffBtn.EnableWindow(false);
        m_COnBtn.EnableWindow();
        m_CHomeBtn.EnableWindow(false);
        m_CMinBtn.EnableWindow(false);
        m_CMaxBtn.EnableWindow(false);
        m_CMinusBtn.EnableWindow(false);
        m_CPlusBtn.EnableWindow(false);
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

UINT CSimpleTestDlg::MonitoringThread(LPVOID pParam)
{

    CSimpleTestDlg *pDlg = (CSimpleTestDlg *)pParam;
    pDlg->MonitoringThread();
    return 0;
}

void CSimpleTestDlg::MonitoringThread()
{
    try {
        EDIManager::MonitoringData monitorData;
        /*
         * Start an infinite loop. This function will be stopped by the OS only
         * when the whole process will end.
         */
        for (;;) {
            /* stop monitoring thread depending on the boolean control member variable*/
            if (!m_bDoMonitoring) {
                return;
            }

            char sPos[10];
            char sVel[10];

            m_pEDIMngr->DisplayThread(monitorData);

            double dPos = monitorData.pos;
            sprintf(sPos, "%f", dPos);
            double dVel = monitorData.vel;
            sprintf(sVel, "%f", dVel);

            char *sStatusMoving  = monitorData.status.drive.moving ? "Moving" : "Not Moving";
            char *sStatusWarning = monitorData.status.drive.warning ? "Warning" : "No Warning";
            char *sStatusError   = monitorData.status.drive.error ? "Error" : "No Error";
            char *sStatusPwrOn   = monitorData.status.drive.powerOn ? "Drive On" : "Drive Off";

            if (m_bDoMonitoring) {
                m_CPosEdit.SetWindowText(CString(sPos));
                m_CVelEdit.SetWindowText(CString(sVel));
                m_CStatusMovingEdit.SetWindowText(CString(sStatusMoving));
                m_CStatusWarningEdit.SetWindowText(CString(sStatusWarning));
                m_CStatusErrorEdit.SetWindowText(CString(sStatusError));
                m_CStatusPwrOnEdit.SetWindowText(CString(sStatusPwrOn));
            }
        }
    }

    catch (const exception &e) {
        char strException[1024];
        strcpy(strException, e.what());
        AfxMessageBox(CString(strException), MB_OK | MB_ICONERROR);
    }
}

void CSimpleTestDlg::CleanMonitoring()
{
    m_CPosEdit.SetWindowText(CString("-"));
    m_CVelEdit.SetWindowText(CString("-"));
    m_CStatusErrorEdit.SetWindowText(CString("-"));
    m_CStatusWarningEdit.SetWindowText(CString("-"));
    m_CStatusMovingEdit.SetWindowText(CString("-"));
    m_CStatusPwrOnEdit.SetWindowText(CString("-"));
}