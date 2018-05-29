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

#include "maPatternStore.h"

#if defined(MA_BLUE)
#include <cstdio>
#include <cstdlib>
#endif

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
#define U_FORMAT    "%u"
#else
#define U_FORMAT    "%lu"
#endif


#include <algorithm>
#include <cstring>
#include <unordered_map>

#include "maUtility.h"
#include "maState.h"

using namespace std;

//// Static instances of empty items
//// for use by member functions that
//// return references when no patterns
//// or other items have been created.
//
//StepList StepList::EmptyList;
//RealTimeList RealTimeList::EmptyList;

PatternStore g_PatternStore;
Pattern Pattern::EmptyPattern;
//ItemMenu PatternStore::m_MenuListWindow;

void PatternStore::SetStatus()
{
    int pos = 0;

    m_FieldPositions.clear();
    m_Highlights.clear();

    InitStatus();
    if ( m_GotFocus )
        m_Status += "[Patterns] ";
    else
        m_Status += " Patterns  ";

    for ( auto it = m_Patterns.begin(); it != m_Patterns.end(); it++ )
    {
//        if ( i > 0 )
//            m_Status += ",";
        if ( m_NewPatternPending && it->PatternID() == m_NewPattern )
            m_Status += '+';
        else if ( it == m_PosPlay )
            m_Status += '>';
        else
            m_Status += ' ';

        pos = m_Status.size();
        m_Status += it->ShortLabel();
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( m_GotFocus && !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEditIndex));
}


bool PatternStore::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
        if ( !m_Patterns.empty() )
        {
            MenuList & m = m_PosEdit->m_MenuList;
            if ( m.m_Items.empty() )
                return true;
            if ( m.m_Cursor == m.m_Items.end() )
                m.Select(m.m_Items.begin());
            else
            {
                (*m.m_Cursor)->SetFocus();
                (*m.m_Cursor)->SetReturnFocus(this);
            }
        }
        break;

    case BaseUI::key_cmd_left:
        UpEditPos();
        break;

    case BaseUI::key_cmd_right:
        DownEditPos();
        break;

    case BaseUI::key_up:
//        UpEditPos();
//        DownListPos();
//        SetRedrawMenuList();
        break;

    case BaseUI::key_down:
//        DownEditPos();
//        UpListPos();
//        SetRedrawMenuList();
        break;

    default:
        return false;
    }

//    SetStatus();

    return true;
}

