#include "gray.h"
#include "ti_msp_dl_config.h"

/*
 * 通道切换以后等待 OUT 电平稳定。
 * 当前设置为约 50 us。
 */
#define GRAY_SETTLE_TIME_US    (50U)

#define GRAY_SETTLE_CYCLES     \
    ((CPUCLK_FREQ / 1000000U) * GRAY_SETTLE_TIME_US)

/*
 * AD0、AD1、AD2 三个地址引脚。
 */
#define GRAY_ADDRESS_PINS      \
    (GRAY_AD0_PIN | GRAY_AD1_PIN | GRAY_AD2_PIN)


/*
 * 根据 channel 设置 AD2、AD1、AD0。
 *
 * channel = 0：选择 X1
 * channel = 1：选择 X2
 * channel = 2：选择 X3
 * ...
 * channel = 7：选择 X8
 */
static void Gray_SelectChannel(uint8_t channel)
{
    uint32_t pinValue = 0U;

    /*
     * channel 的 bit0 控制 AD0。
     */
    if ((channel & 0x01U) != 0U)
    {
        pinValue |= GRAY_AD0_PIN;
    }

    /*
     * channel 的 bit1 控制 AD1。
     */
    if ((channel & 0x02U) != 0U)
    {
        pinValue |= GRAY_AD1_PIN;
    }

    /*
     * channel 的 bit2 控制 AD2。
     */
    if ((channel & 0x04U) != 0U)
    {
        pinValue |= GRAY_AD2_PIN;
    }

    /*
     * 同时设置三个地址引脚。
     */
    DL_GPIO_writePinsVal(
        GRAY_PORT,
        GRAY_ADDRESS_PINS,
        pinValue);
}


/*
 * 只读取中间四路：X3、X4、X5、X6。
 */
uint8_t Gray_ReadLevels(void)
{
    uint8_t sensorIndex;
    uint8_t channel;
    uint8_t levels = 0U;

    /*
     * sensorIndex：
     *
     * 0 → X3
     * 1 → X4
     * 2 → X5
     * 3 → X6
     */
    for (sensorIndex = 0U;
         sensorIndex < 4U;
         sensorIndex++)
    {
        /*
         * X3 对应地址 channel=2，
         * 因此实际地址为 sensorIndex + 2。
         */
        channel = (uint8_t)(sensorIndex + 2U);

        Gray_SelectChannel(channel);

        /*
         * 等待 OUT 电平稳定。
         */
        delay_cycles(GRAY_SETTLE_CYCLES);

        /*
         * OUT 为高电平时，将结果对应位置 1。
         */
        if (DL_GPIO_readPins(
                GRAY_PORT,
                GRAY_OUT_PIN) != 0U)
        {
            levels |=
                (uint8_t)(1U << sensorIndex);
        }
    }

    return levels;
}