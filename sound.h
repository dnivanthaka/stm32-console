#ifndef SOUND_H
#define SOUND_H

void sound_init();
void sound_on();
void sound_off();
void soundq_init();
void soundq_push(uint8_t tone, uint16_t dur);
void soundq_process();

#endif