const char * numbers[] = {"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};

void PatternStore::AddEmptyPattern(vector<std::string> & tokens, int firstToken)
{
//    auto pos = m_Patterns.insert(m_Patterns.end(), m_DefaultPattern);

//    m_Patterns.emplace_back();
//    if ( m_Patterns.empty() )
//        return;

    auto pos = m_Patterns.emplace(m_Patterns.end());

    *pos = m_DefaultPattern;

    string label;

    if ( ! tokens.empty() && firstToken >= 0 )
    {
        auto token = tokens.begin() + firstToken;
        while ( token != tokens.end() )
        {
            label += *token;
            if ( ++token == tokens.end() )
                break;
            label += ' ';
        }
    }
    else
    {
        // Automatic label name.

//        int n = p.SetPatternID();
        int n = pos->SetPatternID();
        while ( n > 0 )
        {
            label.insert(0, numbers[n % 10]);
            n /= 10;
        }
    }

//    p.SetShortLabel();
//    p.SetLabel(label.c_str());
//    p.SetReturnFocus(this);
    pos->SetShortLabel();
    pos->SetLabel(label.c_str());
    pos->SetReturnFocus(this);

//    if ( m_EditPosFollowsPlay )
//        return m_Patterns.size() - 1;
//    else
//        return m_PosEdit = m_Patterns.size() - 1;

//    unsigned pos = m_Patterns.size() - 1;

    SetEditPos(pos);
    SetRedraw();

//    return pos;

}

void PatternStore::CopyCurrentPattern()
{
//    m_Patterns.push_back(*m_PosEdit);
//    SetRedraw();
//    m_Patterns.back().ResetPosition();
//    m_Patterns.back().SetShortLabel();
//    m_Patterns.back().SetLabel((m_Patterns.back().Label() + ", copy").c_str());
//
//    if ( m_EditPosFollowsPlay )
//        return m_Patterns.size() - 1;
//    else
//        return m_PosEdit = m_Patterns.size() - 1;
//    auto pos = m_Patterns.insert(m_Patterns.end(), *m_PosEdit);

    auto pos = m_PosEdit;
    pos = m_Patterns.emplace(++pos);
    *pos = *m_PosEdit;
    pos->ResetPosition();
    pos->SetShortLabel();
    pos->SetLabel((m_Patterns.back().Label() + ", copy").c_str());

//    if ( ! m_EditPosFollowsPlay )
    SetEditPos(pos);
    SetRedraw();
}

#define DEBUG_PATTERN_LIST

void PatternStore::DeleteCurrentPattern()
{
    if ( m_Patterns.empty() )
        return;

    bool resetPlayPos = m_PosEdit == m_PosPlay;

    m_Deleted.push_back(*m_PosEdit);
    auto pos = m_Patterns.erase(m_PosEdit);

    if ( m_Patterns.empty() )
    {
        m_MenuListWindow.SetRedraw();
        SetRedraw();
        m_PosEdit = pos;
        m_PosPlay = pos;
        m_PosEditIndex = -1;
//        m_PosPlayIndex = -1;
    }
    else
    {
        m_PosEdit = pos;
        m_PosEditIndex = distance(m_Patterns.begin(), pos);

        if ( resetPlayPos )
        {
            SetNewPatternPending(m_PosEdit->PatternID());
        }

//        // If the play pointer is above the pattern that was deleted,
//        // move it down to keep it with the pattern it points at.
//        //
//        // Or, if the play pointer was pointing at the last pattern in
//        // the list and that was deleted, it needs to point to the item
//        // that's now at the end of the list.
//        //
//        // (If it was pointing at the pattern that was deleted, it now
//        // points to the one that took its place.)
//
//        if ( m_PosPlay > m_PosEdit || m_PosPlay == m_Patterns.size() )
//            m_PosPlay -= 1;
//
//        // The edit pointer stays in place and now points to next in
//        // list (unless it was already at the end of the list).
//
//        if ( m_PosEdit == m_Patterns.size() )
//            m_PosEdit -= 1;
//
//        for ( auto p = m_PosEdit; p < m_Patterns.size(); p++ )
//            m_Patterns[p].ResetMenuList();
//
//        SetEditPos(m_PosEdit);
    }

    SetRedraw();
}

void PatternStore::DeleteAllPatterns()
{
    while ( !m_Patterns.empty() )
    {
        m_Deleted.push_back(m_Patterns.back());
        m_Patterns.erase(m_Patterns.end());
    }

    m_PosEdit = m_Patterns.end();
    m_PosPlay = m_Patterns.end();
    m_PosEditIndex = -1;
}

void PatternStore::PopDeletedPattern()
{
    if ( m_Deleted.empty() )
        return;

    auto pos = m_Patterns.insert(m_Patterns.end(), m_Deleted.back());
    m_Deleted.pop_back();

    SetEditPos(pos);

//    if ( !m_EditPosFollowsPlay )
//        m_PosEdit = m_Patterns.size() - 1;
}


// TODO:LG
//StepList & PatternStore::CurrentEditStepList()
//{
//#ifdef MA_BLUE
//    if ( m_Patterns.empty() )
//        return StepList::EmptyList;
//    if ( m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
//        return StepList::EmptyList;
//#else
//    if ( m_Patterns.empty() )
//        throw string("Pattern Store is Empty.");
//    if ( m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
//        throw string("Current pattern has no lists.");
//#endif
//
//    size_t pos = m_Patterns.at(m_PosEdit).m_PosEdit;
//    return m_Patterns.at(m_PosEdit).m_StepListSet.at(pos);
//}
//
//RealTimeList & PatternStore::CurrentEditRealTimeList()
//{
//#ifdef MA_BLUE
//    if ( m_Patterns.empty() )
//        return RealTimeList::EmptyList;
//    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
//        return RealTimeList::EmptyList;
//#else
//    if ( m_Patterns.empty() )
//        throw string("Pattern Store is Empty.");
//    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
//        throw string("Current pattern has no real time lists.");
//#endif
//    size_t pos = m_Patterns.at(m_PosEdit).m_PosRealTimeEdit;
//    return m_Patterns.at(m_PosEdit).m_RealTimeSet.at(pos);
//}

double PatternStore::LastRealTimeBeat()
{
    if ( m_Patterns.empty() )
        return 0;
    else
        return m_PosPlay->LastRealTimeBeat();
}


Pattern & PatternStore::CurrentPlayPattern()
{
    if ( m_Patterns.empty() )
#ifdef MA_BLUE
        return Pattern::EmptyPattern;
#else
        throw string("Pattern Store is Empty.");
#endif

    return *m_PosPlay;
}

Pattern & PatternStore::CurrentEditPattern()
{
    if ( m_Patterns.empty() )
#ifdef MA_BLUE
        return Pattern::EmptyPattern;
#else
        throw string("Pattern Store is Empty.");
#endif

    return *m_PosEdit;
}

//void PatternStore::UpListPos()
//{
//    if ( !m_Patterns.empty() )
//        m_Patterns[m_PosEdit].UpCursorPos();
//}
//
//void PatternStore::DownListPos()
//{
//    if ( !m_Patterns.empty() )
//        m_Patterns[m_PosEdit].DownCursorPos();
//}
//
//void PatternStore::UpRTListPos()
//{
//    if ( !m_Patterns.empty() )
//        m_Patterns[m_PosEdit].UpRTEditPos();
//}
//
//void PatternStore::DownRTListPos()
//{
//    if ( !m_Patterns.empty() )
//        m_Patterns[m_PosEdit].DownRTEditPos();
//}

void PatternStore::SetNewPatternPending( int val )
{
    m_NewPattern = val;
    m_NewPatternPending = true;

    // Calculate end beat for current pattern and
    // tell it to when to stop.

    // g_State.NextPhaseZero() is beat - phase + quantum.

    double lastBeat = g_State.NextPhaseZero();
    for ( auto it = m_Patterns.begin(); it != m_Patterns.end(); it++ )
        it->StopAllListGroups(lastBeat);

    SetRedraw();
}


string PatternStore::SetNewPatternOrJump( int val )
{
    if ( m_PatternChain.Mode() == PatternChain::off )
    {
        if ( val >= 0 && static_cast<unsigned>(val) < m_Patterns.size() )
        {
            SetNewPatternPending(val);
            return "Cueing pattern %i";
        }
        else
#ifdef MA_BLUE
            return "Requested pattern doesn't exist!";
#else
            throw string("Requested pattern doesn't exist!");
#endif
    }
    else
    {
        if ( val >= 0 && static_cast<unsigned>(val) < m_PatternChain.size() )
        {
            m_PatternChain.at(m_PatternChain.PosPlay()).ClearRemaining();
            m_PatternChain.SetJumpOverride(val);
            return "Jumping to chain step %i";
        }
        else
#ifdef MA_BLUE
            return "Jump stage doesn't exist!";
#else
            throw string("Jump stage doesn't exist!");
#endif
    }
}

void PatternStore::SetPlayPos( std::list<Pattern>::iterator p )
{
    if ( p == m_Patterns.end() )
        return;

    m_PosPlay = p;
    m_PosPlay->RunAllListGroups(g_State.Beat());
//    if ( m_EditPosFollowsPlay /*&& m_PatternChainMode == PC_MODE_NONE*/ )
//        m_PosEdit = m_PosPlay;
    m_PatternChanged = true; // Cleared again at the start of Step() ..

    if ( m_ResetOnPatternChange )
        m_PosPlay->ResetPosition();

}

void PatternStore::SetEditPos( std::list<Pattern>::iterator p )
{
    if ( m_PosEdit != m_Patterns.end() )
        m_PosEdit->SetVisible(false);
    m_PosEdit = p;
    m_PosEditIndex = distance(m_Patterns.begin(), m_PosEdit);
//    m_EditPosFollowsPlay = false;
    SetRedraw();
    m_PosEdit->SetVisible(true);
    m_PosEdit->SetRedraw();
}

void PatternStore::UpEditPos()
{
    // Move towards beginning of pattern list.
    if ( m_PosEditIndex > 0 )
    {
        auto pos = m_PosEdit;
        SetEditPos(--pos);
    }
}

void PatternStore::DownEditPos()
{
    // Move towards end of pattern list.
    if ( m_PosEditIndex < m_Patterns.size() - 1 )
    {
        auto pos = m_PosEdit;
        SetEditPos(++pos);
    }
}

bool PatternStore::NewPatternPending(bool clearAndReset)
{
    if ( !clearAndReset )
        return m_NewPatternPending;

    if ( m_NewPatternPending )
    {
        // Todo: Pattern Lookup
//        SetPlayPos(m_NewPattern);
        m_NewPatternPending = false;
        SetRedraw();
        return true;
    }
    else
        return false;
}


string PatternStore::PatternChainToStringForDisplay(int firstRow, int rows)
{
    if ( m_Patterns.empty() )
        return "No patterns loaded.\n";

    return m_PatternChain.ToStringForDisplay(firstRow, rows);

}

string PatternStore::PatternStatusPlay()
{

    if ( m_Patterns.empty() )
        return "No patterns loaded.\n";

    string result;
    char buf[80];
#if defined(MA_BLUE)
    const char * format = "Play: %u";
#else
    const char * format = "Play: %lu";
#endif

    snprintf(buf, 80, format, m_PosPlay->PatternID());
    result += buf;

#if defined(MA_BLUE)
    const char * format2 = ", Chain: %c [%u/%u]";
#else
    const char * format2 = ", Chain: %c [%lu/%lu]";
#endif

    switch ( m_PatternChain.Mode() )
    {
        case PatternChain::off :
            strcpy(buf, ", Chain: Off");
            break;

        case PatternChain::natural :
            snprintf(buf, 80, format2, 'N', m_PatternChain.PosPlay() + 1, m_PatternChain.size());
            break;

        case PatternChain::quantum :
            snprintf(buf, 80, format2, 'Q', m_PatternChain.PosPlay() + 1, m_PatternChain.size());
            break;
        default:
            break;
    }

    result += buf;

    return result;
}


string PatternStore::PatternStatusEdit()
{
    if ( m_Patterns.empty() )
        return "";

    string result;
    char buf[80];

    snprintf(buf, 80, "Edit: " U_FORMAT, m_PosEdit->PatternID());
    result += buf;

//    if (  ! m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
//    if (  ! m_Patterns.at(m_PosEdit).m_ListGroups.empty() )
//    {
//        snprintf(buf, 80, ", List " U_FORMAT, m_Patterns.at(m_PosEdit).m_PosEdit + 1);
//        result += buf;
//
////        int listCount = m_Patterns.at(m_PosEdit).m_StepListSet.size();
//        int listCount = m_Patterns.at(m_PosEdit).m_ListGroups.size();
//        if ( listCount > 1 )
//        {
//            snprintf(buf, 80, " of %i", listCount);
//            result += buf;
//        }
//    }

//    if ( m_EditPosFollowsPlay )
//        result += ", following play pos";

    return result;
}


string PatternStore::PatternOverview()
{
    char buff[200];

//    snprintf(buff, 200, "Patterns: %i, Edit %s, Global Vel/Gate: %i/%.0f%% (Hold %s)",
    snprintf(buff, 200, "Patterns: %i, Global Vel/Gate: %i/%.0f%% (Hold %s)",
               PatternCount(),
//               EditFocusFollowsPlay() ? "locked" : "unlocked",
               m_DefaultPattern.Velocity(),
               m_DefaultPattern.Gate() * 100,
               m_DefaultPattern.GateHold() ? "on" : "off");

    return buff;
}

void PatternStore::Step(Cluster & cluster, TrigRepeater & repeater, double phase, double stepValue, double globalBeat)
{
    /*
        As long as PatternChanged() is called for every step, we
        don't need to clear the flag here.
     */

    if ( m_Patterns.empty() )
        return;

    if ( m_PatternChain.Mode() != PatternChain::off && ! m_PatternChain.empty() )
    {
        bool changePattern = false;
        switch ( m_PatternChain.Mode() )
        {
            case PatternChain::natural :
                if ( m_PosPlay->AllListsComplete() )
                    changePattern = true;
                break;

            case PatternChain::quantum :
                if ( m_PhaseIsZero )
                    changePattern = true;
                break;
            default:
                break;
        }

        while ( changePattern ) // Not a loop, just something we can 'break' out of.
        {
            int next = m_PatternChain.JumpOverride();

            // TODO: Can probably do this internally to PatternChain at some point.

            if ( next < 0 && m_PatternChain.at(m_PatternChain.PosPlay()).Remaining() > 0 )
                break;

            if ( next < 0 )
                next = m_PatternChain.at(m_PatternChain.PosPlay()).Jump();

            if ( next < 0 )
                next = m_PatternChain.PosPlay() + 1;

            if ( static_cast<unsigned>(next) >= m_PatternChain.size() )
                next = 0;

            m_PatternChain.SetPosPlay(next);

            // Don't change play pointer if new pattern out of range.

            unsigned pos = m_PatternChain.at(m_PatternChain.PosPlay()).Pattern();

            // Todo: Pattern Lookup
            if ( pos < m_Patterns.size() )
            {
//                m_PosPlay = pos;
//                if ( m_EditPosFollowsPlay )
//                    m_PosEdit = m_PosPlay;
            }

            m_PatternChanged = true;

            if ( m_ResetOnPatternChange )
                m_PosPlay->ResetPosition();

            break;
        }
    }

    m_PhaseIsZero = false;

    m_PosPlay->Step(cluster, repeater, m_StepValueMultiplier, phase, stepValue, globalBeat);
}

void PatternStore::UpdatePatternChainFromSimpleString(string s)
{
    m_PatternChain.FromSimpleString(s);
    m_PatternChain.ResetPosPlay();
}

void PatternStore::UpdatePatternChainFromString(string s)
{
    m_PatternChain.FromString(s);
    m_PatternChain.ResetPosPlay();
}

string PatternStore::PatternChainToString()
{
    return m_PatternChain.ToString();
}

string PatternStore::EditPatternToString()
{
    if ( m_Patterns.empty() )
        return "";
    return m_PosEdit->ToString("Pattern");
}

//string PatternStore::PatternSelectionList(unsigned start, unsigned rows)
//{
//    string result;
//
//    if ( m_Patterns.empty() )
//        return result;
//
//    for ( size_t pos = start; pos < start + rows && pos < m_Patterns.size(); pos++ )
//    {
//        char buff[50];
//#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
//        snprintf(buff, 50, " %02u ", pos + 1);
//#else
//        snprintf(buff, 50, " %02lu ", pos + 1);
//#endif
//
//
//        result += buff;
//        result += pos->Label(15);
//        result += '\n';
//    }
//
//    return result;
//}

void PatternStore::UpdatePattern(StepList & noteList)
{
    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    m_PosEdit->ReplaceList(noteList);
}

void PatternStore::UpdatePattern(std::map<double,Note> & realTimeList, double quantum)
{
    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    m_PosEdit->AddRealTimeList(realTimeList, quantum);
}

#ifndef MA_BLUE
void PatternStore::UpdatePatternFromMidiFile(string s)
{
    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    m_PosEdit->AddRealTimeListFromMidiFile(s);
}
#endif

enum ps_element_names_t
{
    ps_heading,
    ps_name_pattern_chain_mode,
    ps_name_reset_on_pattern_change,
    ps_name_edit_focus_follows_play,
    ps_name_use_pattern_play_data,
    num_ps_element_names
};

#if defined(MA_BLUE)
unordered_map<int, const char *> ps_element_names =
#else
unordered_map<ps_element_names_t, const char *> ps_element_names =
#endif
{
    {ps_heading, "Pattern Store"},
    {ps_name_pattern_chain_mode, "Pattern Chain Mode"},
    {ps_name_reset_on_pattern_change, "Reset on pattern change"},
    {ps_name_edit_focus_follows_play, "Edit focus follows play"},
    {ps_name_use_pattern_play_data, "Use Pattern Play Data"}
};


string PatternStore::ToString()
{
    char buff[100];
    string result;

    result += "<< Pattern Store >>\n\n";
    result += PatternChainToString();
    result += "\n\n";

    snprintf(buff, 100, "Store %s %i\n", ps_element_names.at(ps_name_pattern_chain_mode), static_cast<int>(m_PatternChain.Mode()));
    result += buff;
    snprintf(buff, 100, "Store %s %s\n", ps_element_names.at(ps_name_reset_on_pattern_change), m_ResetOnPatternChange ? "On" : "Off");
    result += buff;
//    snprintf(buff, 100, "Store %s %s\n", ps_element_names.at(ps_name_edit_focus_follows_play), m_EditPosFollowsPlay ? "On" : "Off");
//    result += buff;
    snprintf(buff, 100, "Store %s %s\n", ps_element_names.at(ps_name_use_pattern_play_data), m_UsePatternPlayData ? "On" : "Off");
    result += buff;
    result += '\n';

    result += "<< Pattern Default >>\n\n";
    result += m_DefaultPattern.ToString("Default");
    result += "\n\n";

    for ( auto it = m_Patterns.begin(); it != m_Patterns.end(); it++ )
    {
        snprintf(buff, 100, "<< Pattern %i >>\n\n", it->PatternID());
        result += buff;
        result += it->ToString("Pattern");
        result += "\n";
    }

    return result;
}

void PatternStore::SetFieldsFromString(string s)
{
    for ( ps_element_names_t e = static_cast<ps_element_names_t>(1);
          e < num_ps_element_names;
          e = static_cast<ps_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(s, ps_element_names.at(e));

        if ( token.empty() )
            continue;

        transform(token.begin(), token.end(), token.begin(), ::tolower);

#ifndef MA_BLUE
        try
        {
#endif
            switch (e)
            {
            case ps_name_pattern_chain_mode:
                m_PatternChain.SetMode(static_cast<PatternChain::pattern_chain_mode_t>(strtol(token.c_str(), NULL, 0)));
                break;
            case ps_name_reset_on_pattern_change:
                m_ResetOnPatternChange = token == "on";
                break;
//            case ps_name_edit_focus_follows_play:
//                m_EditPosFollowsPlay = token == "on";
//                break;
            case ps_name_use_pattern_play_data:
                m_UsePatternPlayData = token == "on";
                break;
            default:
                break;
            }
#ifndef MA_BLUE
        }
        catch(invalid_argument ex)
        {

        }
        catch(out_of_range ex)
        {
        }
#endif
    }
}


bool PatternStore::FromString(string s, int & created, int & updates)
{
#ifndef MA_BLUE
    try
    {
#endif
        if ( s.find("<< Pattern Store >>") == 0 )
        {
            // Do nothing, it's just a section heading for readability.
            return true;
        }
        else if ( s.find("<< Pattern Default >>") == 0 ) // Add pattern to end of list.
        {
            // Do nothing, it's just a section heading for readability.
            return true;
        }
        else if ( s.find("<< Pattern >>") == 0 ) // Add pattern to end of list.
        {
//            m_Patterns.emplace_back();
//            m_PosEdit = m_Patterns.size() - 1;
//            created += 1;
            return true;
        }
        else if ( s.find("<< Pattern ") == 0 ) // Create pattern with specific ID
        {
//            size_t index = strtol(s.substr(11).c_str(), NULL, 0) - 1;
//            if ( m_Patterns.size() < index + 1)
//                m_Patterns.resize(index + 1);
//            m_PosEdit = index;
//            created = m_Patterns.size();
            return true;
        }
        else if ( s.find("Chain ") == 0 )
        {
            UpdatePatternChainFromString(s);
            updates += 1;
            return true;
        }
        else if ( s.find("Store ") == 0 )
        {
            SetFieldsFromString(s);
            return true;
        }
        else if ( s.find("Default ") == 0 )
        {
            m_DefaultPattern.FromString(s, updates);
            return true;
        }
        else
        {
            if ( m_Patterns.empty() )
            {
                m_Patterns.emplace_back();
                created += 1;
            }
            return m_PosEdit->FromString(s, updates);
        }
#ifndef MA_BLUE
    }
    catch (invalid_argument e)
    {
        throw string("PatternStore::LoadFromString(), invalid argument.");
    }
#endif
}

//void PatternStore::DeleteCurrentRealTimeList()
//{
//#ifdef MA_BLUE
//    if ( m_Patterns.empty() )
//        return;
//    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
//        return;
//#else
//    if ( m_Patterns.empty() )
//        throw string("Pattern Store is Empty.");
//    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
//        throw string("Current pattern has no real time lists.");
//#endif
//    m_Patterns.at(m_PosEdit).DeleteCurrentRealTimeList();
//
//    SetStatus();
//}

string PatternStore::ListManager(string commandString, vector<string> & tokens)
{
    uint16_t index;

    // We're guaranteed to have at least a second token.
    // (The calling routine doesn't call if there's only one.)

    // Check for commands not specific to any list.

    if ( tokens[1] == "new")
    {
        if ( m_Patterns.empty() )
            m_Patterns.emplace_back();

//        m_Patterns.at(m_PosEdit).NewList();
        return "Empty list created.";
    }

    if ( tokens[1] == "delete" || tokens[1] == "del" )
    {
//        if ( m_Patterns.empty() || m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
        if ( m_Patterns.empty() || m_PosEdit->m_ListGroups.empty() )
            return "Nothing to delete.";
        m_PosEdit->DeleteCurrentList();
        return "Current list deleted.";
    }

    // For everything else, we need a list index and that should
    // be the next token.

    index = strtol(tokens.at(1).c_str(), NULL, 0);

    if ( index <= 0 )
        return "List index is bad or missing.";

    index -= 1;

    // If we just have a list index, display list and set list pointer
    // to this index, then return.

//    if ( tokens.size() == 2 )
//    {
//        m_Patterns.at(m_PosEdit).SetEditPos(index);
//// TODO:LG
////        string temp = "L " + tokens.at(1) + " - " + m_Patterns.at(m_PosEdit).m_StepListSet.at(index).ToString(false);
//        string temp = "Not implemented yet.";
//        if ( temp.size() > 60 )
//        {
//            temp.resize(57);
//            temp += "...";
//        }
//        return temp;
//    }
//
//    // At this point we have at least three tokens and we
//    // know the list index is good.
//
//    if ( tokens.at(2) == "clear" )
//    {
//        if ( m_Patterns.empty() )
//            return "Pattern List is empty.";
//
////        if ( index < m_Patterns.at(m_PosEdit).m_StepListSet.size() )
//        if ( index < m_Patterns.at(m_PosEdit).m_ListGroups.size() )
//        {
//// TODO:LG
////            m_Patterns.at(m_PosEdit).m_StepListSet.at(index).Clear();
//            return "List cleared.";
//        }
//        else
//            return "List doesn't exist.";
//    }
//
//    // If we get to here, expect an actual note list.
//
//    if ( m_Patterns.empty() )
//        m_Patterns.emplace_back();
//
//    int pos = tokens.at(0).size() + tokens.at(1).size() + 1;
//    if ( m_Patterns.at(m_PosEdit).AddStepListFromString(index, commandString.substr(pos)) )
//        return "List updated.";
//    else
        return "Nothing changed.";
}


TranslateTable & PatternStore::TranslateTableForPlay()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_Patterns.at(m_PosEdit).TranslateTableValid() */ )
        return m_PosPlay->PatternTranslateTable();
    else
        return m_DefaultPattern.PatternTranslateTable();
}


