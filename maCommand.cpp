#include "maCommand.h"

#include<algorithm>
#include<cstring>
#include<cmath>
#include<unordered_map>

#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"


// Global objects.

extern AlsaSequencer g_Sequencer;
extern State g_State;
extern PatternStore g_PatternStore;
extern Display gDisplay;
extern ListBuilder g_ListBuilder;
extern TranslateTable * pTranslateTable;

enum command_t
{
    C_NONE,
    C_EXIT,
    C_HELP,

    C_RUN,
    C_STOP,
    C_HALT,
    C_RESET,
    C_SET_RESETONPATTERNCHANGE,
    C_RESET_BEAT,

    C_STEPVAL,
    C_QUANTUM,
    C_GATELENGTH,
    C_GATE_HOLD,
    C_GATE_NORMAL,
    C_VELOCITY,
    C_TRANSPOSE,

    C_SCALE,
    C_SCALE_FROM_LIST,
    C_SCALE_SHOW,
    C_SCALE_HELP,
    C_SCALE_CONTROLS,
    C_SETROOT,

    C_FEEL,

    C_MIDI,

    C_CUE,              // Set the next pattern to play
    C_EDIT,             // Set focus for copy/paste
    C_EDIT_CURSOR_LOCK,
    C_EDIT_CURSOR_UNLOCK,
    C_PATTERN_CHAIN,    // Set pattern chain mode.
    C_NEW,              // Create new (empty) pattern.
    C_COPY,             // Copy current pattern into a new entry at the end of the pattern list.
    C_DELETE,           // Delete pattern and reduce size of pattern list.
    C_CLEAR,            // Clear pattern but leave empty entry in pattern list.
    C_UNDO,             // Restore deleted or cleared pattern to end of the pattern list.
    C_STATUS,           // Display current stats.

    C_STORE,
    C_STORE_STEP,
    C_STORE_GATE,
    C_STORE_VELOCITY,
    C_STORE_SCALE,
    C_STORE_ALL,
    C_STORE_HELP,
    C_LOAD,
    C_LOAD_STEP,
    C_LOAD_GATE,
    C_LOAD_VELOCITY,
    C_LOAD_SCALE,
    C_LOAD_ALL,

    C_USE,
    C_USE_GLOBAL_PLAYDATA,
    C_USE_PATTERN_PLAYDATA,
    C_USE_HELP,

    C_LIST,             // Note list commands.

    C_HELP_1,
    C_HELP_2,
    C_HELP_3,
    C_HELP_4,

    C_NUM_COMMANDS
};


std::unordered_map<std::string, command_t> gCommandList =
{
    {"exit", C_EXIT},
    {"quit", C_EXIT},
    {"help", C_HELP},

    {"run", C_RUN},
    {"stop", C_STOP},
    {"stop now", C_HALT},
    {"halt", C_HALT},

    {"reset", C_RESET},
    {"reset beat", C_RESET_BEAT},
    {"autoreset", C_SET_RESETONPATTERNCHANGE},

    {"note", C_STEPVAL},
    {"step", C_STEPVAL},
    {"quantum", C_QUANTUM},
    {"quan", C_QUANTUM},
    {"gate", C_GATELENGTH},
    {"gate hold", C_GATE_HOLD,},
    {"gate h", C_GATE_HOLD,},
    {"gate normal", C_GATE_NORMAL,},
    {"gate n", C_GATE_NORMAL,},

    {"velocity", C_VELOCITY},
    {"vel", C_VELOCITY},

    {"tran", C_TRANSPOSE},
    {"transpose", C_TRANSPOSE},

    {"scale", C_SCALE},
    {"s", C_SCALE},
    {"scale from list", C_SCALE_FROM_LIST},
    {"scale show", C_SCALE_SHOW},
    {"scale help", C_SCALE_HELP},
    {"help scale", C_SCALE_HELP},
    {"scale controls", C_SCALE_CONTROLS},
    {"scale c", C_SCALE_CONTROLS},
    {"scale k", C_SCALE_CONTROLS},
    {"root", C_SETROOT},

    {"feel", C_FEEL},
    {"f", C_FEEL},

    {"channel", C_MIDI},
    {"chan", C_MIDI},
    {"midi", C_MIDI},
    {"mi", C_MIDI},

    {"edit", C_EDIT},
    {"e", C_EDIT},
    {"cue", C_CUE},
    {"play", C_CUE},
    {"chain", C_PATTERN_CHAIN},
    {"chain:", C_PATTERN_CHAIN},
    {"new", C_NEW},
    {"copy", C_COPY},
    {"delete", C_DELETE},
    {"del", C_DELETE},
    {"clear", C_CLEAR},

    {"lock", C_EDIT_CURSOR_LOCK},
    {"unlock", C_EDIT_CURSOR_UNLOCK},

    {"list", C_LIST},
    {"l", C_LIST},
    {"status", C_STATUS},
    {"stat", C_STATUS},
    {"undo", C_UNDO},

    {"store", C_STORE},
    {"store step", C_STORE_STEP},
    {"store gate", C_STORE_GATE},
    {"store vel", C_STORE_VELOCITY},
    {"store scale", C_STORE_SCALE},
    {"store all", C_STORE_ALL},
    {"store help", C_STORE_HELP},
    {"help store", C_STORE_HELP},
    {"load help", C_STORE_HELP},
    {"help load", C_STORE_HELP},
    {"load", C_LOAD},
    {"load step", C_LOAD_STEP},
    {"load gate", C_LOAD_GATE},
    {"load vel", C_LOAD_VELOCITY},
    {"load scale", C_LOAD_SCALE},
    {"load all", C_LOAD_ALL},

    {"use", C_USE},
    {"use globals", C_USE_GLOBAL_PLAYDATA},
    {"use g", C_USE_GLOBAL_PLAYDATA},
    {"use pattern", C_USE_PATTERN_PLAYDATA},
    {"use p", C_USE_PATTERN_PLAYDATA},
    {"use help", C_USE_HELP},
    {"help use", C_USE_HELP},

    {"misc", C_HELP_1},
    {"pattern", C_HELP_3},
    {"control", C_HELP_2}
};

