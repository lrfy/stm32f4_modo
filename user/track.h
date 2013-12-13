#ifndef __TRACK_H
#define __TRACK_H


void Origin_Correct(u16 x, u16 y);
void Translate_Co(u16 x, u16 y);
void Track_Clear(void);
void Track_Reset(void);
void Track_Show(u16 x, u16 y);
u8 Triangle_Jud(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);

#endif
