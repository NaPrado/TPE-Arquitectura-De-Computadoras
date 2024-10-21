#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
// Function declarations and macros for the Keyboard driver

/**
 * @brief Handles keyboard interrupts.
 * 
 * This function is called whenever a keyboard interrupt occurs.
 * It processes the key press and performs the necessary actions.
 */
void keyboard_handler();

/**
 * @brief Retrieves the current key from the keyboard buffer.
 * 
 * This function returns the current key from the keyboard buffer without
 * removing it. It can be used to peek at the current key.
 * 
 * @return char The current key.
 */
char getKey();

/**
 * @brief Checks if there is a next key available.
 * 
 * @return char 1 if there is a next key, 0 otherwise.
 */
char hasNextKey();

/**
 * @brief Retrieves the next key from the keyboard buffer.
 * 
 * @return char The next key.
 */
char nextKey();

/**
 * @brief Checks if Caps Lock is active.
 * 
 * @return char 1 if Caps Lock is active, 0 otherwise.
 */
char getCapslock();

/**
 * @brief Checks if Shift is pressed.
 * 
 * @return char 1 if Shift is pressed, 0 otherwise.
 */
char getShiftPressed();

/**
 * @brief Checks if Ctrl is pressed.
 * 
 * @return char 1 if Ctrl is pressed, 0 otherwise.
 */
char getCtrlPressed();

/**
 * @brief Checks if Alt is pressed.
 * 
 * @return char 1 if Alt is pressed, 0 otherwise.
 */
char getAltPressed();


#endif // KEYBOARD_DRIVER_H