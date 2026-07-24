#include "track.h"
#include "gray.h"

/*
 * 黑线对应的 OUT 电平。
 */
#define GRAY_LINE_LEVEL         (0U)

/*
 * error 在 -1～+1 时按直行处理。
 */
#define TRACK_STRAIGHT_RANGE    (1)


/*
 * 四路传感器权重。
 */
static const int8_t g_grayWeight[4] =
{
     -3, -1, 1, 3
};


/*
 * 最近一次读取到的四路原始电平。
 * 仅低四位有效。
 */
static volatile uint8_t g_rawLevels = 0U;

/*
 * 当前检测到黑线的传感器数量。
 */
static volatile uint8_t g_activeCount = 0U;

/*
 * 当前循迹误差。
 */
static volatile int16_t g_trackError = 0;


/*
 * 根据四路传感器电平计算 error。
 */
static int16_t Track_CalculateError(uint8_t rawLevels)
{
    int16_t weightSum = 0;
    uint8_t count = 0U;
    uint8_t sensorIndex;

    /*
     * 只遍历四路传感器。
     */
    for (sensorIndex = 0U;
         sensorIndex < 4U;
         sensorIndex++)
    {
        uint8_t level;

        /*
         * 取出当前传感器的电平。
         */
        level = (uint8_t)(
            (rawLevels >> sensorIndex) & 0x01U);

        /*
         * 当前电平等于黑线有效电平时，
         * 将对应权重加入计算。
         */
        if (level == GRAY_LINE_LEVEL)
        {
            weightSum += g_grayWeight[sensorIndex];
            count++;
        }
    }

    g_activeCount = count;

    /*
     * 四路均未检测到黑线。
     */
    if (count == 0U)
    {
        return 0;
    }

    /*
     * 计算平均权重。
     */
    return (int16_t)(
        weightSum / (int16_t)count);
}


static void Track_GoStraight(void)
{
    /*
     * 后续添加直行函数。
     */
}


static void Track_TurnLeft(void)
{
    /*
     * 后续添加左转函数。
     */
}


static void Track_TurnRight(void)
{
    /*
     * 后续添加右转函数。
     */
}


static void Track_LineLost(void)
{
    /*
     * 后续可以停车或执行寻线。
     */
}


/*
 * 执行一次循迹。
 */
void Track_Run(void)
{
    /*
     * 读取 X3～X6 四路电平。
     */
    g_rawLevels = Gray_ReadLevels();

    /*
     * 计算 error。
     */
    g_trackError =
        Track_CalculateError(g_rawLevels);

    /*
     * 在 track.c 内完成循迹判断。
     */
    if (g_activeCount == 0U)
    {
        Track_LineLost();
    }
    else if (
        g_trackError < -TRACK_STRAIGHT_RANGE)
    {
        /*
         * 黑线在左侧。
         */
        Track_TurnLeft();
    }
    else if (
        g_trackError > TRACK_STRAIGHT_RANGE)
    {
        /*
         * 黑线在右侧。
         */
        Track_TurnRight();
    }
    else
    {
        /*
         * error 位于 -1～+1。
         */
        Track_GoStraight();
    }
}


uint8_t Track_GetRawLevels(void)
{
    return g_rawLevels;
}


int16_t Track_GetError(void)
{
    return g_trackError;
}