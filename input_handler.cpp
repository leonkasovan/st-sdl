#include "input_handler.h"

#define _GLIBCXX_USE_CXX11_ABI 0

#include <iostream>
#include <string>

// Control Keys.
static enum
{
	CTRLA = 0x01,
	CTRLB = 0x02,
	CTRLC = 0x03,
	CTRLD = 0x04,
	CTRLE = 0x05,
	CTRLF = 0x06,
	CTRLG = 0x07,
	CTRLH = 0x08,
	CTRLI = 0x09,
	CTRLJ = 0x0a,
	CTRLK = 0x0b,
	CTRLL = 0x0c,
	CTRLM = 0x0d,
	CTRLN = 0x0e,
	CTRLO = 0x0f,
	CTRLP = 0x10,
	CTRLQ = 0x11,
	CTRLR = 0x12,
	CTRLS = 0x13,
	CTRLT = 0x14,
	CTRLU = 0x15,
	CTRLV = 0x16,
	CTRLW = 0x17,
	CTRLX = 0x18,
	CTRLY = 0x19,
	CTRLZ = 0x1a
} CtrlKeys;

const static
unsigned char CTRL_KEY_TABLE[26] =
{
	CTRLA, CTRLB, CTRLC, CTRLD, CTRLE,
	CTRLF, CTRLG, CTRLH, CTRLI, CTRLJ,
	CTRLK, CTRLL, CTRLM, CTRLN, CTRLO,
	CTRLP, CTRLQ, CTRLR, CTRLS, CTRLT,
	CTRLU, CTRLV, CTRLW, CTRLX, CTRLY,
	CTRLZ
};

static std::string keyMap("LINUX"); //TODO: Allow changes?
static std::string inputSequence;

/**
 * @brief Used to Restart the SDL_GetText Event on changeing focus.
 */
void input_resetSDLTextInput()
{
    SDL_StopTextInput();
    SDL_StartTextInput();
	input_reset();
}

void setInputSequence(const std::string& sequence)
{
	inputSequence += sequence;
}

/**
 * @brief Handles SDL_TEXTINPUT Events.
 * @param event
 * @return
 */
bool handleTextInputEvent(SDL_Event &event)
{
    // Check for NumKey and Number Input,  IF Numlock if off
    // Then ignore numbers
    //if (!(event.key.keysym.mod & KMOD_NUM))
    if(strlen(event.text.text) > 0)
        setInputSequence(event.text.text);
    else
        return false;

    return true;
}

/**
 * @brief Handles Shift + CTRL + _ Event
 * @param event
 * @return
 */
bool handleShiftControlKeys(SDL_Event &event)
{
    // Special for some editors
    switch(event.key.keysym.sym)
    {
        case 45: // UnderScore
            //std::cout << "createUnderScoreSequence" << std::endl;
            setInputSequence("\x1f");
            return true;

        default:
            // Testing
            //std::cout << "event.key.keysym.sym: "
            //<< event.key.keysym.sym << std::endl;
            break;
    }
    return false;
}

/**
 * @brief Handles CTRL Key Sequences
 * @param event
 * @return
 */
bool handleControlKeys(SDL_Event &event)
{
    // CTRL Keys have lowercase letters -
    // translate CTRL Key combo and
    // Grab the Current Key Symbol.
    char ch = static_cast<char>(event.key.keysym.sym);
    std::string sequence(&ch);
    if((event.key.keysym.sym >= (Uint16)'a') &&
            (event.key.keysym.sym <= (Uint16)'z'))
    {
        // Translate Letter to CTRL Letter value
        ch = CTRL_KEY_TABLE[ch-97];
        sequence = ch;
        setInputSequence(sequence);
        return true;
    }
    return false;
}

/**
 * @brief Handles ALT Key Sequences & Window Resizing. Unused for now.
 * @param event
 * @return
 */
bool handleAlternateKeys(SDL_Event &event)
{
    switch(event.key.keysym.sym)
    {
        default:
            break;
    }
    return false;
}

