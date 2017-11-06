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

#include "maNotes.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <unordered_map>

#include "maUtility.h"

using namespace std;

unordered_map<int, string> mapNumbersToNotes =
{
    {-1, ""},
    {0, "C"},
    {1, "C#"},
    {2, "D"},
    {3, "Eb"},
    {4, "E"},
    {5, "F"},
    {6, "Gb"},
    {7, "G"},
    {8, "Ab"},
    {9, "A"},
    {10, "Bb"},
    {11, "B"},
};

unordered_map<string, int> mapNotesToNumbers =
{

    {"R", -1}, {"Rest", -1}, {"-", -1},

    {"B#0", 0},{"C0", 0},
    {"C#0", 1},{"Db0", 1},
    {"D0", 2},
    {"D#0", 3},{"Eb0", 3},
    {"E0", 4},{"Fb0", 4},
    {"E#0", 5},{"F0", 5},
    {"F#0", 6},{"Gb0", 6},
    {"G0", 7},
    {"G#0", 8},{"Ab0", 8},
    {"A0", 9},
    {"A#0", 10},{"Bb0", 10},
    {"B0", 11},{"Cb0", 11},

    {"B#1", 12},{"C1", 12},
    {"C#1", 13},{"Db1", 13},
    {"D1", 14},
    {"D#1", 15},{"Eb1", 15},
    {"E1", 16},{"Fb1", 16},
    {"E#1", 17},{"F1", 17},
    {"F#1", 18},{"Gb1", 18},
    {"G1", 19},
    {"G#1", 20},{"Ab1", 20},
    {"A1", 21},
    {"A#1", 22},{"Bb1", 22},
    {"B1", 23},{"Cb1", 23},

    {"B#2", 24},{"C2", 24},
    {"C#2", 25},{"Db2", 25},
    {"D2", 26},
    {"D#2", 27},{"Eb2", 27},
    {"E2", 28},{"Fb2", 28},
    {"E#2", 29},{"F2", 29},
    {"F#2", 30},{"Gb2", 30},
    {"G2", 31},
    {"G#2", 32},{"Ab2", 32},
    {"A2", 33},
    {"A#2", 34},{"Bb2", 35},
    {"B2", 35},{"Cb2", 35},


    {"B#3", 36},{"C3", 36},
    {"C#3", 37},{"Db3", 37},
    {"D3", 38},
    {"D#3", 39},{"Eb3", 39},
    {"E3", 40},{"Fb3", 40},
    {"E#3", 41},{"F3", 41},
    {"F#3", 42},{"Gb3", 42},
    {"G3", 43},
    {"G#3", 44},{"Ab3", 44},
    {"A3", 45},
    {"A#3", 46},{"Bb3", 46},
    {"B3", 47},{"Cb3", 47},

    {"B#4", 48},{"C4", 48},
    {"C#4", 49},{"Db4", 49},
    {"D4", 50},
    {"D#4", 51},{"Eb4", 51},
    {"E4", 52},{"Fb4", 52},
    {"E#4", 53},{"F4", 53},
    {"F#4", 54},{"Gb4", 54},
    {"G4", 55},
    {"G#4", 56},{"Ab4", 56},
    {"A4", 57},
    {"A#4", 58},{"Bb4", 58},
    {"B4", 59},{"Cb4", 59},

    {"B#5", 60},{"C5", 60},
    {"C#5", 61},{"Db5", 61},
    {"D5", 62},
    {"D#5", 63},{"Eb5", 63},
    {"E5", 64},{"Fb5", 64},
    {"E#5", 65},{"F5", 65},
    {"F#5", 66},{"Gb5", 66},
    {"G5", 67},
    {"G#5", 68},{"Ab5", 68},
    {"A5", 69},
    {"A#5", 70},{"Bb5", 70},
    {"B5", 71},{"Cb5", 71},

    // Note names without numbers default to middle octave

    {"C", 60},
    {"C#", 61},{"Db", 61},
    {"D", 62},
    {"D#", 63},{"Eb", 63},
    {"E", 64},{"Fb", 64},
    {"E#", 65},{"F", 65},
    {"F#", 66},{"Gb", 66},
    {"G", 67},
    {"G#", 68},{"Ab", 68},
    {"A", 69},
    {"A#", 70},{"Bb", 70},
    {"B", 71},{"Cb", 71},

    {"B#6", 72},{"C6", 72},
    {"C#6", 73},{"Db6", 73},
    {"D6", 74},
    {"D#6", 75},{"Eb6", 75},
    {"E6", 76},{"Fb6", 76},
    {"E#6", 77},{"F6", 77},
    {"F#6", 78},{"Gb6", 78},
    {"G6", 79},
    {"G#6", 80},{"Ab6", 80},
    {"A6", 81},
    {"A#6", 82},{"Bb6", 82},
    {"B6", 83},{"Cb6", 83},

    {"B#7", 84},{"C7", 84},
    {"C#7", 85},{"Db7", 85},
    {"D7", 86},
    {"D#7", 87},{"Eb7", 87},
    {"E7", 88},{"Fb7", 88},
    {"E#7", 89},{"F7", 89},
    {"F#7", 90},{"Gb7", 90},
    {"G7", 91},
    {"G#7", 92},{"Ab7", 92},
    {"A7", 93},
    {"A#7", 94},{"Bb7", 94},
    {"B7", 95},{"Cb7", 95},

    {"B#8", 96},{"C8", 96},
    {"C#8", 97},{"Db8", 97},
    {"D8", 98},
    {"D#8", 99},{"Eb8", 99},
    {"E8", 100},{"Fb8", 100},
    {"E#8", 101},{"F8", 101},
    {"F#8", 102},{"Gb8", 102},
    {"G8", 103},
    {"G#8", 104},{"Ab8", 104},
    {"A8", 105},
    {"A#8", 106},{"Bb8", 106},
    {"B8", 107},{"Cb8", 107},

    {"B#9", 108},{"C9", 108},
    {"C#9", 109},{"Db9", 109},
    {"D9", 110},
    {"D#9", 111},{"Eb9", 111},
    {"E9", 112},{"Fb9", 112},
    {"E#9", 113},{"F9", 113},
    {"F#9", 114},{"Gb9", 114},
    {"G9", 115},
    {"G#9", 116},{"Ab9", 116},
    {"A9", 117},
    {"A#9", 118},{"Bb9", 118},
    {"B9", 119},{"Cb9", 119},

    {"B#10", 120},{"C10", 120},
    {"C#10", 121},{"Db10", 121},
    {"D10", 122},
    {"D#10", 123},{"Eb10", 123},
    {"E10", 124},{"Fb10", 124},
    {"E#10", 125},{"F10", 125},
    {"F#10", 126},{"Gb10", 126},
    {"G10", 127}
};

