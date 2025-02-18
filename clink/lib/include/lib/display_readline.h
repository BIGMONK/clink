#pragma once

#include <core/str.h>
#include <vector>

// Define USE_SUGGESTION_HINT_COMMENTROW to show "[Right]-Accept Suggestion"
// (with a hyperlink) in the comment row when there's suggestion text.
//#define USE_SUGGESTION_HINT_COMMENTROW

// Define USE_SUGGESTION_HINT_INLINE to show "[->]-Accept" (with a hyperlink)
// inline when there's suggestion text.
//#define USE_SUGGESTION_HINT_INLINE

class line_buffer;
typedef struct _history_expansion history_expansion;

extern void reset_readline_display();
extern void refresh_terminal_size();
extern void display_readline();
extern void set_history_expansions(history_expansion* list=nullptr);
extern void resize_readline_display(const char* prompt, const line_buffer& buffer, const char* _prompt, const char* _rprompt);
extern uint32 get_readline_display_top_offset();
extern bool translate_xy_to_readline(uint32 x, uint32 y, int32& pos, bool clip=false);

//------------------------------------------------------------------------------
#if defined(USE_SUGGESTION_HINT_COMMENTROW) || defined(USE_SUGGESTION_HINT_INLINE)
#define DOC_HYPERLINK_AUTOSUGGEST "https://chrisant996.github.io/clink/clink.html#gettingstarted_autosuggest"
#endif

//------------------------------------------------------------------------------
#ifdef USE_SUGGESTION_HINT_INLINE
#define STR_SUGGESTION_HINT_INLINE      "    ->-Accept"
#define IDX_SUGGESTION_KEY_BEGIN        (-9)
#define IDX_SUGGESTION_KEY_END          (-7)
#define IDX_SUGGESTION_LINK_TEXT        (-6)
#endif

//------------------------------------------------------------------------------
#define BIT_PROMPT_PROBLEM          (0x01)
#define BIT_PROMPT_MAYBE_PROBLEM    (0x02)
struct prompt_problem_details
{
    int32           type;
    str_moveable    code;
    int32           offset;
};
extern int32 prompt_contains_problem_codes(const char* prompt, std::vector<prompt_problem_details>* out=nullptr);

//------------------------------------------------------------------------------
#define FACE_INVALID        ((char)1)
#define FACE_SPACE          ' '
#define FACE_NORMAL         '0'
#define FACE_STANDOUT       '1'

#define FACE_INPUT          '2'
#define FACE_MODMARK        '*'
#define FACE_MESSAGE        '('
#define FACE_SCROLL         '<'
#define FACE_SELECTION      '#'
#define FACE_HISTEXPAND     '!'
#define FACE_SUGGESTION     '-'
#ifdef USE_SUGGESTION_HINT_INLINE
#define FACE_SUGGESTIONKEY  char(0x1a)  // In OEM 437 codepage, 0x1a is a right-arrow character.
#define FACE_SUGGESTIONLINK char(0x81)
#endif

#define FACE_OTHER          'o'
#define FACE_UNRECOGNIZED   'u'
#define FACE_EXECUTABLE     'x'
#define FACE_COMMAND        'c'
#define FACE_ALIAS          'd'
#define FACE_ARGMATCHER     'm'
#define FACE_ARGUMENT       'a'
#define FACE_FLAG           'f'
#define FACE_NONE           'n'

//------------------------------------------------------------------------------
class display_accumulator
{
public:
                    display_accumulator();
                    ~display_accumulator();
    void            split();
    void            flush();
private:
    void            restore();
    static void     fwrite_proc(FILE*, const char*, int32);
    static void     fflush_proc(FILE*);
    void (*m_saved_fwrite)(FILE*, const char*, int32) = nullptr;
    void (*m_saved_fflush)(FILE*) = nullptr;
    bool            m_active = false;
    static int32    s_nested;
};
