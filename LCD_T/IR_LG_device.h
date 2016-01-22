/*
 * IR_LG_device.h
 *
 * Created: 08.09.2014 21:30:43
 *  Author: Vadim Belickiy
 */ 

/*коды кнопок пульта LG TV AKB73715603 
 *Протокл: NEC
 */
#ifndef IR_LG_DEVICE_H_
#define IR_LG_DEVICE_H_
#define DEVICE_ADDRESS	0x04	//адрес нашего пульта
#define BT_POWER		0x08
#define BT_TV_RAD		0xf0
#define BT_SUBTITLE		0x39
#define BT_AV_MODE		0x30
#define BT_RATIO		0x79
#define BT_INPUT		0x0b
#define BT_1			0x11
#define BT_2			0x12
#define BT_3			0x13
#define BT_4			0x14
#define BT_5			0x15
#define BT_6			0x16
#define BT_7			0x17
#define BT_8			0x18
#define BT_9			0x19
#define BT_0			0x10
#define BT_LIST			0x53
#define BT_Q_VIEW		0x1a
#define BT_VOL_UP		0x02
#define BT_VOL_DOWN		0x03
#define BT_PAGE_UP		0x00
#define BT_PAGE_DOWN	0x01
#define BT_FAV			0x1e
#define BT_GUIDE		0xab
#define BT_MUTE			0x09
#define BT_TEXT			0x20
#define BT_INFO			0xaa
#define BT_T_OPT		0x21
#define BT_SETTING		0x43
#define BT_UP			0x40
#define BT_Q_MENU		0x45
#define BT_LEFT			0x07
#define BT_OK			0x44
#define BT_RIGTH		0x06
#define BT_BACK			0x28
#define BT_DOWN			0x41
#define BT_EXIT			0x5b
#define BT_SIMPLINK		0x7e
#define BT_AD			0x91
#define BT_STOP			0xb1
#define BT_REWIND		0x8f
#define BT_PLAY			0xb0
#define BT_PAUSE		0xba
#define BT_FORWARD		0x8e
#define BT_RED			0x72
#define BT_GREEN		0x71
#define BT_YELLOW		0x63
#define BT_BLUE			0x61


#endif /* IR_LG_DEVICE_H_ */