command_t command_from_string(std::string commandName)
{

    command_t command;

    try
    {
        std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
        command = gCommandList.at(commandName);
    }
    catch ( std::out_of_range const & e )
    {
        command = C_NONE;
    }

    return command;
}

command_t command_from_string(std::vector<std::string> & tokens, int count = 1)
{

    command_t command;

    try
    {
        std::string commandName = tokens[0];

        for ( int i = 1; i < count; i++ )
        {
            commandName += " ";
            commandName += tokens[i];
        }

        command = command_from_string(commandName);
    }
    catch ( std::out_of_range const & e )
    {
        command = C_NONE;
    }

    return command;
}

void do_help(std::string topicName)
{
    command_t topic = command_from_string(topicName);

    switch ( topic )
    {
        case C_CUE :
            set_status(STAT_POS_2, "Set the next pattern to play.");
            break;

        case C_EDIT :
            set_status(STAT_POS_2, "Set focus for copy/paste and other operations.");
            break;

        case C_NEW :
            set_status(STAT_POS_2, "Create new (empty) pattern.");
            break;

        case C_COPY :
            set_status(STAT_POS_2, "Copy current pattern into a new entry at the end of the pattern list.");
            break;

        case C_DELETE :
            set_status(STAT_POS_2, "Delete pattern and reduce size of pattern list.");
            break;

        case C_CLEAR :
            set_status(STAT_POS_2, "Clear pattern but leave empty entry in pattern list.");
            break;

        case C_UNDO :
            set_status(STAT_POS_2, "Restore deleted or cleared pattern to end of the pattern list.");
            break;

        case C_HELP_1 :
            set_status(STAT_POS_2, "Misc: exit/quit, status/stat, help, run, stop");
            break;

        case C_HELP_2 :
            set_status(STAT_POS_2, "Control: midi/channel/chan, step/note, transpose/tran, quantum/quan. gate");
            break;

        case C_HELP_3 :
            set_status(STAT_POS_2, "Pattern: play/cue, reset, autoreset, edit, new, copy, clear, delete/del, undo, list");
            break;

        case C_HELP_4 :
            set_status(STAT_POS_2, "");
            break;

        case C_NONE :
            set_status(STAT_POS_2, "Topics: 'control', 'pattern', 'misc' or try a command name");
            break;
        default:
            break;
    }
}

