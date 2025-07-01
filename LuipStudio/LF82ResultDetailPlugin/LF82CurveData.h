#ifndef LF82CURVEDATA_H
#define LF82CURVEDATA_H

#include <vector>
#include <QObject>
#include "LuipShare.h"
#include <memory>

using std::list;

namespace ResultDetail
{

/**
 * @brief 曲线数据。
 * @details
 */
class LUIP_SHARE LF82CurveData
{

public:
    LF82CurveData(int length);
    float* GetWholeData(void);

    bool AddData(float* buffer, int length);
    float GetData(int index)const;
    void SetData(int index, float data);
    int GetLength()const;
    int GetSize()const;
    float GetMaxData();
    float GetMinData();

private:
    std::shared_ptr<float> m_data;
    int m_index;
    int m_length;

    float m_maxData;
    float m_minData;
};

}

#endif  //LF82CURVEDATA_H

