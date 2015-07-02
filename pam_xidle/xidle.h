/* 
 * File:   xidle.h
 * Author: slawek
 *
 * Created on 16 czerwiec 2015, 17:48
 */
#include <stdbool.h>

#ifndef XIDLE_H
#define	XIDLE_H

int test_void_fun();
int det_user_interrupt(const int sample_period, const int debounces, 
        bool verbose);
int GetIdleTime ();

#endif	/* XIDLE_H */