/**
 * @brief Handles Special Case Shit And Default Keypad Sequences
 * @param event
 * @return
 */
bool handleKeyPadAndFunctionKeys(SDL_Event &event)
{
    switch(event.key.keysym.sym)
    {
        // Handle KeyPad Keys without numlock
        // Numlock Numbers are read by Text Input already.
        /* Caught by TextInput()
        case SDLK_KP_PERIOD:
            if (event.key.keysym.mod & KMOD_NUM)
            {
                setInputSequence(".");
                return true;
            }
            break;*/
        case SDLK_KP_8: // UP
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[A");
                return true;
            }
            break;
        case SDLK_KP_2: // DN
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[B");
                return true;
            }
            break;
        case SDLK_KP_6: // RIGHT
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[C");
                return true;
            }
            break;
        case SDLK_KP_4: // LEFT
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[D");
                return true;
            }
            break;

        case SDLK_ESCAPE:
            setInputSequence("\x1b");
            return true;

        // Add Toggle for Hardware Keys ESC0A etc..
        case SDLK_UP:
            if(event.key.keysym.mod & KMOD_SHIFT)
                setInputSequence("\x1b[1;2A");
            else
                setInputSequence("\x1b[A");
            return true;
        case SDLK_DOWN:
            if(event.key.keysym.mod & KMOD_SHIFT)
                setInputSequence("\x1b[1;2B");
            else
                setInputSequence("\x1b[B");
            return true;
        case SDLK_RIGHT:
            if(event.key.keysym.mod & KMOD_SHIFT)
                setInputSequence("\x1b[1;2C");
            else
                setInputSequence("\x1b[C");
            return true;
        case SDLK_LEFT:
            if(event.key.keysym.mod & KMOD_SHIFT)
                setInputSequence("\x1b[1;2D");
            else
                setInputSequence("\x1b[D");
            return true;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            setInputSequence("\r");
            return true;

        // Add Swap for BS and DEL (Win vs Nix Terms)
        case SDLK_TAB:
            if(event.key.keysym.mod & KMOD_SHIFT)
                setInputSequence("\x1b[Z");
            else
                setInputSequence("\t");
            return true;

        default:
            break;
    }
    return false;
}

/**
 * @brief Handle ANSI Keypad Sequences (DOS)
 * @param event
 * @return
 */
bool handleANSIKeyMapFunctionKeys(SDL_Event &event)
{
    // screen2|old VT 100/ANSI X3.64 virtual terminal:
    // Used By Synchronet/Mystic and DOS Type Systems!
    switch(event.key.keysym.sym)
    {
        case SDLK_KP_BACKSPACE:
        case SDLK_BACKSPACE:
            setInputSequence("\x08");
            return true;
        case SDLK_DELETE:
            setInputSequence("\x7f");
            return true;

        case SDLK_INSERT:   // insert
            setInputSequence("\x1b[@");
            return true;
        case SDLK_HOME:     // home
            setInputSequence("\x1b[H");
            return true;
        case SDLK_END:      // end
            setInputSequence("\x1b[K");
            return true;
        case SDLK_PAGEUP:   // page up
            setInputSequence("\x1b[V");
            return true;
        case SDLK_PAGEDOWN: // page down
            setInputSequence("\x1b[U");
            return true;

        // Handle KeyPad Keys without Numlock
        // Numlock Numbers are read by Text Input already.
        case SDLK_KP_PERIOD: // Delete
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x7f");
                return true;
            }
            break;
        case SDLK_KP_0: // INS
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[@");
                return true;
            }
            break;
        case SDLK_KP_1: // END
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[K");
                return true;
            }
            break;
        case SDLK_KP_3: // PGDN
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[U");
                return true;
            }
            break;
        case SDLK_KP_5: // Empty, space?
            if(!(event.key.keysym.mod & KMOD_NUM))
            {

            }
            break;
        case SDLK_KP_7: // HOME
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[H");
                return true;
            }
            break;
        case SDLK_KP_9: // PAGEUP
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[V");
                return true;
            }
            break;

        // Function Keys
        case SDLK_F1:
            setInputSequence("\x1b[OP");
            return true;
        case SDLK_F2:
            setInputSequence("\x1b[OQ");
            return true;
        case SDLK_F3:
            setInputSequence("\x1b[OR");
            return true;
        case SDLK_F4:
            setInputSequence("\x1b[OS");
            return true;
        case SDLK_F5:
            // Windows Console Telnet \\x1b[[15~
            setInputSequence("\x1b[OT");
            return true;
        case SDLK_F6:
            setInputSequence("\x1b[[17~");
            return true;
        case SDLK_F7:
            setInputSequence("\x1b[[18~");
            return true;
        case SDLK_F8:
            setInputSequence("\x1b[[19~");
            return true;
        case SDLK_F9:
            setInputSequence("\x1b[[20~");
            return true;
        case SDLK_F10:
            setInputSequence("\x1b[[21~");
            return true;
        case SDLK_F11:
            setInputSequence("\x1b[[23~");
            return true;
        case SDLK_F12:
            setInputSequence("\x1b[[24~");
            return true;

        default:
            break;
    }
    return false;
}

