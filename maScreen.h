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

#ifndef MASCREEN_H_INCLUDED
#define MASCREEN_H_INCLUDED

#include "maAnsiUI.h"

#ifndef MA_BLUE

#include "platform_Linux/maCairoUI.h"

#endif // MA_BLUE

// Screen locations

// At the moment these are common to text and cairo UIs, so
// maybe they should stay here.

//#define COMMAND_HOME 3,4
//#define STAT_POS_TOPLINE 0,0
//#define STAT_POS_STEP AnsiUI::progress_panel, 0,0
//#define STAT_POS_PATTERN AnsiUI::small_panel, 1,0
//#define STAT_POS_PATTERN_EDIT AnsiUI::small_panel, 2,0
//#define STAT_POS_2 2,4
//#define STAT_POS_MENU 3,4
//
void set_status(int y, int x, const char *format, ... );
void set_status_w(AnsiUI::window_area_t area, int y, int x, const char *format, ...);

void place_cursor(int row, int col);

void highlight_pattern_panel();

void set_top_line();
void show_status();
void show_translation_status();
void show_translation_map_status();
void show_listbuilder_status();
//void show_status_after_navigation();

//void update_pattern_list_panels(bool refreshList = false);
//void update_big_panel();
//void update_pattern_status_panel();
void update_progress_bar();
void update_item_menus();


#endif // MASCREEN_H_INCLUDED