string Note::NoteName(int n)
{
    if ( n >= 0 )
    {
        char buffer[25];
        sprintf(buffer, "%s%i", mapNumbersToNotes.at(n % 12).c_str(), n / 12);
        return buffer;
    }
    else
        return "0";
}

int Note::NoteNumberFromString(string note)
{
    try
    {
        return mapNotesToNumbers.at(note.c_str());
    }
    catch (...)
    {
        return -1;
    }
}


string Note::ToString(bool showVelocity)
{
    if ( m_NoteNumber == -1 )
        return "-";

    char buffer[25];
    if ( showVelocity && m_NoteVelocity >= 0 )
        sprintf(buffer, "%s%i:%i:%.3f:%.3f",
            mapNumbersToNotes.at(m_NoteNumber % 12).c_str(),
            m_NoteNumber / 12,
            m_NoteVelocity,
            m_Phase,
            m_Length);
    else
        sprintf(buffer, "%s%i", mapNumbersToNotes.at(m_NoteNumber % 12).c_str(), m_NoteNumber / 12);

    return buffer;
}

void Note::FromString(string s)
{

    // We may have leading white space, which will throw the map look-up,
    // so convert separators to white space and then parse on that.

    replace( s.begin(), s.end(), ':', ' ');
    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() >= 1 )
        m_NoteNumber = mapNotesToNumbers.at(tokens.at(0));
    else
        m_NoteNumber = -1;

    if ( tokens.size() >= 2 )
    {
        m_NoteVelocity = stoi(tokens.at(1));
        if ( m_NoteVelocity < 0 || m_NoteVelocity > 127 )
            m_NoteVelocity = -1;
    }
    else
        m_NoteVelocity = -1;

    if ( tokens.size() >= 3 )
        m_Phase = stod(tokens.at(2));
    else
        m_Phase = 0.0;

    if ( tokens.size() >= 4 )
        m_Length = stod(tokens.at(3));
    else
        m_Length = 0.0;

