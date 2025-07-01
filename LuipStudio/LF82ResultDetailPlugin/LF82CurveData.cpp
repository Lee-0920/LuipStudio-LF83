/**
 * @file
 * @brief 曲线数据
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */
#include "LF82CurveData.h"
#include "vector"
#include <QDebug>
#include "Log.h"
using namespace std;

namespace ResultDetail
{

/**
 * @brief 驱动控制器构造。
 */
LF82CurveData::LF82CurveData(int length):
    m_index(0),m_maxData(0),m_minData(0),
    m_length(length)
{
    if (m_length > 0)
    {
        m_data.reset(new float[length], std::default_delete<float[]>());
        memset(m_data.get(), 0, sizeof(float)*length);
    }
}

float* LF82CurveData::GetWholeData(void)
{
    return m_data.get();
}


bool LF82CurveData::AddData(float* buffer, int length)
{
    if(m_length - m_index >= length)
    {
        memcpy(&m_data.get()[m_index], buffer, sizeof(float)*length);
        m_index += length;
        return true;
    }
    else
    {
        return false;
    }
}

float LF82CurveData::GetData(int index)const
{
    return m_data.get()[index];
}

void LF82CurveData::SetData(int index, float data)
{
    if(index < 0 || index >= m_length)
    {
        return;
    }
    this->m_data.get()[index] = data;
}

int LF82CurveData::GetLength()const
{
    return this->m_length;
}

int LF82CurveData::GetSize()const
{
    return this->m_length * sizeof(float);
}


float LF82CurveData::GetMaxData()
{
    if(m_length > 0)
    {
        m_maxData = this->GetData(0);
        for(int i = 0;i < m_length; i++)
        {
            if(m_data.get()[i] > m_maxData)
            {
                m_maxData = this->GetData(i);
            }
        }
    }
    return m_maxData;
}

float LF82CurveData::GetMinData()
{
    if(m_length > 0)
    {
        m_minData = this->GetData(0);
        for(int i = 0;i < m_length; i++)
        {
            if(m_data.get()[i] < m_minData)
            {
                m_minData = this->GetData(i);
            }
        }
    }
    return m_minData;
}


}
