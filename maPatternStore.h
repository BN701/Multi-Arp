//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017, 2018  Barry Neilsen
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

#ifndef PATTERN_STORE_H_INCLUDED
#define PATTERN_STORE_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "maChainLink.h"
#include "maCommand.h"
#include "maItemMenu.h"
#include "maFeelMap.h"
#include "maNotes.h"
#include "maPattern.h"
#include "maPatternChain.h"

// Reset modes.

#define RESET_NONE 0
#define RESET_ALL 1
#define RESET_CURRENT 2

// Patternchain modes

//#define PC_MODE_NONE 0
//#define PC_MODE_NATURAL 1
//#define PC_MODE_FORCED 2

// class State;



struct PatternStore : public ItemMenu
{
    ItemMenu m_MenuListWindow;

    // Yes, we're storing iterators. But these are list iterators, so they stay valid
    // unless a specific item is deleted. (

    std::list<Pattern>::iterator m_PosPlay = m_Patterns.end();
    std::list<Pattern>::iterator m_PosEdit = m_Patterns.end();

    // We still need to track with a numeric counter, sometimes, but never use for pattern lookup.

    int m_PosEditIndex = -1;
//    int m_PosPlayIndex = -1;

    std::map<size_t, std::list<Pattern>::iterator> m_PatternLookup;

//    std::vector<int>::size_type m_PosPatternChain;
//    int m_PatternChainMode;
    bool m_ResetOnPatternChange;
    bool m_PhaseIsZero;
//    bool m_EditPosFollowsPlay;
    bool m_NewPatternPending;
    size_t m_NewPattern;
    bool m_PatternChanged;
    bool m_UsePatternPlayData;

    PatternChain m_PatternChain;

    std::list<Pattern> m_Patterns;
    std::list<Pattern> m_Deleted;

    Pattern m_DefaultPattern; // Somewhere to keep 'global' data when not using a specific pattern.
    double m_StepValueMultiplier = 1.0;

    PatternStore(/*TranslateTable & table*/):
//        m_PosPatternChain(0),
//        m_PatternChainMode(PatternChain::off),
        m_ResetOnPatternChange(true),
        m_PhaseIsZero(false),
//        m_EditPosFollowsPlay(true),
        m_NewPatternPending(false),
        m_PatternChanged(false),
        m_UsePatternPlayData(false)
    {
        m_DisplayObjectType = BaseUI::dot_pattern_store;
        m_MenuListWindow.ClearStatus();
        m_MenuListWindow.SetVisible(true);
        m_MenuListWindow.SetType(BaseUI::dot_pattern_menu_list);
        m_Visible = true;
        SetRedraw();
    };

    bool Empty() { return m_Patterns.empty(); }
    bool PatternChainEmpty() { return m_PatternChain.empty(); }

    std::string PatternStatusPlay();
    std::string PatternStatusEdit();
    std::string PatternOverview();

    int PatternCount() { return m_Patterns.size(); }

//    std::string SetNewPatternOrJump( std::string val );
    void SetNewPatternPending( size_t val );
    bool NewPatternPending(bool clearAndReset = true);

//    void SetEditFocusFollowsPlay(bool bVal)
//    {
//        m_EditPosFollowsPlay = bVal;
//        if ( m_EditPosFollowsPlay )
//            m_PosEdit = m_PosPlay;
//    }

//    bool EditFocusFollowsPlay() { return m_EditPosFollowsPlay; }
    void SetResetOnPatternChange(bool bVal) { m_ResetOnPatternChange = bVal; }
    void SetPatternChainMode(PatternChain::pattern_chain_mode_t bVal) { m_PatternChain.SetMode(bVal); }
    PatternChain::pattern_chain_mode_t PatternChainMode() { return m_PatternChain.Mode(); }
    std::vector<ChainLink>::size_type CurrentPosPatternChain() { return m_PatternChain.PosPlay(); }
//    bool EditPatternIsPlayPattern() { return m_PosEdit == m_PosPlay; }

    void SetStepValCurrentEditPattern(int i)
    {
        if ( !m_Patterns.empty() )
            m_PosEdit->SetStepValue(i);
    }

//    void SetLabelCurrentEditPattern(const char * label)
//    {
//        if ( !m_Patterns.empty() )
//            m_PosEdit->SetLabel(label);
//    }
//
//    double StepValueCurrentPlayPattern()
//    {
//        if ( m_Patterns.empty() )
//            return 0;
//        else
//            return m_PosPlay->StepValue();
//    }

