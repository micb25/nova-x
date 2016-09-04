#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

extern char getkeybbufsize();
extern void initkeyb();
extern char getkeybbufchar(char);

// Liest ein Zeichen ein
unsigned char getchar();
// Liest eine Zeichenkette ein, Rückgabewert ist die Anzahl der eingelesenen Zeichen 
int gets(unsigned char *fmt);   
// Setzt die LEDs auf der Tastatur
void setLEDs(
    unsigned char shift,
    unsigned char num,
    unsigned char scroll);
void checkspecialkeys();
void initkbmap();

#define SCANCODE_A 30
#define SCANCODE_B 48
#define SCANCODE_C 46
#define SCANCODE_D 32
#define SCANCODE_E 18
#define SCANCODE_F 33
#define SCANCODE_G 34
#define SCANCODE_H 35
#define SCANCODE_I 23
#define SCANCODE_J 36
#define SCANCODE_K 37
#define SCANCODE_L 38
#define SCANCODE_M 50
#define SCANCODE_N 49
#define SCANCODE_O 24
#define SCANCODE_P 25
#define SCANCODE_Q 16  
#define SCANCODE_R 19
#define SCANCODE_S 31
#define SCANCODE_T 20
#define SCANCODE_U 22
#define SCANCODE_V 47
#define SCANCODE_W 17
#define SCANCODE_X 45
#define SCANCODE_Y 44
#define SCANCODE_Z 21

#define SCANCODE_1 2
#define SCANCODE_2 3
#define SCANCODE_3 4
#define SCANCODE_4 5
#define SCANCODE_5 6
#define SCANCODE_6 7
#define SCANCODE_7 8
#define SCANCODE_8 9
#define SCANCODE_9 10
#define SCANCODE_0 11

#define SCANCODE_NB_7_POS1 71
#define SCANCODE_NB_8_UP 72
#define SCANCODE_NB_9_PUP 73
#define SCANCODE_NB_SUB 74
#define SCANCODE_NB_4_LEFT 75
#define SCANCODE_NB_5 76
#define SCANCODE_NB_6_RIGHT 77
#define SCANCODE_NB_ADD 78
#define SCANCODE_NB_1_END 79
#define SCANCODE_NB_2_DOWN 80
#define SCANCODE_NB_3_PDOWN 81
#define SCANCODE_NB_0_INS 82
#define SCANCODE_NB_COMMA 83

#define SCANCODE_NB_ENTER 158
#define SCANCODE_NB_MUL 55
#define SCANCODE_NB_DIV 95


#define SCANCODE_COMMAS 51
#define SCANCODE_POINTS 52
#define SCANCODE_HYPHEN 53 /* Bidnestrich/Minus */
#define SCANCODE_ADD 27
#define SCANCODE_SHARP 43

#define SCANCODE_AE 40
#define SCANCODE_OE 39
#define SCANCODE_UE 26

#define SCANCODE_SMALLER_BIGGER 86

#define SCANCODE_RIGHT_SHIFT 54
#define SCANCODE_LEFT_SHIFT 42
#define SCANCODE_SHIFT_LOCK 58
#define SCANCODE_TAB 15
#define SCANCODE_BACKSPACE 14
#define SCANCODE_ESCAPE 1
#define SCANCODE_RETURN 28
#define SCANCODE_SPACE 57
#define SCANCODE_LEFT_CTRL 29
#define SCANCODE_RIGHT_CTRL 64
#define SCANCODE_ALT 56
#define SCANCODE_ALT_GR 52
#define SCANCODE_NUM 69
#define SCANCODE_PRINT 28
#define SCANCODE_SCROLL 70

#define SCANCODE_UP_ARROW 224
#define SCANCODE_DOWN_ARROW 224
#define SCANCODE_LEFT_ARROW 224
#define SCANCODE_RIGHT_ARROW 224
/*
 * ASCII BLOCK
 */

