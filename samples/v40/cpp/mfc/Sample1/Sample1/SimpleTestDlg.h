/*
 * SimpleTestDlg.h
 *
 * Copyright (c) 2012-2021 ETEL SA. All Rights Reserved.
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
 *
 */


#pragma once
#include "afxwin.h"
#include "EDIManager.h"

#include <exception>
#include <dsa40.h>  /* EDI high level library version 3.0x.  */


// CSimpleTestDlg dialog
class CSimpleTestDlg : public CDialog
{
// Construction
public:
	CSimpleTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIMPLE_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	CButton m_CMinBtn;
	CButton m_CMinusBtn;
	CButton m_CPlusBtn;
	CButton m_CMaxBtn;
	CButton m_CDisconnectBtn;
	CButton m_CConnectBtn;
	CButton m_COnBtn;
	CButton m_COffBtn;
	CButton m_CHomeBtn;

	CEdit m_CPosEdit;
	CEdit m_CVelEdit;
	CEdit m_CStatusMovingEdit;
	CEdit m_CStatusWarningEdit;
	CEdit m_CStatusErrorEdit;
	CEdit m_CStatusPwrOnEdit;

	CComboBox CComboConnect;
	EDIManager* m_pEDIMngr;

	ebool m_bHomingDone;
	/*
	 * Synchronisation between main thread and monitoring thread:
	 * To tell the monitoring thread whether to fetch and display the data or not.
	 */
	ebool m_bDoMonitoring;

	static UINT MonitoringThread(LPVOID pParam);
	void MonitoringThread();
	void CleanMonitoring();

public:
	
	CString	m_sTCPIP;

	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnCbnSelchangeComboConnect();
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnBnClickedButtonMax();
	afx_msg void OnBnClickedButtonDisconnect();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();

};
