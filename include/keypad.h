#ifndef KEYPAD_H
#define KEYPAD_H

//keypad keys, check macro
#define KEYPAD_UP(val)    (!(val & (1 << 0)))
#define KEYPAD_DOWN(val)  (!(val & (1 << 1)))
#define KEYPAD_LEFT(val)  (!(val & (1 << 2)))
#define KEYPAD_RIGHT(val) (!(val & (1 << 3)))
#define KEYPAD_A(val)     (!(val & (1 << 4)))
#define KEYPAD_B(val)     (!(val & (1 << 5)))
#define KEYPAD_SELECT(val) (!(val & (1 << 7)))
#define KEYPAD_START(val)  (!(val & (1 << 6)))

#define KEYPAD_DEFAULT_VAL 0xff

void keypad_init();
void keypad_read();
uint8_t keypad_keypressed();
uint8_t keypad_getstate();

#endif
