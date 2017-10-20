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

#include "maPatternStore.h"

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


string Note::ToString()
{
    if ( m_NoteNumber == -1 )
        return " -";

    char buffer[25];
    if ( m_NoteVelocity >= 0 )
        sprintf(buffer, " %s%i/%i", mapNumbersToNotes.at(m_NoteNumber % 12).c_str(), m_NoteNumber / 12, m_NoteVelocity);
    else
        sprintf(buffer, " %s%i", mapNumbersToNotes.at(m_NoteNumber % 12).c_str(), m_NoteNumber / 12);

    return buffer;
}

void Note::FromString(string s)
{
    size_t pos = s.find('/');

    if ( pos != string::npos )
    {
        m_NoteNumber = mapNotesToNumbers.at(s.substr(0, pos));
        m_NoteVelocity = stoi(s.substr(pos + 1));
        if ( m_NoteVelocity < 0 || m_NoteVelocity > 127 )
            m_NoteVelocity = -1;
    }
    else
    {
        m_NoteNumber = mapNotesToNumbers.at(s);
        m_NoteVelocity = -1;
    }
}

bool Chord::IsRest()
{
    if ( Empty() )
        return true;

    for ( vector<int>::size_type i = 0; i < m_Notes.size(); i++ )
        if ( m_Notes[i].m_NoteNumber >= 0 )
            return false;

    return true;
}

string Chord::ToString()
{
    string result;
    for ( vector<Note>::iterator i = m_Notes.begin(); i != m_Notes.end(); i++ )
    {
        // if ( result.size() > 0 )
        //  result += ' ';
        result += Note(*i).ToString();
    }
    return result;
}

void Chord::FromString(string s)
{
    vector<string> noteStrings = split(s.c_str());

    if ( noteStrings.size() == 0 )
    {
        // Empty notes are rests, so leave the note list empty.
        return;
    }

//    for ( int i = 0; i < noteStrings.size(); i++ )
    for ( vector<string>::iterator it = noteStrings.begin(); it != noteStrings.end(); it++ )
    {
//        const char * noteString = noteStrings[i].c_str();

        Note n;
        try
        {
//            n.FromString(noteStrings.at(i));
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

Chord * NoteList::Step()
{
    if ( m_Chords.empty() )
        return NULL;

    m_LastRequestedPos = m_Pos;
    Chord *pChord = & m_Chords[m_Pos++];

    // Look ahead for rests.

    if ( !pChord->IsRest() )
    {
        vector<int>::size_type p = m_Pos;
        pChord->m_StepLength = 0;

        do
        {
            if ( p == m_Chords.size() )
                p = 0;

            if ( m_Chords[p++].IsRest() )
                pChord->m_StepLength += 1;
            else
                break;

        } while ( true );
    }
    else
        pChord = NULL;

    // Set completion flag.

    if ( m_Pos >= m_Chords.size() )
    {
        m_Complete = true;
        m_Pos = 0;
    }
    else
    {
        m_Complete = false;
    }

    return pChord;
}


bool NoteList::PlayPositionInfo(int & offset,  int & length)
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

string NoteList::ToString()
{
    string result;
    m_PosInfo.clear();
    for ( vector<Chord>::iterator i = m_Chords.begin(); i != m_Chords.end(); )
    {
        int iStart = result.size();
        result += Chord(*i).ToString();
        if ( ++i < m_Chords.end() )
            result += ",";
        m_PosInfo.push_back(PosInfo(iStart, result.size() - iStart));
    }
    return result;
}

void NoteList::FromString(string s)
{
    vector<string> chordStrings = split(s.c_str(), ',', true);

    if ( chordStrings.size() == 0 )
        throw string("Note List parse error: nothing found.");

    for ( vector<string>::iterator it = chordStrings.begin(); it != chordStrings.end(); it++ )
//    for ( int i = 0; i < chordStrings.size(); i++ )
    {
//        const char * chordString = chordStrings[i].c_str();

        Chord chord;
        chord.FromString(*it);
        m_Chords.push_back(chord);
    }
}