#define ASCII_EXCLAMATION_MARK 33
#define ASCII_QUOTATION_MARK 34
#define ASCII_SHARP 35
#define ASCII_DOLLAR 36
#define ASCII_PER_CENT 37
#define ASCII_AND_MARK 38
#define ASCII_SPECIAL_1	40					/* ( */
#define ASCII_SPECIAL_2 41					/* ) */
#define ASCII_MULTIPLY 42
#define ASCII_ADD 43
#define ASCII_COMMA 44
#define ASCII_SUBTRACT 45
#define ASCII_POINT 46
#define ASCII_SLASH 47
#define ASCII_DOUBLE_POINT 58
#define ASCII_SEMICOLON 59
#define ASCII_SMALLER 60
#define ASCII_EQUAL_MARK 61
#define ASCII_BIGGER 62
#define ASCII_QUESTION_MARK 63
#define ASCII_PARAGRAPH 63					/* WRONG VALUE */
#define ASCII_SPACE 32					/* WRONG VALUE */

#define ASCII_0 48
#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_8 56
#define ASCII_9 57

#define ASCII_START_OF_HEADING 1
#define ASCII_START_OF_TEXT 2
#define ASCII_END_OF_TEXT 3
#define ASCII_END_OF_TRANSMISSION 4
#define ASCII_ENQUIRY 5
#define ASCII_ACKNOWLEDGE 6
#define ASCII_BELL 7
#define ASCII_BACKSPACE 8
#define ASCII_HORIZONTAL_TAB 9
#define ASCII_NEW_LINE 10
#define ASCII_VERTICAL_TAB 11
#define ASCII_NEW_PAGE 12
#define ASCII_CARRIAGE_RETURN 13
#define ASCII_SHIFT_OUT 14
#define ASCII_SHIFT_IN 15
#define ASCII_DATA_LINK_ESCAPE 16
/* ANDERE KOMMEN NOCH */
#define ASCII_ESCAPE 27


#define ASCII_SMALL_A 97
#define ASCII_SMALL_B 98
#define ASCII_SMALL_C 99
#define ASCII_SMALL_D 100
#define ASCII_SMALL_E 101
#define ASCII_SMALL_F 102
#define ASCII_SMALL_G 103
#define ASCII_SMALL_H 104
#define ASCII_SMALL_I 105
#define ASCII_SMALL_J 106
#define ASCII_SMALL_K 107
#define ASCII_SMALL_L 108
#define ASCII_SMALL_M 109
#define ASCII_SMALL_N 110
#define ASCII_SMALL_O 111
#define ASCII_SMALL_P 112
#define ASCII_SMALL_Q 113
#define ASCII_SMALL_R 114
#define ASCII_SMALL_S 115
#define ASCII_SMALL_T 116
#define ASCII_SMALL_U 117
#define ASCII_SMALL_V 118
#define ASCII_SMALL_W 119
#define ASCII_SMALL_X 120
#define ASCII_SMALL_Y 121
#define ASCII_SMALL_Z 122

#define ASCII_LARGE_A 65
#define ASCII_LARGE_B 66
#define ASCII_LARGE_C 67
#define ASCII_LARGE_D 68
#define ASCII_LARGE_E 69
#define ASCII_LARGE_F 70
#define ASCII_LARGE_G 71
#define ASCII_LARGE_H 72
#define ASCII_LARGE_I 73
#define ASCII_LARGE_J 74
#define ASCII_LARGE_K 75
#define ASCII_LARGE_L 76
#define ASCII_LARGE_M 77
#define ASCII_LARGE_N 78
#define ASCII_LARGE_O 79
#define ASCII_LARGE_P 80
#define ASCII_LARGE_Q 81
#define ASCII_LARGE_R 82
#define ASCII_LARGE_S 83
#define ASCII_LARGE_T 84
#define ASCII_LARGE_U 85
#define ASCII_LARGE_V 86
#define ASCII_LARGE_W 87
#define ASCII_LARGE_X 88
#define ASCII_LARGE_Y 89
#define ASCII_LARGE_Z 90

#define ASCII_SMALL_AE 132
#define ASCII_SMALL_OE 148
#define ASCII_SMALL_UE 129

#define ASCII_LARGE_AE 142
#define ASCII_LARGE_OE 153
#define ASCII_LARGE_UE 154

#endif