//    size_t pos = s.find(':');
//
//    if ( pos != string::npos )
//    {
//        m_NoteNumber = mapNotesToNumbers.at(s.substr(0, pos));
//        m_NoteVelocity = stoi(s.substr(pos + 1));
//        if ( m_NoteVelocity < 0 || m_NoteVelocity > 127 )
//            m_NoteVelocity = -1;
//    }
//    else
//    {
//        m_NoteNumber = mapNotesToNumbers.at(s);
//        m_NoteVelocity = -1;
//    }
}

bool Cluster::IsRest()
{
    if ( Empty() )
        return true;

    for ( vector<int>::size_type i = 0; i < m_Notes.size(); i++ )
        if ( m_Notes[i].m_NoteNumber >= 0 )
            return false;

    return true;
}

string Cluster::ToString(bool showVelocity)
{
    string result;
    for ( vector<Note>::iterator i = m_Notes.begin(); i != m_Notes.end(); i++ )
    {
         if ( result.size() > 0 )
          result += '/';
        result += Note(*i).ToString(showVelocity);
    }
    return result;
}

void Cluster::FromString(string s)
{
    replace( s.begin(), s.end(), '/', ' ');         // Support both spacers.
    vector<string> noteStrings = split(s.c_str());

    if ( noteStrings.size() == 0 )
    {
        // Empty notes are rests, so leave the note list empty.
        return;
    }

    for ( vector<string>::iterator it = noteStrings.begin(); it != noteStrings.end(); it++ )
    {
        Note n;
        try
        {
            n.FromString(*it);
            m_Notes.push_back(n);
        }
        catch (...)
        {
            // Do nothing with this, we just want to
            // carry on and try the next note.
        }

    }
}

Cluster * StepList::Step()
{
    if ( m_Clusters.empty() )
        return NULL;

    m_LastRequestedPos = m_Pos;
    Cluster *pCluster = & m_Clusters[m_Pos++];

    // Look ahead for rests.

    if ( !pCluster->IsRest() )
    {
        vector<int>::size_type p = m_Pos;
        pCluster->m_StepLength = 0;

        do
        {
            if ( p == m_Clusters.size() )
                p = 0;

            if ( m_Clusters[p++].IsRest() )
                pCluster->m_StepLength += 1;
            else
                break;

        } while ( true );
    }
    else
        pCluster = NULL;

    // Set completion flag.

    if ( m_Pos >= m_Clusters.size() )
    {
        m_Complete = true;
        m_Pos = 0;
    }
    else
    {
        m_Complete = false;
    }

    return pCluster;
}


bool StepList::PlayPositionInfo(int & offset,  int & length)
{
    if ( m_LastRequestedPos >= m_PosInfo.size() )
        return false;

    /*
        Why m_LastRequestedPos?

        Display updates are triggered just before a set of events is
        about to be scheduled. m_Pos always points to the next
        item to be used, but we need to display info about what just
        happened. Hence we need store a 'previous' pointer, too.

     */

    offset = m_PosInfo.at(m_LastRequestedPos).offset;
    length = m_PosInfo.at(m_LastRequestedPos).length;

    return true;
}

