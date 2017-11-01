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

#include "maScreen.h"

#include "maState.h"
#include "maUtility.h"
#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maTranslateTable.h"

using namespace std;

// Global ALSA Sequencer instance.

extern AlsaSequencer g_Sequencer;
extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;
extern TranslateTable * pTranslateTable;

// Additional colours beyond the basic eight that are defined
// in ncurses.h. (Note the UK spelling to distinguish them from
// the defaults, which aren't changed.)

// (I haven't found documentation that says it's safe to use colour
// definition numbers beyond the first 0..7. It just works, so I'm
// using them for now.)

#define COLOUR_GREEN        8
#define COLOUR_RED          9
#define COLOUR_BLUE         10
#define COLOUR_GREY         11
#define COLOUR_BRIGHT_RED   12
#define COLOUR_REDDISH      13
#define COLOUR_BRIGHT_GREEN 14
#define COLOUR_YELLOW       16
#define COLOUR_DARK_GREY    17
#define COLOUR_DARKER_GREY  18
#define COLOUR_WHITE        19
#define COLOUR_BLACK        20

// Color Pair uses

enum colour_pairs
{
    CP_MAIN = 1,
    CP_PATTERN_LIST_PANEL,
    CP_PATTERN_LIST_PANEL_2,
    CP_PATTERN_LIST_PANEL_3,
    CP_PATTERN_LIST_PANEL_BKGND,
    CP_PROGRESS_BAR_HIGHLIGHT,
    CP_PROGRESS_BAR_BKGND,
    CP_SMALL_PANEL_BKGND,
    CP_MENU_HIGHLIGHT,
    CP_RUNNING,
    CP_RECORD,
    CP_REALTIME
};


Display::Display()
{
    // Set up NCurses.

    initscr();
    raw();
    keypad(stdscr, true);

    start_color();

    init_color(COLOUR_GREEN, 0, 750, 0);
    init_color(COLOUR_BRIGHT_GREEN, 0, 750, 0);
    init_color(COLOUR_YELLOW, 750, 500, 0);
    init_color(COLOUR_RED, 750, 0, 0);
    init_color(COLOUR_REDDISH, 900, 300, 200);
    init_color(COLOUR_BLUE, 250, 750, 900);
    init_color(COLOUR_BRIGHT_RED, 1000, 0, 0);
    init_color(COLOUR_GREY, 750, 750, 750);
    init_color(COLOUR_DARK_GREY, 500, 500, 500);
    init_color(COLOUR_DARKER_GREY, 250, 250, 250);

    init_color(COLOUR_WHITE, 1000, 1000, 1000);
    init_color(COLOUR_BLACK, 0, 0, 0);

	init_pair(CP_MAIN, COLOUR_WHITE, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL, COLOUR_BRIGHT_GREEN, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_2, COLOUR_BLUE, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_3, COLOUR_REDDISH, COLOUR_BLACK);
	init_pair(CP_RUNNING, COLOUR_WHITE, COLOUR_GREEN);
	init_pair(CP_REALTIME, COLOUR_WHITE, COLOUR_RED);
	init_pair(CP_RECORD, COLOUR_WHITE, COLOUR_YELLOW);
	init_pair(CP_MENU_HIGHLIGHT, COLOUR_WHITE, COLOUR_REDDISH);
	init_pair(CP_PATTERN_LIST_PANEL_BKGND, COLOR_YELLOW, COLOUR_GREY);
    init_pair(CP_SMALL_PANEL_BKGND, COLOR_YELLOW, COLOUR_BLACK);
    init_pair(CP_PROGRESS_BAR_BKGND, COLOUR_DARK_GREY, COLOUR_BLACK);
    init_pair(CP_PROGRESS_BAR_HIGHLIGHT, COLOUR_GREY, COLOUR_BLACK);


    mvprintw(6, 1, "=> ");

    m_SmallPanel = newwin(4, 64, 2, 4);
    m_BigPanel = newwin(16, 80, 9, 0);

    bkgd(COLOR_PAIR(CP_MAIN));
    wbkgd(m_SmallPanel, COLOR_PAIR(CP_SMALL_PANEL_BKGND));
    wbkgd(m_BigPanel, COLOR_PAIR(CP_MAIN));
}