TranslateTable & PatternStore::TranslateTableForEdit(bool setFocus)
{
    TranslateTable * pTable = NULL;

    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_PosEdit).TranslateTableValid() */ )
        pTable = & m_PosEdit->PatternTranslateTable();
    else
        pTable = & m_DefaultPattern.PatternTranslateTable();

    if ( setFocus )
        pTable->SetFocus();

    return *pTable;
}

FeelMap & PatternStore::FeelMapForPlay()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_PosEdit->TranslateTableValid() */ )
        return m_PosPlay->PatternFeelMap();
    else
        return m_DefaultPattern.PatternFeelMap();
}


FeelMap & PatternStore::FeelMapForEdit(bool setFocus)
{
    FeelMap * pMap = NULL;

    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_PosEdit->TranslateTableValid() */ )
        pMap = & m_PosEdit->PatternFeelMap();
    else
        pMap = & m_DefaultPattern.PatternFeelMap();

    if ( setFocus )
        pMap->SetFocus();

    return *pMap;
}


void PatternStore::SetUsePatternPlayData( bool val )
{
    if ( val && m_Patterns.empty() )
#ifdef MA_BLUE
        return;
#else
        throw string("Cannot use pattern play data until a pattern is loaded.");
#endif

    m_UsePatternPlayData = val;
}