string StepList::ToString(bool showVelocity)
{
    string result;
    m_PosInfo.clear();
    for ( vector<Cluster>::iterator i = m_Clusters.begin(); i != m_Clusters.end(); )
    {
        int iStart = result.size();
        result += Cluster(*i).ToString(showVelocity);
        if ( ++i < m_Clusters.end() )
            result += ",";
        m_PosInfo.push_back(PosInfo(iStart, result.size() - iStart));
    }
    return result;
}

void StepList::FromString(string s)
{
    vector<string> chordStrings = split(s.c_str(), ',', true);

    if ( chordStrings.size() == 0 )
        throw string("Note List parse error: nothing found.");

    Clear();

    for ( vector<string>::iterator it = chordStrings.begin(); it != chordStrings.end(); it++ )
    {
        Cluster chord;
        chord.FromString(*it);
        m_Clusters.push_back(chord);
    }
}

void StepList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    m_Status = "Step List - ";

    pos = m_Status.size();
    m_Status += "Edit List";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_StepListFocus));

}

bool StepList::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
        if ( m_ListSubMenu != NULL )
            delete m_ListSubMenu;
        m_ListSubMenu = new StepListSubMenu(m_Clusters);
        m_ListSubMenu->SetFocus();
        m_ListSubMenu->SetStatus();
        m_ListSubMenu->SetReturnFocus(this);
        break;
    case back_space:
        m_ReturnFocus->SetFocus();
        m_ReturnFocus->SetStatus();
        break;
    }

    m_FirstField = m_StepListFocus == 0;

    SetStatus();

    return true;
}

void StepListSubMenu::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    m_Status = "Chords - ";

//    pos = m_Status.size();
//    m_Status += "Edit List";
//    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
//
//    m_Highlights.push_back(m_FieldPositions.at(m_StepListFocus));

}

bool StepListSubMenu::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
//        if ( m_ListSubMenu != NULL )
//            delete m_ListSubMenu;
//        m_ListSubMenu = new StepListSubMenu(m_Clusters);
//        m_ListSubMenu->SetFocus();
//        m_ListSubMenu->SetStatus();
//        m_ListSubMenu->SetReturnFocus(this);
        break;
    case back_space:
        m_ReturnFocus->SetFocus();
        m_ReturnFocus->SetStatus();
        break;
    }

    m_FirstField = m_PosEdit == 0;

    SetStatus();

    return true;
}


enum rtl_element_names_t {
    rtl_name_loop,
    rtl_name_quantum,
    rtl_name_multiplier,
    rtl_name_window_adjust,
    num_rtl_element_names
};


unordered_map<rtl_element_names_t, const char *> rtl_element_names = {
    {rtl_name_loop, "Loop"},
    {rtl_name_quantum, "Quantum"},
    {rtl_name_multiplier, "Multiplier"},
    {rtl_name_window_adjust, "Window"},
    {num_rtl_element_names, ""}
};


void RealTimeList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_Status = "Real Time List - ";
    m_FieldPositions.clear();
    m_Highlights.clear();

    pos = m_Status.size();
    m_Status += "Notes";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Loop - ";

    m_Status += "S: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_LoopStart);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Q: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_LocalQuantum);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Mul: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_Multiplier);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Small Window: ";
    pos = m_Status.size();
    sprintf(buff, "%s", m_AdjustWindowToStep ? "on" : "off");
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_RTListFocus));

}

