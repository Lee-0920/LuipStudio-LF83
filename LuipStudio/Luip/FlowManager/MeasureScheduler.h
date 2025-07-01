/**
 * @file
 * @brief 测量排期调度器。
 * @details 
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */


#if !defined(MEASURE_MEASURESCHEDULER_H_)
#define MEASURE_MEASURESCHEDULER_H_
#include <memory>
#include <list>
#include <vector>
#include <QString>
#include <QTimer>
#include <QMutex>
#include <QDateTime>
#include "lua.hpp"
#include "System/Types.h"
#include "LuaEngine/LuaEngine.h"
#include "IScheduleNotifiable.h"
#include "IFlowNotifiable.h"

using std::list;
using std::vector;
using System::Uint16;
using System::Uint32;
using System::String;

namespace Flow
{

/**
 * @brief 当前流程信息。
 * @details
 */
struct CurrFlow
{
    String name;
    Uint32 startTime;
    Uint32 runTime;
    Uint32 endTime;
};

/**
 * @brief UI开启自动排期项信息。
 * @details
 */
class SchedulerItem
{
public:
    SchedulerItem()
        :name(""),
         text(""),
         interval(0),
         priority(0),
         lastTime(-1),
         nextTime(-1),
         isRetry(false),
         isTimedMeasure(false)
    {

    }
    ~SchedulerItem() {}

public:
    String name;
    String text;
    float interval;
    int priority;
    long lastTime;
    long nextTime;
    bool isRetry;
    bool isTimedMeasure;
    OOLUA::Lua_func_ref calculateNextTime;
    OOLUA::Lua_func_ref createFlow;
};


/**
 * @brief 测量排期调度器。
 * @details
 */
class MeasureScheduler  : public QObject , public IFlowNotifiable
{
    Q_OBJECT
public:
    static MeasureScheduler *Instance();
    bool Init();
    void Register(IScheduleNotifiable *handle);
    void Deregister(IScheduleNotifiable *handle);
    void Update();

    void SyncSysTimeToSchedule(QDateTime oldDateTime);

    void OnFlowActivated(String name, long time);
    void OnFlowFinished(String name, String result);
    void OnFlowThreadExit();

    ~MeasureScheduler();

signals:
    void InitSignal();
    void UpdateSignal();
    void OnFlowActivatedSignal(QString name, long time);
    void OnFlowFinishedSignal(QString name, QString result);
    void OnFlowThreadExitSignal();
    void StartTimerSignal(int times);
    void StopTimerSignal();
public slots:
    void InitSlot();
    void UpdateSlot();
    void TimeoutSlot();
    void OnFlowActivatedSlot(QString name, long time);
    void OnFlowFinishedSlot(QString name, QString result);
    void OnFlowThreadExitSlot();
    void StartTimerSlot(int times);
    void StopTimerSlot();

private:
    MeasureScheduler();

    void UpdateItemList();
    void UpdateQueue();
    void UpdateReadyItem();
    void UpdateExpectInfo(long nextTime, String text);

    void RestReadyItem();
    bool NextTimeAnalysis(CurrFlow currFlow, vector<SchedulerItem> &queue);
    bool PriorityAnalysis(long refTime, vector<SchedulerItem> &queue);
    bool RetyAnalysis(vector<SchedulerItem> &queue);

private:
   list<IScheduleNotifiable*> m_notifise;
   list<SchedulerItem> m_itemList;
   vector<SchedulerItem> m_queue;

   CurrFlow m_currFlow;
   QTimer *m_timer;
   SchedulerItem m_readyItem;

   QMutex m_updateMutex;
   QThread *m_scheduleThread;

   lua_State * m_newState;

   static std::unique_ptr<MeasureScheduler> m_instance;
};

}

#endif  //MEASURE_MEASURESCHEDULER_H_
