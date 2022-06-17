/*
 * TCPIPDlg.cpp
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

#include "TCPIPDlg.h"
#include "SimpleTest.h"
#include "stdafx.h"

// TCPIPDlg dialog

IMPLEMENT_DYNAMIC(TCPIPDlg, CDialog)

TCPIPDlg::TCPIPDlg(CWnd *pParent /*=NULL*/) : CDialog(TCPIPDlg::IDD, pParent), m_sAddress("")
{
}

TCPIPDlg::~TCPIPDlg()
{
}

void TCPIPDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS1, m_CTcpIpCtrl);
}

BEGIN_MESSAGE_MAP(TCPIPDlg, CDialog)
ON_BN_CLICKED(IDOK, &TCPIPDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// TCPIPDlg message handlers

void TCPIPDlg::OnBnClickedOk()
{
    BYTE Bfield0, Bfield1, Bfield2, Bfield3;
    char sfield0[15];
    char sfield1[15];
    char sfield2[15];
    char sfield3[15];

    m_CTcpIpCtrl.GetAddress(Bfield0, Bfield1, Bfield2, Bfield3);

    _itoa((int)(Bfield0), sfield0, 10);
    _itoa((int)(Bfield1), sfield1, 10);
    _itoa((int)(Bfield2), sfield2, 10);
    _itoa((int)(Bfield3), sfield3, 10);

    m_sAddress = (CString(sfield0) + '.' + CString(sfield1) + '.' + CString(sfield2) + '.' + CString(sfield3));

    OnOK();
}