bool RealTimeList::HandleKey(key_type_t k)
{
    int temp;
    double inc = 0.1;
    switch ( k )
    {
    case enter:
        if ( m_RTListFocus == rtl_edit_notes )
        {
            // Another object here?
        }
        break;

    case back_space:
        m_ReturnFocus->SetFocus();
        m_ReturnFocus->SetStatus();
        break;

    case left:
        temp = static_cast<int>(m_RTListFocus) - 1;
        if ( temp >= 0 && temp < number_rt_list_focus_modes )
            m_RTListFocus = static_cast<rt_list_focus_t>(temp);
        break;

    case right:
        temp = static_cast<int>(m_RTListFocus) + 1;
        if ( temp >= 0 && temp < number_rt_list_focus_modes )
            m_RTListFocus = static_cast<rt_list_focus_t>(temp);
        break;

    case shift_up:
        inc = 0.01;
    case up:
        switch ( m_RTListFocus )
        {
        case rtl_loop_start:
            m_LoopStart += inc;
            break;
        case rtl_local_quantum:
            m_LocalQuantum += inc;
            if ( m_LocalQuantum == 0 )
                m_LocalQuantum += inc;
            break;
        case rtl_multiplier:
            m_Multiplier += inc;
            break;
        case rtl_window_adjust:
            m_AdjustWindowToStep = true;
            break;
        default:
            break;
        }
        break;

    case shift_down:
        inc = 0.01;
    case down:
        switch ( m_RTListFocus )
        {
        case rtl_loop_start:
            m_LoopStart -= inc;
            break;
        case rtl_local_quantum:
            if ( m_LocalQuantum - inc < 0 )
                m_LocalQuantum = 0;
            else
                m_LocalQuantum -= inc;
            break;
        case rtl_multiplier:
            m_Multiplier -= inc;
            break;
        case rtl_window_adjust:
            break;
        default:
            m_AdjustWindowToStep = false;
            break;
        }
        break;
    }

    m_FirstField = m_RTListFocus == 0;

    SetStatus();

    return true;
}

