/*
 * This source file is part of the Atlantis Little Helper program.
 * Copyright (C) 2001 Maxim Shariy.
 *
 * Atlantis Little Helper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Atlantis Little Helper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Atlantis Little Helper; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "stdhdr.h"

#include "wx/spinctrl.h"
#include "wx/dialog.h"


#include "cstr.h"
#include "collection.h"
#include "cfgfile.h"
#include "files.h"
#include "atlaparser.h"
#include "consts_ah.h"
#include "hash.h"
#include "ahapp.h"
#include "ahframe.h"

#include "unitsplitdlg.h"
#include "data_control.h"
#include "ah_control.h"


#define SPACER_GENERIC 5
#define SPACER_NARROW  1

BEGIN_EVENT_TABLE(CUnitSplitDlg, wxDialog)
    EVT_BUTTON  (wxID_CANCEL  ,  CUnitSplitDlg::OnCancel)
    EVT_BUTTON  (wxID_OK      ,  CUnitSplitDlg::OnOk)
END_EVENT_TABLE()



//--------------------------------------------------------------------------

CUnitSplitDlg::CUnitSplitDlg(wxWindow *parent, CUnit * pUnit)
              :CResizableDlg( parent, wxT("Split Unit"), SZ_SECT_WND_SPLIT_UNIT_DLG )
{
    wxBoxSizer      * topsizer;
    wxBoxSizer      * sizer   ;
    wxGridSizer     * gridsizer;
    wxStaticText    * st;
    int               rows, cols, idx;
    wxSpinCtrl      * pSpin;

    m_pUnit = pUnit;
    ScanProperties();

    topsizer = new wxBoxSizer( wxVERTICAL );
    m_btnOk         = new wxButton     (this, wxID_OK     , wxT("Set")    );
    m_btnCancel     = new wxButton     (this, wxID_CANCEL , wxT("Cancel") );
    m_spinUnitCount = new wxSpinCtrl   (this, -1, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000);
    m_textNewCommand= new wxTextCtrl  (this, -1, wxT(""), wxDefaultPosition, wxSize(225,150), wxTE_MULTILINE);

    if (game_control::get_game_config_val<long>(SZ_SECT_COMMON, SZ_KEY_AUTONAMING))
    {
        *m_textNewCommand << "@;; $c\n";
        *m_textNewCommand << "@name unit \"\"\n";
    }

    sizer    = new wxBoxSizer( wxHORIZONTAL );
        sizer->Add(m_spinUnitCount , 0, wxALIGN_CENTER | wxALL , SPACER_GENERIC);
        st = new wxStaticText(this, -1, wxT("Number of new units to form"));
        sizer->Add(st              , 0, wxALIGN_CENTER | wxALL , SPACER_GENERIC);
    topsizer->Add(sizer        , 0, wxALL | wxGROW, SPACER_GENERIC );

    sizer    = new wxBoxSizer( wxHORIZONTAL );
        sizer->Add(m_textNewCommand , 1, wxGROW | wxALL , SPACER_GENERIC);
        st = new wxStaticText(this, -1, wxT("Orders for each new unit"));
        sizer->Add(st              , 0, wxALIGN_CENTER | wxALL , SPACER_GENERIC);
    topsizer->Add(sizer        , 1, wxALL | wxGROW, SPACER_GENERIC );

    st = new wxStaticText(this, -1, wxT("Give each new unit:"));
    topsizer->Add(st              , 0, wxALIGN_LEFT | wxALL , SPACER_GENERIC);

    cols = 2;
    rows = m_SplitControls.Count() / cols;
    while (rows > cols*5)
    {
        cols++;
        rows = m_SplitControls.Count() / cols;
    }
    while (rows*cols < m_SplitControls.Count())
        rows++;

    gridsizer = new wxGridSizer(rows, cols, 3, 3) ;
    for (idx=0; idx<m_SplitControls.Count(); idx++ )
    {
        pSpin = (wxSpinCtrl*)m_SplitControls.At(idx);

        sizer    = new wxBoxSizer( wxHORIZONTAL );
        st       = new wxStaticText(this, -1, pSpin->GetName());
        sizer->Add(st   , 1, wxALIGN_LEFT  | wxALL, SPACER_NARROW );
        sizer->Add(pSpin, 0, wxALL, SPACER_NARROW );

        gridsizer->Add(sizer, 1, wxGROW);
    }
    topsizer->Add(gridsizer, 0, wxALIGN_CENTER );

    sizer    = new wxBoxSizer( wxHORIZONTAL );
        sizer->Add(m_btnOk       , 0, wxALIGN_CENTER);
        sizer->Add(m_btnCancel   , 0, wxALIGN_CENTER | wxALL, SPACER_GENERIC);
    topsizer->Add(sizer, 0, wxALIGN_CENTER );

    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_ALT, 13, wxID_OK);
    entries[1].Set(wxACCEL_CTRL, 13, wxID_OK);
    wxAcceleratorTable accel(2, entries);
    this->SetAcceleratorTable(accel);

    SetAutoLayout( TRUE );     // tell dialog to use sizer
    SetSizer( topsizer );      // actually set the sizer
    topsizer->Fit( this );            // set size to minimum size as calculated by the sizer
    topsizer->SetSizeHints( this );   // set size hints to honour mininum size}

    m_spinUnitCount->SetFocus();
    // m_btnOk->SetDefault(); // Commented because it steals 'enter/return' from the multiline wxTextCtrl.

    CResizableDlg::SetPos();
}


//--------------------------------------------------------------------------

CUnitSplitDlg::~CUnitSplitDlg()
{
    m_SplitControls.DeleteAll();
}

//--------------------------------------------------------------------------

void CUnitSplitDlg::ScanProperties()
{
    // make a spin control for each property

    int             propidx;
    CStr            propname;
    EValueType      type;
    const void    * value;
    wxSpinCtrl    * pSpin;

    propidx  = 0;
    propname = m_pUnit->GetPropertyName(propidx);
    while (!propname.IsEmpty())
    {
        if (m_pUnit->GetProperty(propname.GetData(), type, value, eOriginal) &&
            eLong==type &&
            !IsASkillRelatedProperty(propname.GetData()) &&
            0!=stricmp(PRP_SEQUENCE     , propname.GetData()) &&
            0!=stricmp(PRP_GUI_COLOR    , propname.GetData()) &&
            0!=stricmp(PRP_STRUCT_ID    , propname.GetData())&&
            0!=stricmp(PRP_FRIEND_OR_FOE, propname.GetData())
           )
        {
            pSpin = new wxSpinCtrl   (this, -1, wxT("1"));
            pSpin->SetRange(0, 0x7fffffff);
            pSpin->SetValue(0);
            pSpin->SetName(wxString::FromAscii(propname.GetData()));

            m_SplitControls.AtInsert(m_SplitControls.Count(), pSpin);
        }
        propname = m_pUnit->GetPropertyName(++propidx);
    }
}

//--------------------------------------------------------------------------

void CUnitSplitDlg::OnOk(wxCommandEvent& )
{
    CLand * pLand;
    int     id,i,idx;
    CStr    S, Cmd;
    wxSpinCtrl * pSpin;
    wxString     sBoo;
    const char * p;

    pLand = gpApp->m_pAtlantis->GetLand(m_pUnit->LandId);
    if (pLand)
        id = pLand->GetNextNewUnitNo();
    else
        id = 1;

    sBoo = m_textNewCommand->GetValue();
    p = sBoo.mb_str();
    while (p && *p)
    {
        p = SkipSpaces(S.GetToken(p, '\n', TRIM_ALL));
        Cmd << S << EOL_SCR;
    }

    m_pUnit->Orders.TrimRight(TRIM_ALL);
    if (!m_pUnit->Orders.IsEmpty())
        m_pUnit->Orders << EOL_SCR;

    for (i=0; i<m_spinUnitCount->GetValue(); i++)
    {
        CUnit * pUnitNew = gpApp->m_pAtlantis->SplitUnit(m_pUnit, id+i);
        if (pUnitNew)
            pUnitNew->Orders << Cmd;

        for (idx=0; idx<m_SplitControls.Count(); idx++ )
        {
            pSpin = (wxSpinCtrl*)m_SplitControls.At(idx);

            if (pSpin->GetValue() > 0)
                m_pUnit->Orders << "GIVE NEW " << (long)(id+i) << " " << (long)pSpin->GetValue() << " " << pSpin->GetName().mb_str() << EOL_SCR;
        }
    }

    if (pLand)
        gpApp->m_pAtlantis->RunOrders(pLand);

    StoreSize();
    EndModal(wxID_OK);
}

//--------------------------------------------------------------------------

void CUnitSplitDlg::OnCancel(wxCommandEvent& )
{
    StoreSize();
    EndModal(wxID_CANCEL);
}

//--------------------------------------------------------------------------