Display::~Display()
{
    delwin(m_SmallPanel);
    delwin(m_BigPanel);
    endwin();			/* End curses mode		  */
}

Display gDisplay;

void highlight(int base_row, int base_col, int ofs, int len, int attr, int colour)
{
    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);
    mvchgat(base_row, base_col + ofs, len, attr, colour, NULL);
    move(scr_y, scr_x);
    refresh();
}

void set_top_line()
{
    set_status(STAT_POS_TOPLINE, "Multi Arp - Midi:%02i, Step:%5.2f, Link Quantum:%5.2f     %s",
               g_Sequencer.MidiChannel() + 1,
               g_State.CurrentStepValue(),
               g_State.Quantum(),
               g_State.RunState() ? "<<   RUN   >>" : "<<   ---   >>");

    highlight(0, 0, 0, 80, A_BOLD, g_ListBuilder.MidiInputModeAsColour(vector<int> {CP_MAIN, CP_RECORD, CP_RECORD, CP_REALTIME})); // Hmm ...
    highlight(0, 0, 60, 5, A_BOLD, g_State.RunState() ? CP_RUNNING : CP_MAIN);
}

void update_progress_bar()
{
    double progress, stepWidth;
    g_State.Progress(progress, stepWidth);

    int n = lround(64.0 * progress);
    int len = lround(64.0 * stepWidth);

    int mode = 0;
    if ( len == 1 )
        mode = 1;
    else if ( len < 9 )
        mode == 2;

    if ( n + len > 64 )
        len = 64 - n;

    string barline(64, '.');
    string marker(len, '-');

    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);

    switch ( mode )
    {
    case 1:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(1, 4, barline.c_str());
        attron(COLOR_PAIR(CP_MAIN));
        mvaddch(1, 4 + n, ACS_RARROW);
        break;
    case 2:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(1, 4, barline.c_str());
        attron(COLOR_PAIR(CP_MAIN));
        mvprintw(1, 4 + n, marker.c_str());
        break;
    default:
        move(1, 4);
        clrtoeol();
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(1, 4 + n, marker.c_str());
        break;
    }

    attroff(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));

    move(scr_y, scr_x);
    refresh();

}


std::vector<int> g_ListDisplayRows;

void update_pattern_panel()
{
    int scr_x, scr_y;

    wmove(gDisplay.BigPanel(), 0, 0);
    wclrtobot(gDisplay.BigPanel());

    if ( !g_PatternStore.Empty() )
    {
        wattron(gDisplay.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL_3));

        Pattern & p = g_PatternStore.CurrentPlayPattern();
        wmove(gDisplay.BigPanel(), 0, 4);
        wprintw(gDisplay.BigPanel(), "Play %i, %s", g_PatternStore.CurrentPlayPatternID(), p.Label(50).c_str());

        wmove(gDisplay.BigPanel(), 1, 4);
        wprintw(gDisplay.BigPanel(), "Step %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
            p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");

        TranslateTable table = p.PatternTranslateTable();
        wmove(gDisplay.BigPanel(), 2, 4);
        wprintw(gDisplay.BigPanel(), "Chromatic %i, Tonal %i (%s), %s-%s",
                table.Transpose(),
                table.DegreeShift(),
                table.ShiftName(),
                table.RootName().c_str(),
                table.ScaleName());

        wmove(gDisplay.BigPanel(), 4, 0);
    }

	wattron(gDisplay.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL_2));

    g_ListDisplayRows.clear();

    for ( int i = 0; i < g_PatternStore.RealTimeListCount(); i++ )
    {
//        getyx(gDisplay.BigPanel(), scr_y, scr_x);
//        g_ListDisplayRows.push_back(scr_y);
        wprintw(gDisplay.BigPanel(), "%s\n", g_PatternStore.RealTimeListToStringForDisplay(i).c_str());
//        highlight(i, 0, 0, 80, 0, CP_PATTERN_LIST_PANEL_2);
    }

    wprintw(gDisplay.BigPanel(), "\n");
	wattron(gDisplay.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL));

    for ( int i = 0; i < g_PatternStore.PlayPatternListCount(); i++ )
    {
        getyx(gDisplay.BigPanel(), scr_y, scr_x);
        g_ListDisplayRows.push_back(scr_y);
        wprintw(gDisplay.BigPanel(), "%s\n", g_PatternStore.PlayPatternListToString(i).c_str());
//        highlight(scr_y, 0, 0, 80, 0, CP_PATTERN_LIST_PANEL);
    }

    wattroff(gDisplay.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL));

    wrefresh(gDisplay.BigPanel());

}