/**
 * @brief Handles VT100 (Terminals Sequences)
 * @param event
 * @return
 */
bool handleVT100KeyMapFunctionKeys(SDL_Event &event)
{
    // VT-100 Putty
    switch(event.key.keysym.sym)
    {
        // \x7f = ^?  // 0x08 = ^H
        case SDLK_KP_BACKSPACE:
        case SDLK_BACKSPACE:
            setInputSequence("\x7f");
            return true;
        case SDLK_DELETE:
            setInputSequence("\x1b[3~");
            return true;

        case SDLK_INSERT:   // insert
            setInputSequence("\x1b[2~");
            return true;
        case SDLK_HOME:     // home
            setInputSequence("\x1b[1~");
            return true;
        case SDLK_END:      // end
            setInputSequence("\x1b[4~");
            return true;
        case SDLK_PAGEUP:   // page up
            setInputSequence("\x1b[5~");
            return true;
        case SDLK_PAGEDOWN: // page down
            setInputSequence("\x1b[6~");
            return true;

        // Handle KeyPad Keys without NumLock
        // NumLock Numbers are read by Text Input already.
        case SDLK_KP_PERIOD: // Delete
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[3~");
                return true;
            }
            break;
        case SDLK_KP_0: // INS
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[2~");
                return true;
            }
            break;
        case SDLK_KP_1: // END
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[4~");
                return true;
            }
            break;
        case SDLK_KP_3: // PGDN
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[6~");
                return true;
            }
            break;
        case SDLK_KP_5: // Empty, space?
            if(!(event.key.keysym.mod & KMOD_NUM))
            {

            }
            break;
        case SDLK_KP_7: // HOME
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[1~");
                return true;
            }
            break;
        case SDLK_KP_9: // PAGEUP
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[5~");
                return true;
            }
            break;

        // Function Keys
        case SDLK_F1:
            setInputSequence("\x1b[OP");
            return true;
        case SDLK_F2:
            setInputSequence("\x1b[OQ");
            return true;
        case SDLK_F3:
            setInputSequence("\x1b[OR");
            return true;
        case SDLK_F4:
            setInputSequence("\x1b[OS");
            return true;
        case SDLK_F5:
            setInputSequence("\x1b[OT");
            return true;
        case SDLK_F6:
            setInputSequence("\x1b[OU");
            return true;
        case SDLK_F7:
            setInputSequence("\x1b[OV");
            return true;
        case SDLK_F8:
            setInputSequence("\x1b[OW");
            return true;
        case SDLK_F9:
            setInputSequence("\x1b[OX");
            return true;
        case SDLK_F10:
            setInputSequence("\x1b[OY");
            return true;
        case SDLK_F11:
            setInputSequence("\x1b[OZ");
            return true;
        case SDLK_F12:
            setInputSequence("\x1b[O[");
            return true;

        default:
            break;
    }
    return false;
}