    void SetPhaseIsZero() { m_PhaseIsZero = true; }

    void Step(Cluster & cluster, TrigRepeater & repeater, double phase, double stepValue, double globalBeat);
    double StepValueMultiplier() { return m_StepValueMultiplier; }

    std::string EditPatternToString();

//    std::string PatternSelectionList(unsigned start, unsigned rows);

    std::string PatternChainToString();
    std::string PatternChainToStringForDisplay(int firstRow, int rows);

    std::string ToString();
    bool FromString(std::string s, int & created, int & updates);
    void UpdatePattern(StepList & noteList);
    void UpdatePattern(std::map<double,Note> & realTimeList, double quantum);
#ifndef MA_BLUE
    void UpdatePatternFromMidiFile(std::string s);
#endif
    void SetFieldsFromString(std::string s);
    void UpdatePatternChainFromSimpleString(std::string s); // Old style, from chain command, no support for jumps.
    void UpdatePatternChainFromString(std::string s);

    StepList & CurrentEditStepList();
    RealTimeList & CurrentEditRealTimeList();
    Pattern & CurrentPlayPattern();
    Pattern & CurrentEditPattern();

    const char * CurrentEditPatternID() { return m_PosEdit->ShortLabel(); }
    const char * CurrentPlayPatternID() { return m_PosPlay->ShortLabel(); }

    double LastRealTimeBeat();

    void AddEmptyPattern(std::vector<std::string> & tokens, int firstToken);
    void CopyCurrentPattern();
    void ClearCurrentPattern()
    {
        m_Deleted.push_back(*m_PosEdit);
        m_PosEdit->Clear();
    }

    void DeleteCurrentPattern();
    void DeleteAllPatterns();
    void PopDeletedPattern();

    bool PatternChanged(bool resetValue = false)
    {
        bool result = m_PatternChanged;
        if ( resetValue )
            m_PatternChanged = false;
        return result;
    }

    void SetPlayPattern(size_t patternIdHash);
    void SetEditPos(std::list<Pattern>::iterator it);
    void UpEditPos();
    void DownEditPos();


//    void UpListPos();
//    void DownListPos();
//    void UpRTListPos();
//    void DownRTListPos();

//    bool ValidPosition( std::vector<int>::size_type p )
//    {
//        return p >= 0 && p < m_Patterns.size();
//    }

    void ResetCurrentPattern()
    {
        if ( !m_Patterns.empty() )
            m_PosEdit->ResetPosition();
    }

    void ResetAllPatterns()
    {
        for ( auto it = m_Patterns.begin(); it != m_Patterns.end(); it++ )
            it->ResetPosition();
        m_PatternChain.ResetPosPlay();
//        m_PosPatternChain = 0; // This seems odd, but it's incremented immediately on phase
    }

    void StorePatternPlayData( unsigned char mask = PLAY_DATA_ALL );
    std::string LoadPatternPlayData( unsigned char mask = PLAY_DATA_ALL );
    std::string ShowPatternPlayData();

    void SetStepValue(double step);
    void SetGateLength(double gate);
    void SetGateHold(bool val);
    void SetNoteVelocity(unsigned char vel);

    double StepValue();
    double GateLength();
    bool GateHold();
    unsigned char NoteVelocity();

    TranslateTable & TranslateTableForEdit(bool setFocus = true);
    TranslateTable & TranslateTableForPlay();

    FeelMap & FeelMapForEdit(bool setFocus = true);
    FeelMap & FeelMapForPlay();

    PatternChain & PatternChainForEdit() { return m_PatternChain; }

    void SetUsePatternPlayData( bool val );
    bool UsePatternPlayData() { return m_UsePatternPlayData; }

    std::string ListManager(std::string commandString, std::vector<std::string> & tokens);
    void DeleteCurrentRealTimeList();

    virtual void SetStatus();

    void OpenCurrentItemMenu();

    protected:
        virtual bool HandleKey(BaseUI::key_command_t k);

};

extern PatternStore g_PatternStore;

#endif // PATTERN_STORE_H_INCLUDED
