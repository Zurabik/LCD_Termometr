/*
 * Timer.h
 *
 * Created: 16.08.2014 15:47:25
 *  Author: Vadim2
 */ 
#ifndef TIMER_H_
#define TIMER_H_

#include "IR_LG_device.h"
#include "nec.h"
#include "ExternLoad.h"
#include "shift_register.h"
#include "Temper.h"

#define Tic_10mS (255-(F_CPU/102400))

void Init_Timer0();

void Get_Timer0_100mS();//сдесь функция 10 раз в 1 сек

void Get_Timer0_1S();//сдесь функция 1 раз в 1 сек

void Get_Timer0_10S();//сдесь функция 1 раз в 10 сек

void Get_Timer0_60S();//сдесь функция 1 раз в 1 мин

void Get_Timer0_60M();//сдесь функция 1 раз в 1 час

void Check_Timer0();

#endif