void highlight_pattern_panel()
{
/*
    Clear previous highlist.

    Get list number and highlight info from g_PatternStore.

    Translate to screen location for highlight.

    How to allow for lines that wrap?
        - Probably OK for first List = 1.
        - But need to account for any wraps on previous lists when updating List > 1.

    Update attributes.
*/
    static std::vector<int> clearPositions;

    while ( clearPositions.size() >= 3 )
    {
        int lastLength = clearPositions.back();
        clearPositions.pop_back();
        int lastOffset = clearPositions.back();
        clearPositions.pop_back();
        int lastRow = clearPositions.back();
        clearPositions.pop_back();

        mvwchgat(gDisplay.BigPanel(), lastRow, lastOffset, lastLength, 0, 1, NULL);
    }

    int listIndex, offset, length;

    if ( g_PatternStore.PlayPositionInfo(listIndex, offset, length) )
    {
        int row = g_ListDisplayRows.at(listIndex);

        // TODO: (Sort of.) This is a hack to cope with the fact that the pattern now inserts a simple
        //       arrow at the start of each row to show selection. The 'big panel' now uses the full width
        //       of the screen, but the play list ToString() routine still store's highlight positions
        //       relative to the beginning of the string it creates, which doesn't include the selection
        //       arrow. If we keep this flickering highlight mechanism, we need to make this better.

        offset += 4;

        while ( offset >= 76 )
        {
            row += 1;
            offset -= 76;
        }

        if ( (offset + length) >= 76 )
        {
            int length0 = 76 - offset;
            mvwchgat(gDisplay.BigPanel(), row, offset, length0, A_BOLD, 1, NULL);

            clearPositions.push_back(row);
            clearPositions.push_back(offset);
            clearPositions.push_back(length0);

            row += 1;
            offset = 0;
            length -= length0;
        }

        mvwchgat(gDisplay.BigPanel(), row, offset, length, A_REVERSE, 1, NULL);

        clearPositions.push_back(row);
        clearPositions.push_back(offset);
        clearPositions.push_back(length);
    }

    wrefresh(gDisplay.BigPanel());
}



void show_status()
{
    set_status(STAT_POS_2, g_PatternStore.PatternOverview().c_str());
}

void show_status_after_navigation()
{
    // Call this after any change to focus or navigation involving
    // objects derived from CursorKeys. All objects share the same
    // static pointer to the object in focus, and calling Status()
    // on any of them will retrieve the status string for the object
    // that currently has focus. (For now we use g_PatternStore itself,
    // because it's global and always present, but there's no other
    // special status awarded to it than that.)

    set_status(STAT_POS_2, g_PatternStore.Status().c_str());
    int ofs, len;
    vector<screen_pos_t> & highlights = g_PatternStore.GetHighlights();

    for ( int i = 0; i < highlights.size(); i++ )
        highlight(STAT_POS_2, highlights.at(i).offset, highlights.at(i).length, A_BOLD, CP_MENU_HIGHLIGHT);
}

void show_translation_map_status()
{
    set_status(STAT_POS_2, "I: %s     Map: %s", g_PatternStore.TranslateTableForEdit(false).ShowScale().c_str(), g_PatternStore.TranslateTableForEdit(false).ShowNoteMap().c_str());
}

void show_translation_status()
{
    set_status(STAT_POS_2, g_PatternStore.TranslateTableForEdit().Status().c_str());
}


void show_listbuilder_status()
{
    std::string temp = g_ListBuilder.ToString();
    int len = temp.size();
    if ( len > 60 )
        set_status(STAT_POS_2, "%c: ...%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str() + len - 60 );
    else
        set_status(STAT_POS_2, "%c:%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str());
}
