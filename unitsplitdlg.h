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

#ifndef __AH_SPLIT_UNIT_DIALOG_INCL__
#define __AH_SPLIT_UNIT_DIALOG_INCL__



class CUnitSplitDlg : public CResizableDlg
{
public:
    CUnitSplitDlg(wxWindow *parent, CUnit * pUnit);
    ~CUnitSplitDlg();

private:
    void ScanProperties();

    void OnCancel       (wxCommandEvent& event);
    void OnOk           (wxCommandEvent& event);

    CUnit         * m_pUnit         ;
    CLongColl     m_SplitControls   ;

    wxButton      * m_btnOk         ;
    wxButton      * m_btnCancel     ;
    wxSpinCtrl    * m_spinUnitCount ;
    wxTextCtrl    * m_textNewCommand;

    DECLARE_EVENT_TABLE()
};

#include <vector>
#include <unordered_map>
#include <string>
#include <wx/checkbox.h>


class CCreateNewUnit : public CResizableDlg
{
    CUnit* unit_;
    CLand* land_;

    //naming section
    wxTextCtrl* text_alias_;
    wxTextCtrl* text_name_;
    wxTextCtrl* text_loc_description_;
    wxTextCtrl* text_description_;

    //flags section
    wxCheckBox* flag_behind_;
    wxCheckBox* flag_avoid_;
    wxCheckBox* flag_hold_;
    wxCheckBox* flag_noaid_;
    wxCheckBox* flag_guard_;
    wxCheckBox* flag_nocross_;

    wxRadioBox* radiobox_flag_reveal_;
    wxRadioBox* radiobox_flag_spoils_;
    wxRadioBox* radiobox_flag_consume_;

    //buy section
    wxSpinCtrl* spin_buy_units_amount_;
    wxComboBox* combobox_buy_units_type_;
    wxCheckBox* flag_buy_repeating_;
    wxCheckBox* flag_buy_all_;

public:
    //flags consist of command and it's possible values
    CCreateNewUnit(wxWindow *parent, CUnit * pUnit, CLand* pLand);
    ~CCreateNewUnit();

private:
    void OnCancel       (wxCommandEvent& event);
    void OnOk           (wxCommandEvent& event);

    CLongColl     m_SplitControls   ;
    wxSpinCtrl    * m_spinUnitCount ;
    wxTextCtrl    * m_textNewCommand;

    DECLARE_EVENT_TABLE()
};

#endif
