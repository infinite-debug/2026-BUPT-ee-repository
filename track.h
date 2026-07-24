#ifndef TRACK_H_
#define TRACK_H_

#include <stdint.h>

/*
 * 执行一次完整的循迹处理：
 *
 * 1. 调用 Gray_ReadLevels()；
 * 2. 判断哪些通道检测到黑线；
 * 3. 计算加权平均 error；
 * 4. 在 track.c 内判断直行、左转、右转或丢线。
 */
void Track_Run(void);


/*
 * 以下两个函数为调试使用（后续可删）。分别为原始电平和偏差值
 */
uint8_t Track_GetRawLevels(void);

int16_t Track_GetError(void);

#endif /* TRACK_H_ */