void midi_setup(std::vector<std::string> & tokens)
{
    if ( tokens.size() < 2 )
    {
        set_status(STAT_POS_2, "Hint: chan[nel]|mi[di] q[uick]|f[ull]|n (1..16)");
        return;
    }

    if ( tokens[1] == "f" || tokens[1] == "full" )
    {
        g_ListBuilder.SetMidiInputMode(MIDI_INPUT_FULL);
        set_status(STAT_POS_2, "Midi Input set to FULL mode.");
    }
    else if ( tokens[1] == "q" || tokens[1] == "quick" )
    {
        g_ListBuilder.SetMidiInputMode(MIDI_INPUT_QUICK);
        set_status(STAT_POS_2, "Midi Input set to QUICK mode.");
    }
    else if ( tokens[1] == "off" )
    {
        g_ListBuilder.SetMidiInputMode(MIDI_INPUT_OFF);
        set_status(STAT_POS_2, "Midi Input OFF.");
    }
    else
    {
        // Expect a number and use it to set midi channel.

        int iTemp = std::stoi(tokens[1].c_str());
        if ( iTemp >= 1 && iTemp <= 16)
        {
            set_status(STAT_POS_2, "Set midi channel: %s", tokens[1].c_str());
            g_Sequencer.SetMidiChannel(iTemp - 1);
            set_top_line();
        }
    }
}

void navigate_pattern_store(int key)
{
    switch ( key )
    {
    case KEY_UP:
        g_PatternStore.DownListPos();
        set_status(STAT_POS_2, "List edit DOWN.");
        break;
    case KEY_DOWN:
        g_PatternStore.UpListPos();
        set_status(STAT_POS_2, "List edit UP.");
        break;
    case KEY_LEFT:
        g_PatternStore.DownEditPos();
        set_status(STAT_POS_2, "Pattern edit DOWN.");
        break;
    case KEY_RIGHT:
        g_PatternStore.UpEditPos();
        set_status(STAT_POS_2, "Pattern edit UP.");
        break;
    }
}