void RealTimeList::FromString(string s)
{
    vector<string> tokens = split(s.c_str(), ',', true);

    if ( tokens.size() == 0 )
        throw string("Note List parse error: nothing found.");

    // Expect field list in first token ...

    for ( rtl_element_names_t e = static_cast<rtl_element_names_t>(0);
          e < num_rtl_element_names;
          e = static_cast<rtl_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(tokens.at(0), rtl_element_names.at(e));

        if ( token.empty() )
            continue;

        try
        {
            switch (e)
            {
            case rtl_name_loop:
                m_LoopStart = stod(token);
                break;
            case rtl_name_quantum:
                m_LocalQuantum = stod(token);
                break;
            case rtl_name_multiplier:
                m_Multiplier = stod(token);
                break;
            case rtl_name_window_adjust:
                m_AdjustWindowToStep = token.find("on") == 0;
                break;
            default:
                break;
            }
        }
        catch(invalid_argument ex)
        {

        }
        catch(out_of_range ex)
        {
        }
    }

    // Anything after that is a note.

    if ( tokens.size() == 1 )
        return;  // Leave note list intact.

    m_RealTimeList.clear();

    for ( vector<string>::iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
    {
        Note note;
        note.FromString(*it);

        // Check for existing entries and adjust start time if found.

        while ( true )
        {
            map<double,Note>::iterator it2 = m_RealTimeList.find(note.Phase());
            if ( it2 == m_RealTimeList.end() )
                break;
            note.SetPhase(note.Phase() + 0.0001);
        }

        m_RealTimeList.insert(make_pair(note.Phase(), note));
    }
}

string RealTimeList::ToString()
{
    string result;

    char buff[200];
    sprintf(buff, " %s %.3f %s %.3f %s %.3f %s '%s'",
            rtl_element_names.at(rtl_name_loop), m_LoopStart,
            rtl_element_names.at(rtl_name_quantum), m_LocalQuantum,
            rtl_element_names.at(rtl_name_multiplier), m_Multiplier,
            rtl_element_names.at(rtl_name_window_adjust), m_AdjustWindowToStep ? "on" : "off"
            );

    result += buff;

    int i = 0;
    for (map<double,Note>::iterator it = m_RealTimeList.begin(); it != m_RealTimeList.end(); it++)
    {
        result += ", ";
        if ( i++ % 4 == 0 )
            result += "\\\n     ";
        result += it->second.ToString();
    }

    return result;
}


//string RealTimeList::ToString(int step, double phase)
//{
//    char buff[50];
//    sprintf(buff, "%6.2f (%.2f): ", m_LastRequestedPhase, m_LastRequestedStepValue);
//
//    string result = buff;
//
//    double space = m_LastRequestedPhase + 2.0/m_LastRequestedStepValue;
//
//    for (map<double,Note>::iterator it = m_RealTimeList.lower_bound(m_LastRequestedPhase - 2.0/m_LastRequestedStepValue); it != m_RealTimeList.end(); it++)
//    {
//        while ( space < it->second.Phase() )
//        {
//            result += 'a';
//            space += 4.0/m_LastRequestedStepValue;
//        }
////        sprintf(buff, "%s", it->second.ToString(false).c_str());
////        result += buff;
//        result += it->second.ToString(false);
//        space += 4.0/m_LastRequestedStepValue;
//    }
//
//    for ( ; space < m_QuantumAtCapture; space += 4.0/m_LastRequestedStepValue )
//        result += 'b';
//
//
//    space = 2.0/m_LastRequestedPhase;
//
//    for (map<double,Note>::iterator it = m_RealTimeList.begin(); it != m_RealTimeList.upper_bound(m_LastRequestedPhase); it++)
//    {
////        for ( ; space < it->second.Phase(); space += 4.0/m_LastRequestedStepValue )
////            result += 'c';
//        while ( space < it->second.Phase() )
//        {
//            result += 'c';
//            space += 4.0/m_LastRequestedStepValue;
//        }
////        sprintf(buff, "%s", it->second.ToString(false).c_str());
////        result += buff;
//        it->second.ToString(false);
//        space += 4.0/m_LastRequestedStepValue;
//    }
//
//    for ( ; space < m_LastRequestedPhase; space += 4.0/m_LastRequestedStepValue )
//        result += 'd';
//
//    return result;
//}
//

// Less efficient (probably) but easier to read (possibly) ...

string RealTimeList::ToStringForDisplay(int width)
{
    char buff[100];
    sprintf(buff, "%05.2f ", m_LastRequestedPhase);

    string result = buff;

    long topLimit = lround(1000 * m_QuantumAtCapture);
    long midLimit = lround(1000 * m_LastRequestedPhase);

    double windowPos = m_LastRequestedPhase;
    double windowStep = 4.0/m_LastRequestedStepValue;

    Cluster notes;

    while ( lround(1000 * windowPos) < topLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        windowPos += windowStep;
    }

    windowPos = 0;

    while ( lround(1000 * windowPos) < midLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        windowPos += windowStep;
    }

    if ( result.size() > width )
        result.resize(width);

    sprintf(buff, "\n    Multiplier %.2f, Loop Start %.2f, Loop Quantum %.2f, Window Adjust %s",
        m_Multiplier, m_LoopStart, m_LocalQuantum, m_AdjustWindowToStep ? "ON" : "OFF");
    result += buff;

    return result;
}


void RealTimeList::Step(Cluster & cluster, double phase, double stepValue /*, double quantum*/)
{
    bool localLoop = lround(m_LocalQuantum) > 0;

    phase *= m_Multiplier;

    // Wrap to start of local loop.

    if ( localLoop )
        while ( phase > m_LocalQuantum )
            phase -= m_LocalQuantum;

    phase += m_LoopStart;

    // Wrap to start of capture loop loop.

    if ( localLoop )
        while ( phase > m_QuantumAtCapture )
            phase -= m_QuantumAtCapture;

    m_LastRequestedStepValue = stepValue;
    m_LastRequestedPhase = phase;

    double window = 4.0 / stepValue;

    if ( m_AdjustWindowToStep && abs(m_Multiplier) < 1.0 )
        window *= m_Multiplier;

    double windowStart = phase - window/2;
    double windowEnd = phase + window/2;

    for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                    it != m_RealTimeList.upper_bound(windowEnd); it++ )
        cluster.Add(it->second);

    // When phase is zero, window start will be negative, so we also need to
    // look for notes at the top of the loop that would normally be quantized
    // up to next beat zero.

//    if ( windowStart < 0 )
//    {
//        windowStart += m_LinkQuantum;
//        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
//                    it != m_RealTimeList.upper_bound(m_LinkQuantum); it++ )
//            m_Captured.Add(it->second);
//    }

//    return result;

}
