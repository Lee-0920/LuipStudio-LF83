/**
 * @file
 * @brief 曲线数据
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */
#include "OpticalADCurve.h"
#include "vector"
#include "Log.h"
#include <cstring>

using namespace std;

namespace Controller
{

OpticalADCurve::OpticalADCurve(int length):
    m_index(0),
    m_length(length),
    m_begin(0),
    m_end(0)
{
    if (m_length > 0)
    {
        m_data.reset(new float[length], std::default_delete<float[]>());
        memset(m_data.get(), 0, sizeof(float)*length);
    }
}

float* OpticalADCurve::GetWholeData(void)
{
    return m_data.get();
}


float OpticalADCurve::GetData(int index)const
{
    return m_data.get()[index];
}

void OpticalADCurve::SetData(int index, float data)
{
    if(index < 0 || index >= m_length)
    {
        return;
    }
    this->m_data.get()[index] = data;
}

int OpticalADCurve::GetLength()const
{
    return this->m_length;
}

int OpticalADCurve::GetSize()const
{
    return this->m_length * sizeof(float);
}

float OpticalADCurve::GetBegin()const
{

    return this->m_begin;
}

float OpticalADCurve::GetEnd()const
{
    return this->m_end;
}

}