bool do_command(std::string/*const char * */ commandString)
{
    bool bResult = true;

    std::vector<std::string> tokens = split(commandString.c_str());

    if ( tokens.empty() )
        return bResult;

    command_t command;
//    int firstParameter;

    // Try for combinations first. Hopefully this makes the following switch
    // statement easier to read (though longer).

    for ( int i = tokens.size(); i >= 0; i-- )
        if ( command = command_from_string(tokens, i), command != C_NONE )
        {
            // firstParameter = tokens.size() - i;
            break;
        }

    // int command = command_from_string(tokens[0]);

    try
    {
        int iTemp;
        double fTemp;

        switch ( command )
        {

        case C_RUN :
            g_State.SetNewRunStatePending(true);
            break;

        case C_STOP :
            g_State.SetNewRunStatePending(false, 1);
            break;

        case C_HALT:
            g_State.SetRunState(false);
            break;

        case C_CUE  :
        case C_EDIT :
            if ( tokens.size() < 2 )
            {
                g_PatternStore.SetFocus();
                show_status_after_navigation();
//                throw std::string("Hint: cue|edit n [now]");
            }
            else
            {
                iTemp = std::stoi(tokens[1].c_str()) - 1;
                if ( ! g_PatternStore.ValidPosition(iTemp) )
                    throw std::string("Requested pattern number out of range at the moment.");
                switch ( command )
                {
                case C_CUE :
                    set_status(STAT_POS_2, "Cueing pattern: %s", tokens[1].c_str());
                    g_PatternStore.SetNewPatternPending(iTemp);
                    // newPattern = iTemp;
                    break;
                case C_EDIT :
                    set_status(STAT_POS_2, "Editing pattern: %s", tokens[1].c_str());
                    g_PatternStore.SetEditPos(iTemp);
                    set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
                    break;
                default:
                    break;
                }
            }
            break;

        case C_EDIT_CURSOR_LOCK:
            g_PatternStore.SetEditFocusFollowsPlay(true);
            set_status(STAT_POS_2, "Edit focus locked with playing pattern.");
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;
        case C_EDIT_CURSOR_UNLOCK:
            g_PatternStore.SetEditFocusFollowsPlay(false);
            set_status(STAT_POS_2, "Edit focus unlocked.");
            break;

        case C_NEW :
            g_PatternStore.AddEmptyPattern();
            // g_PatternStore.SetStepValCurrentEditPattern(g_State.StepValue());
            set_status(STAT_POS_2, "Empty pattern added at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_COPY :
            g_PatternStore.CopyCurrentPattern();
            set_status(STAT_POS_2, "Current pattern copied at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_DELETE :
            if ( g_PatternStore.PatternCount() == 0 )
                throw std::string("Nothing to delete.");
            if ( tokens.size() >= 2 && tokens.at(1) == "all" )
            {
                g_PatternStore.DeleteAllPatterns();
                set_status(STAT_POS_2, "All patterns deleted. (\'Undo\' retrieves them one at a time.)");
            }
            else
            {
                g_PatternStore.DeleteCurrentPattern();
                set_status(STAT_POS_2, "Current pattern deleted. (\'Undo\' puts it at the end of the list.)");
            }
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_UNDO :
            g_PatternStore.PopDeletedPattern();
            set_status(STAT_POS_2, "Pattern restored at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_CLEAR :
            g_PatternStore.ClearCurrentPattern();
            // g_PatternStore.SetStepValCurrentEditPattern(g_State.StepValue());
            set_status(STAT_POS_2, "Current edit pattern cleared. (\'Undo\' restores it to the end of the list.)");
            break;

        case C_RESET :
            if ( tokens.size() >= 2 && tokens.at(1) == "all" )
            {
                g_State.SetPatternReset(RESET_ALL);
                set_status(STAT_POS_2, "All patterns will be reset.");
            }
            else if ( g_PatternStore.EditPatternIsPlayPattern() )
            {

                g_State.SetPatternReset(RESET_CURRENT);
                set_status(STAT_POS_2, "Current pattern will be reset.");
            }
            else
            {
                // Reset the edit pattern immediately.
                g_PatternStore.ResetCurrentPattern();
                set_status(STAT_POS_2, "Edit pattern was reset.");
            }
            break;

        case C_RESET_BEAT:
            g_State.SetBeat(ceil(g_State.Beat()));
            break;

        case C_STEPVAL :
            if ( tokens.size() < 2 )
                throw std::string("Hint: step n");
            fTemp = std::stod(tokens[1].c_str());
            if ( fTemp != 0 )
            {
                set_status(STAT_POS_2, "Setting %s Step Value: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
                g_PatternStore.SetStepValue(fTemp);
//                if ( g_State.UsePatternPlayData() )
//                {
//                    g_PatternStore.CurrentEditPattern().StoreStepValue(fTemp);
//                    g_PatternStore.SetNewPatternPending(g_PatternStore.CurrentPlayPatternID() - 1);
//                }
//                else
//                    g_State.SetNewStepValuePending(fTemp);
            }
            else
            {
                set_status(STAT_POS_2, "Step Value cannot be zero.", tokens[1].c_str());
            }
            break;

        case C_GATELENGTH :
            if ( tokens.size() >= 2 )
            {
                fTemp = std::stod(tokens[1].c_str());
                if ( fTemp >= 0 )
                {
                    set_status(STAT_POS_2, "Setting %s Gate Length: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
                    g_PatternStore.SetGateLength(fTemp/100);
//                    if ( g_State.UsePatternPlayData() )
//                        g_PatternStore.CurrentEditPattern().StoreGate(fTemp/100);
//                    else
//                        g_State.SetGateLength(fTemp/100);
                }
                else
                {
                    set_status(STAT_POS_2, "Gate length cannot be negative.", tokens[1].c_str());
                }
            }
            else
                throw std::string("Hint: gate h[old]|n[ormal]|n.n%");
            break;
        case C_GATE_HOLD:
            g_PatternStore.SetGateHold(true);
//            if ( g_State.UsePatternPlayData() )
//                g_PatternStore.CurrentEditPattern().StoreGateHold(true);
//            else
//                g_State.SetExtendGateOverRests(true);
            break;
        case C_GATE_NORMAL:
            g_PatternStore.SetGateHold(false);
//            if ( g_State.UsePatternPlayData() )
//                g_PatternStore.CurrentEditPattern().StoreGateHold(false);
//            else
//                g_State.SetExtendGateOverRests(false);
            break;

        case C_VELOCITY :
            if ( tokens.size() < 2 )
                throw std::string("Hint: vel[ocity] n");
            iTemp = std::stoi(tokens[1].c_str());
            if ( iTemp >= 0 && iTemp <= 127 )
            {
                set_status(STAT_POS_2, "Setting %s velocity: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
                g_PatternStore.SetNoteVelocity(iTemp);
//                if ( g_State.UsePatternPlayData() )
//                    g_PatternStore.CurrentEditPattern().StoreVelocity(fTemp);
//                else
//                    g_State.SetNoteVelocity(iTemp);
            }
            else
            {
                set_status(STAT_POS_2, "Velocity must be in the range 0 .. 127.", tokens[1].c_str());
            }
            break;

        case C_TRANSPOSE :
            if ( tokens.size() < 2 )
                throw std::string("Hint: trans[pose] n [now]");
            iTemp = std::stoi(tokens[1].c_str());
            if ( tokens.size() >= 3 && tokens[2] == "now")
            {
                set_status(STAT_POS_2, "Transpose value set.");
                g_PatternStore.CurrentTranslateTableForEdit().SetTranspose(iTemp);
                set_top_line();
            }
            else
            {
                set_status(STAT_POS_2, "Setting transpose value: %s", tokens[1].c_str());
                g_PatternStore.CurrentTranslateTableForEdit().SetNewTransposePending(iTemp);
            }
            break;

        case C_FEEL:
            g_PatternStore.CurrentFeelMapForEdit();
            show_status_after_navigation();
            break;

        case C_SCALE:
            g_PatternStore.CurrentTranslateTableForEdit();    // This automatically sets focus.
            show_status_after_navigation();
            break;
//            if ( firstParameter == 0 )
//                show_translation_status();
//            else if ( firstParameter == 1 && g_PatternStore.CurrentTranslateTable().SetScale(tokens[1]) )
//                set_status(STAT_POS_2, "Scale set to %s-%s", g_PatternStore.CurrentTranslateTable().RootName().c_str(), g_PatternStore.CurrentTranslateTable().ScaleName());
//            else
//                set_status(STAT_POS_2, "Hint: scale 1..%i|major|natural|harmonic|help", static_cast<int>(scroll_top));
//            break;
        case C_SCALE_FROM_LIST:
            g_PatternStore.CurrentTranslateTableForEdit().SetScale(g_PatternStore.CurrentEditNoteList());
            show_translation_map_status();
            break;
        case C_SCALE_SHOW:
            show_translation_map_status();
            break;
        case C_SCALE_CONTROLS:
            set_status(STAT_POS_2, "Ctl: Tonal/Chromatic, Sh: Scale/Root, C+S: Accidentals/Premap");
            break;
        case C_SCALE_HELP:
            set_status(STAT_POS_2, "scale show|controls|[name|1..%i]|from list", static_cast<int>(scroll_top));
            break;

        case C_SETROOT :
            if ( tokens.size() < 2 || ! g_PatternStore.CurrentTranslateTableForEdit().SetRoot(tokens[1]) )
                throw std::string("Hint: root C, C#, Eb, C5, F#6, etc.");
            else
                show_translation_status();
            break;

        case C_MIDI :
            midi_setup(tokens);
            break;

        case C_QUANTUM :
            if ( tokens.size() < 2 )
                throw std::string("Hint: quan[tum] n.nn");
            fTemp = std::stod(tokens[1].c_str());
            if ( fTemp > 0 )
            {
                set_status(STAT_POS_2, "Setting new Quantum: %s", tokens[1].c_str());
                g_State.SetNewQuantumPending(fTemp);
                // newQuantum = fTemp;
            }
            break;

        case C_SET_RESETONPATTERNCHANGE : // Auto-reset
            if ( tokens.size() < 2 )
                throw std::string("Hint: autoreset on|off");
            if ( tokens[1] == "on")
                g_PatternStore.SetResetOnPatternChange(true);
            else if (tokens[1] == "off")
                g_PatternStore.SetResetOnPatternChange(false);
            else
                throw std::string("Autoreset not changed.");
            break;

        case C_PATTERN_CHAIN :
            if ( tokens.size() < 2 )
                throw std::string("Hint: chain off|natural|quantum|show");
            if ( tokens[1] == "off")
                g_PatternStore.SetPatternChainMode(PC_MODE_NONE);
            else if ( tokens[1] == "n" || tokens[1] == "natural" )
                g_PatternStore.SetPatternChainMode(PC_MODE_NATURAL);
            else if ( tokens[1] == "q" || tokens[1] == "quantum" )
                g_PatternStore.SetPatternChainMode(PC_MODE_FORCED);
            else if (tokens[1] == "show")
                set_status(STAT_POS_2, "%s", g_PatternStore.PatternChainToString().c_str());
            else
                g_PatternStore.UpdatePatternChainFromString(commandString);
            break;

        case C_LOAD:
        case C_STORE:
        case C_STORE_HELP:
            set_status(STAT_POS_2, "store|load step|scale|gate|vel|all");
            break;

        case C_STORE_STEP:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_STEP );
            set_status(STAT_POS_2, "Step Value stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_GATE:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_GATE );
            set_status(STAT_POS_2, "Gate Length stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_VELOCITY:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_VELO );
            set_status(STAT_POS_2, "Velociry stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_SCALE:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_SCALE );
            set_status(STAT_POS_2, "Scale settings stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_ALL:
            g_PatternStore.StorePatternPlayData();
            set_status(STAT_POS_2, "All play data stored to current edit pattern.");
            break;

        case C_LOAD_STEP:
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.LoadPatternPlayData(PLAY_DATA_STEP).c_str());
            break;
        case C_LOAD_GATE:
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.LoadPatternPlayData(PLAY_DATA_GATE).c_str());
            break;
        case C_LOAD_VELOCITY:
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.LoadPatternPlayData(PLAY_DATA_VELO).c_str());
            break;
        case C_LOAD_SCALE:
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.LoadPatternPlayData(PLAY_DATA_SCALE).c_str());
            break;
        case C_LOAD_ALL:
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.LoadPatternPlayData().c_str());
            break;

        case C_USE:
            set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());

            break;
        case C_USE_GLOBAL_PLAYDATA:
            g_PatternStore.SetUsePatternPlayData(false);
            set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());
            break;
        case C_USE_PATTERN_PLAYDATA:
            g_PatternStore.SetUsePatternPlayData(true);
            set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());
            break;
        case C_USE_HELP:
            set_status(STAT_POS_2, "use g[lobal]|p[attern]");
            break;

        case C_LIST :
            if ( tokens.size() < 2 )
                throw std::string("Hint: list new|delete|n [clear|: n1, n2 ,...]");
            set_status(STAT_POS_2, "%.60s", g_PatternStore.ListManager(commandString, tokens).c_str());
            update_pattern_panel();
            break;

        case C_HELP :
            if ( tokens.size() < 2 )
                do_help("");
            else
                do_help(tokens.at(1));
            break;

        case C_STATUS :
            show_status();
            break;

        case C_EXIT :
            bResult = false;
            break;

        case C_NONE :
            iTemp = std::stoi(tokens[0]) - 1;
            if ( ! g_PatternStore.ValidPosition(iTemp) )
                throw std::string("Requested pattern number out of range at the moment.");
            set_status(STAT_POS_2, "Cueing pattern: %i", iTemp + 1);
            g_PatternStore.SetNewPatternPending(iTemp);
            // newPattern = iTemp;
            break;
        default :
            break;
        }
    }
    catch (std::invalid_argument e)
    {
        set_status(STAT_POS_2, "Phrase not recognised. (%s)", e.what());
    }
    catch (std::out_of_range e)
    {
        set_status(STAT_POS_2, "Something went out of range ...");
    }
    catch ( std::string s )
    {
        set_status(STAT_POS_2, "%s", s.c_str());
    }

    commandString = "";
    return bResult;
}

void do_command_line(int argc, char *argv[])
{

    // Set console window title. (This doesn't work after NCurses starts.)

    std::string appTitle;
    if ( argc > 1 )
        for ( int i = 1; i < argc; i++ )
        {
            if ( std::strstr(argv[i], "-midi=") == argv[i] )
            {
                int iTemp = std::strtol(argv[i] + 6, NULL, 10);
                if ( iTemp >= 1 && iTemp <= 16)
                    g_Sequencer.SetMidiChannel(iTemp - 1);
            }
            else if ( std::strstr(argv[i], "-transpose=") == argv[i] )
            {
                g_PatternStore.CurrentTranslateTableForEdit().SetTranspose(std::strtol(argv[i] + 11, NULL, 10));
            }
            else if ( strstr(argv[i], "-tempo=") == argv[i] )
            {
                // TODO: Involves updating the timeline, which I haven't tried yet.
            }
            else if ( strstr(argv[i], "-step=") == argv[i] )
            {
                g_PatternStore.SetStepValue(strtof(argv[i] + 6, NULL));

            }
            else
            {
                if ( !appTitle.empty() )
                    appTitle += ' ';
                appTitle += argv[i];
            }
        }

    if ( appTitle.empty() )
        appTitle += "MultiArp";

    printf("%c]0;%s%c", '\033', appTitle.c_str(), '\007');
    g_Sequencer.SetClientName(appTitle.c_str());


}
