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



std::map<std::string, int> noteMap = {

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
