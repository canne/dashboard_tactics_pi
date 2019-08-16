/***************************************************************************
* $Id: streamout.h, v1.0 2019/08/08 DarthVader $
*
* Project:  OpenCPN
* Purpose:  dashboard_tactics_pi plug-in streaming out
* Author:   Petri Makijarvi
*       (Inspired by original work from Jean-Eudes Onfray)
***************************************************************************
*   Copyright (C) 2010 by David S. Register                               *
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
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
***************************************************************************
*/

#ifndef __STREAMOUT_H__
#define __STREAMOUT_H__
using namespace std;
#include <mutex>
#include <vector>
#include <queue>


#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/socket.h>
#include <wx/thread.h>

#include "instrument.h"

enum StreamoutSingleStateMachine {
    SSSM_STATE_UNKNOWN, SSSM_STATE_DISPLAYRELAY, SSSM_STATE_INIT, SSSM_STATE_CONFIGURED,
    SSSM_STATE_READY, SSSM_STATE_FAIL };

enum SocketThreadStateMachine {
    STSM_STATE_UNKNOWN, STSM_STATE_INIT, STSM_STATE_ERROR, STSM_STATE_CONNECTING,
    STSM_STATE_READY };


//+------------------------------------------------------------------------------
//|
//| CLASS:
//|    TacticsInstrument_StreamoutSingle
//|
//| DESCRIPTION:
//|    This class creates a single data streamout instrument
//|
//+------------------------------------------------------------------------------
class TacticsInstrument_StreamoutSingle : public DashboardInstrument, public wxThreadHelper
{
public:
	TacticsInstrument_StreamoutSingle(
        wxWindow *pparent, wxWindowID id, wxString title, unsigned long long cap, wxString format,
        std::mutex &mtxNofStreamOut, int &nofStreamOut, wxString &echoStreamerShow, wxString confdir);
	~TacticsInstrument_StreamoutSingle();

	wxSize GetSize(int orient, wxSize hint);
	void SetData(unsigned long long st, double data, wxString unit);

protected:

    class sentenceSchema
    {
    public:
        sentenceSchema(void) {
            stc = wxEmptyString;
            st = 0ULL;
            bStore = false;
            iInterval = 0;
            lastTimeStamp = 0LL;
            sMeasurement = wxEmptyString;
            sProp1 = wxEmptyString;
            sProp2 = wxEmptyString;
            sProp3 = wxEmptyString;
            sField1 = wxEmptyString;
            sField2 = wxEmptyString;
            sField3 = wxEmptyString;
        };
        sentenceSchema( const sentenceSchema& source) {
            stc = source.stc;
            st = source.st;
            bStore = source.bStore;
            iInterval = source.iInterval;
            lastTimeStamp = source.lastTimeStamp;
            sMeasurement = source.sMeasurement;
            sProp1 = source.sProp1;
            sProp2 = source.sProp2;
            sProp3 = source.sProp3;
            sField1 = source.sField1;
            sField2 = source.sField2;
            sField3 = source.sField3;
        };
        const sentenceSchema& operator = (const sentenceSchema &source) {
            if ( this != &source) {
                stc = source.stc;
                st = source.st;
                bStore = source.bStore;
                iInterval = source.iInterval;
                lastTimeStamp = source.lastTimeStamp;
                sMeasurement = source.sMeasurement;
                sProp1 = source.sProp1;
                sProp2 = source.sProp2;
                sProp3 = source.sProp3;
                sField1 = source.sField1;
                sField2 = source.sField2;
                sField3 = source.sField3;
            }
            return *this;
        };
        wxString stc;
        unsigned long long st;
        bool bStore;
        int iInterval;
        long long lastTimeStamp;
        wxString sMeasurement;
        wxString sProp1;
        wxString sProp2;
        wxString sProp3;
        wxString sField1;
        wxString sField2;
        wxString sField3;
    }; // This class presents the elements of the configuration file
    
    class lineProtocol
    {
    public:
        lineProtocol(void) {
            measurement = wxEmptyString;
            tag_key1 = wxEmptyString;
            tag_value1 = wxEmptyString;
            tag_key2 = wxEmptyString;
            tag_value2 = wxEmptyString;
            tag_key3 = wxEmptyString;
            tag_value3 = wxEmptyString;
            field_key1 = wxEmptyString;
            field_value1 = wxEmptyString;
            field_key2 = wxEmptyString;
            field_value2 = wxEmptyString;
            field_key3 = wxEmptyString;
            field_value3 = wxEmptyString;
            timestamp = wxEmptyString;
        };
        lineProtocol( const lineProtocol& source) {
            measurement = source.measurement;
            tag_key1 = source.tag_key1;
            tag_value1 = source.tag_value1;
            tag_key2 = source.tag_key2;
            tag_value2 = source.tag_value2;
            tag_key3 = source.tag_key3;
            tag_value3 = source.tag_value3;
            field_key1 = source.field_key1;
            field_value1 = source.field_value1;
            field_key2 = source.field_key2;
            field_value2 = source.field_value2;
            field_key3 = source.field_key3;
            field_value3 = source.field_value3;
            timestamp = source.timestamp;
        };
        const lineProtocol& operator = (const lineProtocol &source) {
            if ( this != &source) {
                measurement = source.measurement;
                tag_key1 = source.tag_key1;
                tag_value1 = source.tag_value1;
                tag_key2 = source.tag_key2;
                tag_value2 = source.tag_value2;
                tag_key3 = source.tag_key3;
                tag_value3 = source.tag_value3;
                field_key1 = source.field_key1;
                field_value1 = source.field_value1;
                field_key2 = source.field_key2;
                field_value2 = source.field_value2;
                field_key3 = source.field_key3;
                field_value3 = source.field_value3;
                timestamp = source.timestamp;
            }
            return *this;
        };
        wxString measurement;
        wxString tag_key1;
        wxString tag_value1;
        wxString tag_key2;
        wxString tag_value2;
        wxString tag_key3;
        wxString tag_value3;
        wxString field_key1;
        wxString field_value1;
        wxString field_key2;
        wxString field_value2;
        wxString field_key3;
        wxString field_value3;
        wxString timestamp;
    }; // This class presents the line propotocol elements in the data FIFO queue 

    TacticsInstrument_StreamoutSingle *m_frame;
    
    int               m_state;
    std::mutex       *m_mtxNofStreamOut;
    int              *m_nofStreamOut;
    wxString         *m_echoStreamerShow;
    wxString          m_data;
    wxString          m_format;
    int               m_DataHeight;
    wxString          m_confdir;
    wxString          m_configFileName;
    wxFileConfig     *m_pconfig;
    bool              m_configured;

    std::vector<sentenceSchema> vSchema;
    std::queue<lineProtocol> qLine;
    std::mutex        m_mtxQLine;
    int               m_stateComm;
    wxString          m_threadMsg;

    // From configuration file
    wxString          m_server;
    wxString          m_api;
    wxString          m_org;
    wxString          m_bucket;
    wxString          m_precision;
    wxString          m_token;
    int               m_connectionRetry;
    wxString          m_timestamps;
    bool              m_stamp;
    int               m_verbosity;

    bool GetSchema(unsigned long long st, long long msNow, sentenceSchema& schema);
    bool LoadConfig(void);
    void SaveConfig(void);
    void Draw(wxGCDC* dc);
    void OnClose(wxCloseEvent& evt);
    wxThread::ExitCode TacticsInstrument_StreamoutSingle::Entry(void);
    void OnThreadUpdate(wxThreadEvent& evt);
    
private :

    wxDECLARE_EVENT_TABLE();

};

#endif // not defined __STREAMOUT_H__

