#ifndef DATA_H
#define DATA_H

#define VOID_STRING         ""
#define REGISTRATION_FAILED "window registration failed"
#define ERROR_MESSAGE       "error"
#define WINDOW_CLASS        "wndclass"
#define WINDOW_NAME         "diablo cheat window"
#define DIABLO_PROCESS_NAME "Diablo_dump.exe" // change this macros to your diablo exe file name

#define BUTTON_CLASS        "button"
#define BOX_CLASS           "edit"

#define BUTTON_NAME         "update character feature"

#define BOX_1_NAME          "health"
#define BOX_2_NAME          "strength"
#define BOX_3_NAME          "dexterity"
#define BOX_4_NAME          "vitality"

#define PROCESS_WAIT_TEXT   "Need to start Diablo process"
#define WRONG_HEALTH        "Wrong health count"
#define WRONG_STRENGTH      "Wrong strength count"
#define WRONG_DEXTERITY     "Wrong dexterity count"
#define WRONG_VITALITY      "Wrong vitality count"

#define ZERO         0
#define EXTRA_BYTES  ZERO
#define QUIT_MESSAGE ZERO
#define BUTTON_STYLE ZERO

#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 250
#define WINDOW_SIZE   WINDOW_WIDTH, WINDOW_HEIGHT

#define BUTTON_WIDTH  200
#define BUTTON_HEIGHT 25
#define BUTTON_SIZE   BUTTON_WIDTH, BUTTON_HEIGHT

#define BUTTON_X_POSITION 5
#define BUTTON_Y_POSITION 5
#define BUTTON_POSITION   BUTTON_X_POSITION, BUTTON_Y_POSITION

#define BOX_WIDTH  50
#define BOX_HEIGHT 25
#define BOX_SIZE   BOX_WIDTH, BOX_HEIGHT

#define BOX_X_OFFSET 5
#define BOX_Y_OFFSET 5

#define BOX_1_X_POSITION BOX_X_OFFSET
#define BOX_1_Y_POSITION BOX_Y_OFFSET + BOX_HEIGHT + BUTTON_Y_POSITION
#define BOX_1_POSITION   BOX_1_X_POSITION, BOX_1_Y_POSITION

#define BOX_2_X_POSITION BOX_X_OFFSET
#define BOX_2_Y_POSITION BOX_Y_OFFSET + BOX_HEIGHT + BOX_1_Y_POSITION
#define BOX_2_POSITION   BOX_2_X_POSITION, BOX_2_Y_POSITION

#define BOX_3_X_POSITION BOX_X_OFFSET
#define BOX_3_Y_POSITION BOX_Y_OFFSET + BOX_HEIGHT + BOX_2_Y_POSITION
#define BOX_3_POSITION   BOX_3_X_POSITION, BOX_3_Y_POSITION

#define BOX_4_X_POSITION BOX_X_OFFSET
#define BOX_4_Y_POSITION BOX_Y_OFFSET + BOX_HEIGHT + BOX_3_Y_POSITION
#define BOX_4_POSITION   BOX_4_X_POSITION, BOX_4_Y_POSITION

#define BUTTON_EX_STYLE WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON
#define BOX_EX_STYLE    WS_CHILD | WS_VISIBLE

#define BUTTON_ACTIVATED 0x1000

#define BOX_1_ACTIVATED  0x2000
#define BOX_2_ACTIVATED  0x2001
#define BOX_3_ACTIVATED  0x2002
#define BOX_4_ACTIVATED  0x2003

typedef unsigned int process_error_t;

#define NULL_2X NULL, NULL
#define NULL_3X NULL_2X, NULL
#define ZERO_2X ZERO, ZERO

#define UNKNOWN_SIZE -1

#define DIABLO_PLAYER_CLASS_ADRESS_OFFSET 0x00286500

#define DIABLO_HEALTH_ADDRESS_OFFSET         DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00E0
#define DIABLO_MAX_HEALTH_ADDRESS_OFFSET     DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00E4
#define DIABLO_STRENGTH_ADDRESS_OFFSET       DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00AC
#define DIABLO_STRENGTH_CMP_ADDRESS_OFFSET   DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00B0
#define DIABLO_DEXTERITY_ADDRESS_OFFSET      DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00BC
#define DIABLO_DEXTERITY_CMP_ADDRESS_OFFSET  DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00C0
#define DIABLO_VITALITY_ADDRESS_OFFSET       DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00C4
#define DIABLO_VITALITY_CMP_ADDRESS_OFFSET   DIABLO_PLAYER_CLASS_ADRESS_OFFSET + 0x00C8

#define DIABLO_MODULES_COUNT 4

#define MESSAGE_NEED_OPEN_DIABLO 0x3001
#define MESSAGE_WRONG_HEALTH     0x3002 
#define MESSAGE_WRONG_STRENGTH   0x3003 
#define MESSAGE_WRONG_DEXTERITY  0x3004 
#define MESSAGE_WRONG_VITALITY   0x3005 
#define MESSAGE_ERROR            0x3000

#define PROCESS_ERROR_SUCCESS              0x4000
#define PROCESS_ERROR_CANNONT_FIND         0x4001
#define PROCESS_ERROR_INVALID_HANDLE_VALUE 0x4002
#define PROCESS_ERROR_EXIT_UNSUCESSFUL     0x4003

#define MESSAGE_CLOSE_TIME 200

#define MAX_HEALTH_TEXT_LENGTH 10

#endif
