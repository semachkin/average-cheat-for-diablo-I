#ifndef DATA_H
#define DATA_H

#define VOID_STRING         ""
#define REGISTRATION_FAILED "window registration failed"
#define ERROR_MESSAGE       "error"
#define WINDOW_CLASS        "wndclass"
#define WINDOW_NAME         "diablo cheat window"
#define BUTTON_CLASS        "button"
#define BOX_CLASS           "edit"
#define BUTTON_1_NAME       "set health"
#define BOX_1_NAME          "health box"
#define PROCESS_WAIT_TEXT   "need to start Diablo process"
#define WRONG_HEALTH        "Wrong health count"
#define DIABLO_PROCESS_NAME "Diablo_dump.exe"                 // SET THIS VALUE TO YOUR GAME FILE

#define ZERO         0
#define EXTRA_BYTES  ZERO
#define QUIT_MESSAGE ZERO
#define BUTTON_STYLE ZERO

#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 200
#define WINDOW_SIZE   WINDOW_WIDTH, WINDOW_HEIGHT

#define BUTTON_WIDTH  100
#define BUTTON_HEIGHT 25
#define BUTTON_SIZE   BUTTON_WIDTH, BUTTON_HEIGHT

#define BUTTON_1_X_POSITION 5
#define BUTTON_1_Y_POSITION 5
#define BUTTON_1_POSITION   BUTTON_1_X_POSITION, BUTTON_1_Y_POSITION

#define BOX_WIDTH  50
#define BOX_HEIGHT 25
#define BOX_SIZE   BOX_WIDTH, BOX_HEIGHT

#define BOX_1_X_POSITION BUTTON_1_X_POSITION * 2 + BUTTON_WIDTH
#define BOX_1_Y_POSITION BUTTON_1_Y_POSITION
#define BOX_1_POSITION   BOX_1_X_POSITION, BOX_1_Y_POSITION

#define BUTTON_1_ACTIVATED 0x0002
#define BOX_1_ACTIVATED    0x0003

#define PROCESS_ERROR_SUCCESS              0x0000
#define PROCESS_ERROR_CANNONT_FIND         0x1001
#define PROCESS_ERROR_INVALID_HANDLE_VALUE 0x1002
#define PROCESS_ERROR_EXIT_UNSUCESSFUL     0x1003

typedef unsigned int process_error_t;

#define NULL_2X NULL, NULL
#define NULL_3X NULL_2X, NULL
#define ZERO_2X ZERO, ZERO

#define UNKNOWN_SIZE -1

#define DIABLO_HEALTH_ADDRESS_OFFSET     0x002865E0

#define MESSAGE_NEED_OPEN_DIABLO 0x2001
#define MESSAGE_WRONG_HEALTH     0x2002 
#define MESSAGE_ERROR            0x2000

#define MESSAGE_CLOSE_TIME 200

#define MAX_HEALTH_TEXT_LENGTH 10

#endif