/**
 * @brief Handles LINUX/XTERM (Terminals Sequences)
 * @param event
 * @return
 */
bool handleLINUXKeyMapFunctionKeys(SDL_Event &event)
{
    // Linux Terminal Putty
    switch(event.key.keysym.sym)
    {
        // \x7f = ^?  // 0x08 = ^H
        case SDLK_KP_BACKSPACE:
        case SDLK_BACKSPACE:
            setInputSequence("\x7f");
            return true;
        case SDLK_DELETE:
            setInputSequence("\x1b[3~");
            return true;

        case SDLK_INSERT:   // insert
            setInputSequence("\x1b[2~");
            return true;
        case SDLK_HOME:     // home
            setInputSequence("\x1b[1~");
            return true;
        case SDLK_END:      // end
            setInputSequence("\x1b[4~");
            return true;
        case SDLK_PAGEUP:   // page up
            setInputSequence("\x1b[5~");
            return true;
        case SDLK_PAGEDOWN: // page down
            setInputSequence("\x1b[6~");
            return true;

        // Handle KeyPad Keys without NumLock
        // NumLock Numbers are read by Text Input already.
        case SDLK_KP_PERIOD: // Delete
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[3~");
                return true;
            }
            break;
        case SDLK_KP_0: // INS
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[2~");
                return true;
            }
            break;
        case SDLK_KP_1: // END
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[4~");
                return true;
            }
            break;
        case SDLK_KP_3: // PGDN
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[6~");
                return true;
            }
            break;
        case SDLK_KP_5: // Empty, space?
            if(!(event.key.keysym.mod & KMOD_NUM))
            {

            }
            break;
        case SDLK_KP_7: // HOME
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[1~");
                return true;
            }
            break;
        case SDLK_KP_9: // PAGEUP
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[5~");
                return true;
            }
            break;

        // Function Keys
        case SDLK_F1:
            setInputSequence("\x1b[[A");
            return true;
        case SDLK_F2:
            setInputSequence("\x1b[[B");
            return true;
        case SDLK_F3:
            setInputSequence("\x1b[[C");
            return true;
        case SDLK_F4:
            setInputSequence("\x1b[[D");
            return true;
        case SDLK_F5:
            setInputSequence("\x1b[[E");
            return true;
        case SDLK_F6:
            setInputSequence("\x1b[[17~");
            return true;
        case SDLK_F7:
            setInputSequence("\x1b[[18~");
            return true;
        case SDLK_F8:
            setInputSequence("\x1b[[19~");
            return true;
        case SDLK_F9:
            setInputSequence("\x1b[[20~");
            return true;
        case SDLK_F10:
            setInputSequence("\x1b[[21~");
            return true;
        case SDLK_F11:
            setInputSequence("\x1b[[23~");
            return true;
        case SDLK_F12:
            setInputSequence("\x1b[[24~");
            return true;

        default:
            break;
    }
    return false;
}

/**
 * @brief Handles SCO (Terminals Sequences)
 * @param event
 * @return
 */
