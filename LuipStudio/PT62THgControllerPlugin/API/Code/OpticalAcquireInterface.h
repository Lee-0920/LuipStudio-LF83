/**
 * @page page_OpticalAcquireInterface 光学信号采集接口
 *  光学信号采集接口提供了光学信号采集相关操作。
 *
 *  具体命令见： @ref module_OpticalAcquireInterface
 *
 * @section sec_OAI_ChangeLog 版本变更历史
 *  接口历史变更记录：
 *  - 1.0.0 基本版本 (2016.4.28)
 *
 */

/**
 * @addtogroup module_OpticalAcquireInterface 光学信号采集接口
 * @{
 */

/**
 * @file
 * @brief 光学信号采集接口。
 * @details 定义了一序列光学信号采集相关的操作。
 * @version 1.1.0
 * @author xiejinqiang
 * @date 2016.09.02
 */

#ifndef DSCP_OPTICAL_ACQUIRE_INTERFACE_H_
#define DSCP_OPTICAL_ACQUIRE_INTERFACE_H_

#define DSCP_OAI_CBASE                  0x0000 + 0x0C00     ///< 命令基值
#define DSCP_OAI_EBASE                  0x8000 + 0x0C00     ///< 事件基值
#define DSCP_OAI_SBASE                  0x0000 + 0x0C00     ///< 状态基值


// *******************************************************************
// 命令和回应

/**
 * @brief 设置信号AD上报周期。
 * @details 系统将根据设定的周期，定时向上发出信号AD上报事件。
 * @param period Float32，信号AD上报周期，单位为秒。0表示不需要上报，默认为0。
 * @see DSCP_EVENT_OAI_SIGNAL_AD_NOTICE
 * @note 所设置的上报周期将在下一次启动时丢失，默认为0，不上报。而且通信失败时不上报。
 */
#define DSCP_CMD_OAI_SET_SIGNAL_AD_NOTIFY_PERIOD    (DSCP_OAI_CBASE + 0x00)

/**
 * @brief 启动采集过程。
 * @details 启动后，将采样时间内得到的数据将以事件 @ref DSCP_EVENT_EAI_AD_ACQUIRED
 *  的形式上传给上位机，按照设置的采样周期进行采样，采样结束即上传数据，无需对数据进行处理。
 * @param samplePeriod Float32，采样周期。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_START_ACQUIRER                 (DSCP_OAI_CBASE + 0x01)

/**
 * @brief 停止采集过程。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_STOP_ACQUIRER                  (DSCP_OAI_CBASE + 0x02)

/**
 * @brief 打开汞灯
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_TURN_ON_MERCURY_LAMP           (DSCP_OAI_CBASE + 0x03)

/**
 * @brief 关闭汞灯
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_TURN_OFF_MERCURY_LAMP          (DSCP_OAI_CBASE + 0x04)


/** * @brief 打开LED
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_TURN_ON_LED                    (DSCP_OAI_CBASE + 0x05)

/**
 * @brief 关闭LED
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
#define DSCP_CMD_OAI_TURN_OFF_LED                   (DSCP_OAI_CBASE + 0x06)


// *******************************************************************
// 事件

/**
 * @brief 信号AD采集完成事件。
 * @details 启动采集后，按照设置的采样周期采完数据时将产生该事件。
 * @param number Uint32，采样时间内得到的数据个数，决定了后面的变长参数长度。
 * @param value[number][2] Uint32，采样时间内得到的AD值。value格式如下：
 *      - value[number][0] reference Uint32，参考端AD。
 *      - value[number][1] measure Uint32，测量端AD。
 * @see DSCP_CMD_OAI_START_ACQUIRER
 */
#define DSCP_EVENT_OAI_AD_ACQUIRED                  (DSCP_OAI_EBASE + 0x00)

/**
 * @brief 信号AD定时上报事件。
 * @details 系统将根据设置的上报周期，定时向上汇报信号AD。
 *  上报周期可通过命令 @ref DSCP_CMD_TCI_SET_TEMPERATURE_NOTIFY_PERIOD 设定。
 * @param reference Uint32，参考端AD。
 * @param measure Uint32，测量端AD。
 */
#define DSCP_EVENT_OAI_SIGNAL_AD_NOTICE               (DSCP_OAI_EBASE + 0x01)

// *******************************************************************
// 状态返回




#endif // DSCP_OPTICAL_ACQUIRE_INTERFACE_H_

/** @} */
