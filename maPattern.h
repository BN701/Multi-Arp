//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef MAPATTERN_H_INCLUDED
#define MAPATTERN_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maNotes.h"
#include "maTranslateTable.h"

struct Pattern
{
    std::vector<StepList>::size_type m_Pos;
    std::vector<StepList>::size_type m_PosEdit;
    std::vector<RealTimeList>::size_type m_PosRealTimeEdit;
    std::vector<StepList>::size_type m_LastRequestedPos;

    std::vector<int> m_Trigs;

    std::vector<StepList> m_StepListSet;
    std::vector<RealTimeList> m_RealTimeSet;

    std::string m_Label;

    double m_StepValue;
    double m_Gate;
    bool m_GateHold;
    unsigned char m_Velocity;
    TranslateTable m_TranslateTable;
    FeelMap m_FeelMap;

    void StoreStepValue( double val );
    void StoreGate( double gate);
    void StoreGateHold( bool val );
    void StoreVelocity( unsigned char vel );
    void StoreTranslateTable( TranslateTable & table );

    double StepValue() { return m_StepValue; }
    double Gate() { return m_Gate; }
    bool GateHold() { return m_GateHold; }
    unsigned char Velocity() { return m_Velocity; }
    TranslateTable & PatternTranslateTable() { return m_TranslateTable; }
    FeelMap & PatternFeelMap() { return m_FeelMap; }

    Pattern():
        m_Pos(0),
        m_PosEdit(0),
        m_PosRealTimeEdit(0),
        m_LastRequestedPos(0),
        m_StepValue(16),
        m_Gate(0.5),
        m_GateHold(false),
        m_Velocity(64)
    {
        // NewList();
    }

    void Clear ()
    {
        m_Label.clear();
        m_Trigs.clear();
        m_StepListSet.clear();
        ResetPosition();
        m_TranslateTable.Reset();
        m_StepValue = 4.0;
        m_Gate = 0.5;
        m_GateHold = false;
        m_Velocity = 64;
    }

    void SetEditPos( std::vector<int>::size_type p )
    {
        if ( p >= 0 && p < m_StepListSet.size() )
            m_PosEdit = p;
    }

    void SetRealTimeEditPos( std::vector<int>::size_type p )
    {
        if ( p >= 0 && p < m_RealTimeSet.size() )
            m_PosRealTimeEdit = p;
    }

    void SetStepValue( double val )
    {
        m_StepValue = val;
    }

    int ListCount()
    {
        return m_StepListSet.size();
    }

    int RealTimeListCount()
    {
        return m_RealTimeSet.size();
    }

    void ResetPosition()
    {
        m_Pos = 0;
        for ( std::vector<StepList>::iterator i = m_StepListSet.begin(); i != m_StepListSet.end(); i++ )
            (*i).ResetPosition();
    }

    void Step(Cluster & cluster, double phase, double stepValue);

    bool LabelEmpty()
    {
        return m_Label.empty();
    }

    std::string Label(size_t width);
    std::string RealTimeListToStringForDisplay(std::vector<int>::size_type n);
    std::string ListToString(std::vector<int>::size_type n);
    std::string ToString(const char * prefix);
    bool FromString(std::string s, int & updates);
    void AddRealTimeList(std::map<double,Note> realTimeList, double quantum);
    void AddRealTimeListFromString(std::vector<RealTimeList>::size_type index, std::string s);
    void AddListFromString(std::vector<int>::size_type index, std::string s);
    void SetFieldsFromString(std::string s);
    bool PlayPositionInfo(int & listIndex, int & offset, int & length);

    void ReplaceList(StepList & noteList);
    int NewList();
    void DeleteCurrentList();
    void DeleteCurrentRealTimeList();

    bool AllListsComplete();

    void UpEditPos() { SetEditPos( m_PosEdit + 1); }
    void DownEditPos() { SetEditPos( m_PosEdit - 1); }
    void UpRTEditPos() { SetRealTimeEditPos( m_PosRealTimeEdit + 1); }
    void DownRTEditPos() { SetRealTimeEditPos( m_PosRealTimeEdit - 1); }

};



#endif // MAPATTERN_H_INCLUDED