void PatternStore::SetStepValue(double step)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreStepValue(step);
    else
        m_DefaultPattern.StoreStepValue(step);

    // SetNewPatternPending(m_PosPlay); // Force reload
}

void PatternStore::SetGateLength(double gate)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreGate(gate);
    else
        m_DefaultPattern.StoreGate(gate);
}

void PatternStore::SetGateHold(bool val)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreGateHold(val);
    else
        m_DefaultPattern.StoreGateHold(val);
}

void PatternStore::SetNoteVelocity(unsigned char vel)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreVelocity(vel);
    else
        m_DefaultPattern.StoreVelocity(vel);
}

double PatternStore::StepValue()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().StepValue();
    else
        return m_DefaultPattern.StepValue();
}

double PatternStore::GateLength()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().Gate();
    else
        return m_DefaultPattern.Gate();
}

bool PatternStore::GateHold()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().GateHold();
    else
        return m_DefaultPattern.GateHold();
}

unsigned char PatternStore::NoteVelocity()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().Velocity();
    else
        return m_DefaultPattern.Velocity();
}

void PatternStore::StorePatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask )
{
    if ( m_Patterns.empty() )
#ifdef MA_BLUE
        return;
#else
        throw string("Pattern Store is empty.");
#endif

    Pattern & p = *m_PosEdit;
    if ( mask & PLAY_DATA_STEP )
        p.StoreStepValue(m_DefaultPattern.StepValue());

    if ( mask & PLAY_DATA_GATE )
    {
        p.StoreGate(m_DefaultPattern.Gate());
        p.StoreGateHold(m_DefaultPattern.GateHold());
    }
    if ( mask & PLAY_DATA_VELO )
        p.StoreVelocity(m_DefaultPattern.Velocity());
    if ( mask & PLAY_DATA_SCALE )
        p.StoreTranslateTable(m_DefaultPattern.PatternTranslateTable());
}