bool handleSCOKeyMapFunctionKeys(SDL_Event &event)
{
    // SCO Putty
    switch(event.key.keysym.sym)
    {
        // \x7f = ^?  // 0x08 = ^H
        case SDLK_KP_BACKSPACE:
        case SDLK_BACKSPACE:
            setInputSequence("\x7f");
            return true;
        case SDLK_DELETE:
            setInputSequence("\x08");
            return true;
        case SDLK_INSERT:   // insert
            setInputSequence("\x1b[L");
            return true;
        case SDLK_HOME:     // home
            setInputSequence("\x1b[H");
            return true;
        case SDLK_END:      // end
            setInputSequence("\x1b[F");
            return true;
        case SDLK_PAGEUP:   // page up
            setInputSequence("\x1b[I");
            return true;
        case SDLK_PAGEDOWN: // page down
            setInputSequence("\x1b[G");
            return true;

        // Handle KeyPad Keys without NumLock
        // NumLock Numbers are read by Text Input already.
        case SDLK_KP_PERIOD: // Delete
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x08");
                return true;
            }
            break;
        case SDLK_KP_0: // INS
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[L");
                return true;
            }
            break;
        case SDLK_KP_1: // END
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[F");
                return true;
            }
            break;
        case SDLK_KP_3: // PGDN
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[G");
                return true;
            }
            break;
        case SDLK_KP_5: // Empty, space?
            if(!(event.key.keysym.mod & KMOD_NUM))
            {

            }
            break;
        case SDLK_KP_7: // HOME
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[H");
                return true;
            }
            break;
        case SDLK_KP_9: // PAGEUP
            if(!(event.key.keysym.mod & KMOD_NUM))
            {
                setInputSequence("\x1b[I");
                return true;
            }
            break;

        // Function Keys
        case SDLK_F1:
            setInputSequence("\x1b[[M");
            return true;
        case SDLK_F2:
            setInputSequence("\x1b[[N");
            return true;
        case SDLK_F3:
            setInputSequence("\x1b[[O");
            return true;
        case SDLK_F4:
            setInputSequence("\x1b[[P");
            return true;
        case SDLK_F5:
            setInputSequence("\x1b[[Q");
            return true;
        case SDLK_F6:
            setInputSequence("\x1b[[R");
            return true;
        case SDLK_F7:
            setInputSequence("\x1b[[S");
            return true;
        case SDLK_F8:
            setInputSequence("\x1b[[T");
            return true;
        case SDLK_F9:
            setInputSequence("\x1b[[U");
            return true;
        case SDLK_F10:
            setInputSequence("\x1b[[V");
            return true;
        case SDLK_F11:
            setInputSequence("\x1b[[W");
            return true;
        case SDLK_F12:
            setInputSequence("\x1b[[X");
            return true;

        default:
            break;
    }
    return false;
}

/**
 * @brief Handles Function Key Sequences
 * @param event
 * @return
 */
bool handleKeyDownEvents(SDL_Event &event)
{

    // Handle Shift + CTRL + _
    if(event.key.keysym.mod & KMOD_SHIFT &&
            event.key.keysym.mod & KMOD_CTRL)
    {
        return handleShiftControlKeys(event);
    }
    // Handle CTRL + KEY
    if(event.key.keysym.mod & KMOD_CTRL)
    {
        return handleControlKeys(event);
    } // END CTRL

    // Handle ALT + KEY
    else if(event.key.keysym.mod & KMOD_ALT)
    {
        return handleAlternateKeys(event);
    } // End ALT

    // Get remaining function keys not handled on TextInput()
    // Then Translate to ESC Sequences for Telnet.
    else
    {
        if(!handleKeyPadAndFunctionKeys(event))
        {
            // Term specific key mappings
            if(keyMap == "ANSI")
            {
                if(handleANSIKeyMapFunctionKeys(event))
                    return true;
            }
            else if(keyMap == "VT100")
            {
                if(handleVT100KeyMapFunctionKeys(event))
                    return true;
            }
            else if(keyMap == "LINUX")
            {
                if(handleLINUXKeyMapFunctionKeys(event))
                    return true;
            }
            else if(keyMap == "SCO")
            {
                if(handleSCOKeyMapFunctionKeys(event))
                    return true;
            }
        }
    }
    return false;
}

/**
 * @brief Handles single input event for session.
 * @param event
 * @return
 */
bool input_update(SDL_Event* event)
{
	bool result = true;
    switch(event->type)
    {
        case SDL_TEXTINPUT:
            result = handleTextInputEvent(*event);
            break;
        case SDL_KEYDOWN:
			handleKeyDownEvents(*event);
            break;

        default:
			result = false;
            break;
    }

    // If Input was received return true.
    if(result && !inputSequence.empty())
        return true;

    return false;
}

void input_reset(){
	inputSequence.clear();
}

const char* input_getSequence(){
	return inputSequence.c_str();
}
