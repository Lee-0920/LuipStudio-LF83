/**
 * @file
 * @brief 驱动控制器。
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */


#if !defined(CONTROLLER_DRIVECONTROLNET_H_)
#define CONTROLLER_DRIVECONTROLNET_H_

#include <vector>
#include <QObject>
#include "LuipShare.h"
#include "ControllerPlugin/BaseController.h"
#include "API/PeristalticPumpInterface.h"
#include "API/SolenoidValveInterface.h"
#include "API/OpticalMeterInterface.h"
#include "API/TemperatureControlInterface.h"
#include "API/OpticalAcquireInterface.h"
#include "API/MotorControlInterface.h"
#include "OpticalADCurve.h"

using std::list;

namespace Controller
{

/**
 * @brief 驱动控制器。
 * @details
 */
class LUIP_SHARE LF82DriveController : public QObject, public BaseController
{
    Q_OBJECT

public:
    LF82DriveController(DscpAddress addr);
    virtual ~LF82DriveController();
    bool Init();
    bool Uninit();

    PeristalticPumpInterface* GetIPeristalticPump();
    SolenoidValveInterface* GetISolenoidValve();
    OpticalMeterInterface* GetIOpticalMeter();
    TemperatureControlInterface*  GetITemperatureControl();
    OpticalAcquireInterface* GetIOpticalAcquire();
    MotorControlInterface* GetIMotorControl();

    float GetDigestTemperature() const;
    float GetEnvironmentTemperature() const;

    float GetEnvironmentSignalTemp() const;
    float GetThermostatSignalTemp(Uint8 index) const;

    float GetCurrEnvironmentTemp();
    float GetCurrThermostatTemp(Uint8 index);

    void ClearThermostatRemainEvent();

    bool GetOpticalADVector(int length);
    OpticalADCurve* GetRefADCurve(int length) const;
    OpticalADCurve* GetMeaADCurve(int length) const;

    // ---------------------------------------
    // IEventReceivable 接口
    void Register(ISignalNotifiable *handle);
    virtual void OnReceive(DscpEventPtr event);
    void StartSignalUpload();
    void StopSignalUpload();


private:
  void EnvironmentTemperatureMonitor(float temp);

public:
    // 设备接口集
    PeristalticPumpInterface * const IPeristalticPump;
    SolenoidValveInterface * const ISolenoidValve;
    OpticalMeterInterface * const IOpticalMeter;
    TemperatureControlInterface * const ITemperatureControl;
    OpticalAcquireInterface * const IOpticalAcquire;
    MotorControlInterface* const IMotorControl;

private:
    list<ISignalNotifiable*> m_notifise;
    //Temperature m_temperature;
    //Temperature m_extraTemperature;

    vector<float> m_ThermostatTemps;
    vector<OpticalAD> m_opticalAD;

    float m_environmentTemp;
    int m_sendEnvTempCount;

};

}

#endif  //CONTROLLER_DRIVECONTROLNET_H_