string PatternStore::LoadPatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask )
{
    if ( m_Patterns.empty() )
#ifdef MA_BLUE
        return "";
#else
        throw string("Pattern Store is empty.");
#endif

    string result;

    Pattern & p = *m_PosEdit;

    if ( (mask & PLAY_DATA_STEP) /* && p.StepValueValid() */ )
    {
        m_DefaultPattern.SetStepValue(p.StepValue());
        result += "Step Value";
    }

    if ( (mask & PLAY_DATA_GATE) /* && p.GateValid() */ )
    {
        m_DefaultPattern.StoreGate(p.Gate());
        m_DefaultPattern.StoreGateHold(p.GateHold());
        if ( !result.empty() )
            result += ", ";
        result += "Gate Length";
    }

    if ( (mask & PLAY_DATA_VELO) /* && p.VelocityValid() */ )
    {
        m_DefaultPattern.StoreVelocity(p.Velocity());
        if ( !result.empty() )
            result += ", ";
        result += "Velocity";
    }

    if ( (mask & PLAY_DATA_SCALE) /* && p.TranslateTableValid() */)
    {
        m_DefaultPattern.StoreTranslateTable(p.PatternTranslateTable());
        if ( !result.empty() )
            result += ", ";
        result += "Translation Data";
    }

    if ( !result.empty() )
        result.insert(0, "Loading ");
    else
        result = "No play data has been stored here.";

    return result;
}

string PatternStore::ShowPatternPlayData()
{
    char buff[100];
    string result;

    bool usePatternPlayData = m_UsePatternPlayData && !m_Patterns.empty();
    Pattern & p = usePatternPlayData ? *m_PosEdit : m_DefaultPattern;

    if ( usePatternPlayData )
    {
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
        snprintf(buff, 100, "P %02u: ", m_PosEdit + 1);
#else
        snprintf(buff, 100, "P %02lu: ", m_PosEdit->PatternID());
#endif
        result += buff;
    }
    else
        result += "GLOB: ";

    snprintf(buff, 100, "Step %.2f, ", p.StepValue());
    result += buff;

    snprintf(buff, 100, "Vel/Gate %i/%.0f%% (Hold %s), ", p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    result += buff;

    TranslateTable table = p.PatternTranslateTable();
    snprintf(buff, 100, "C/T: %i/%i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());
    result += buff;

    return result;
}

