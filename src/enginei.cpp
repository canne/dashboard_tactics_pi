/******************************************************************************
* $Id: enginei.cpp, v1.0 2019/11/30 VaderDarth Exp $
*
* Project:  OpenCPN
* Purpose:  dahbooard_tactics_pi plug-in
* Author:   Jean-Eudes Onfray
* 
***************************************************************************
*   Copyright (C) 2010 by David S. Register   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
***************************************************************************
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "dashboard_pi.h"
#include "enginei.h"

//************************************************************************************************************************
// Numerical instrument for engine monitoring data
//************************************************************************************************************************

DashboardInstrument_EngineI::DashboardInstrument_EngineI(
                             DashboardWindow *pparent, wxWindowID id, wxString title,
                             wxString path = L"propulsion.port.revolutions", wxString format = L"%4.0f" ) :
    DashboardInstrument_Single(pparent, id, title, 0LL, format )
{
    SetDrawSoloInPane(true);

    m_data = L"---";
    m_pparent = pparent;
    m_path = path;
    // auto m_pushHere = std::bind(&DashboardInstrument_EngineI::PushData,
    //                             (DashboardInstrument_EngineI&) this, _1, _2, _3 );
    std::function<void(double, wxString, long long)>  m_pushHere =
        [this](double data, wxString unit, long long timestamp) {
            if( !std::isnan(data) && (data < 9999.9) ) {
                setTimestamp( timestamp );
                /* Can do all the work here but let's call in interal method to complete: */
                // m_data = wxString::Format(m_format, data) + L" " + unit;
                this->PushData (data, unit, timestamp); // demo demo demo demo
            };
        };
    m_pparent->subscribeTo( path, m_pushHere );
}
DashboardInstrument_EngineI::~DashboardInstrument_EngineI(void)
{
    // m_pparent->unsubscribeFrom( m_path );
    return;
}

void DashboardInstrument_EngineI::SetData(
    unsigned long long st, double data, wxString unit, long long timestamp)
{
    return; // this derived class gets its data from the multiplexer through a callback PushData()
}

void DashboardInstrument_EngineI::PushData( // for demo/testing purposes in this simple instrument
    double data, wxString unit, long long timestamp)
{
    if( !std::isnan(data) && (data < 9999.9) ) {
        m_data = wxString::Format(m_format, data) + L" " + unit;
    } 
}

void DashboardInstrument_EngineI::derivedTimeoutEvent()
{
    m_data = L"---";
}
