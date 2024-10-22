#ifndef _TIME_H_
#define _TIME_H_

/**
 * @brief Timer interrupt handler.
 *
 * This function is called on each timer interrupt.
 */
void timer_handler();

/**
 * @brief Retrieves the number of ticks elapsed since the system started.
 *
 * This function returns the number of timer ticks that have elapsed since the system was started.
 *
 * @return The number of ticks elapsed.
 */
int ticks_elapsed();

/**
 * @brief Retrieves the number of seconds elapsed since the system started.
 *
 * This function returns the number of seconds that have elapsed since the system was started.
 *
 * @return The number of seconds elapsed.
 */
int seconds_elapsed();

/**
 * @brief Suspends the execution of the calling thread for a specified duration.
 *
 * This function causes the calling thread to sleep for the specified number of seconds.
 * During this time, the thread will not execute any instructions.
 *
 * @param seconds The number of seconds for which the thread should sleep.
 */

void sleep(int seconds);

/**
 * @brief Retrieves the current system time as a string.
 *
 * This function returns the current system time formatted as a string.
 * The format of the returned time string is "dd/mm/yy-HH:MM:SS".
 *
 * @return A pointer to a character array containing the current system time.
 */
char * getTime();

#endif
