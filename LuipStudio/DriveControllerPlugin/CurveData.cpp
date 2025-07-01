/**
 * @file
 * @brief 曲线数据
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */
#include "CurveData.h"
#include "vector"
#include <QDebug>
#include "Log.h"
using namespace std;

namespace Controller
{

/**
 * @brief 驱动控制器构造。
 */
CurveData::CurveData(float beginVoltage, float endVoltage, int length):
    m_index(0),
    m_length(length),
    m_begin(beginVoltage),
    m_end(endVoltage)
{
    if (m_length > 0)
    {
        m_data.reset(new float[length], std::default_delete<float[]>());
        memset(m_data.get(), 0, sizeof(float)*length);
    }
}

float* CurveData::GetWholeData(void)
{
    return m_data.get();
}


bool CurveData::AddData(float* buffer, int length)
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

float CurveData::GetData(int index)const
{
    return m_data.get()[index];
}

void CurveData::SetData(int index, float data)
{
    if(index < 0 || index >= m_length)
    {
        return;
    }
    this->m_data.get()[index] = data;
}

int CurveData::GetLength()const
{
    return this->m_length;
}

int CurveData::GetSize()const
{
    return this->m_length * sizeof(float);
}

float CurveData::GetBegin()const
{

    return this->m_begin;
}

float CurveData::GetEnd()const
{
    return this->m_end;
}

/**
 * @brief 曲线拟合法计算电流峰面积
 * @return 电流峰面积，double。
 */
float CurveData::CalculateArea(float beginVoltage, float endVoltage, float windowFrom, float windowTo)
{
    float peakVoltage = 0;
    float peakCurrent = -10000;
    float baseCurrent = 0;
    float area = 0;
    const int length = m_length;
    float xVoltage[length];
    float yCurrent[length];
    memcpy(&yCurrent[0], &m_data.get()[0], sizeof(float)*length);

    if(length <= 0)
    {
        return area;
    }

    for(int i = 0; i < length; i++)
    {
        xVoltage[i] = beginVoltage + i*(endVoltage - beginVoltage)/(length - 1);
    }

    // 计算起点值
    int nStartPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowFrom)
        {
            nStartPoint = i;
            break;
        }
    }

    // 计算终点值
    int nStopPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowTo)
        {
            nStopPoint = i;
            break;
        }
    }

    // 起点和终点间数据点数必须大于等于10个
    if(nStopPoint - nStartPoint + 1 < 10)
    {
        return area;
    }

    // 获取线性拟合的数据
    float fxVoltage[] =
    {
        xVoltage[nStartPoint],
        xVoltage[nStartPoint+1],
        xVoltage[nStartPoint+2],
        xVoltage[nStartPoint+3],
        xVoltage[nStartPoint+4],
        xVoltage[nStopPoint-4],
        xVoltage[nStopPoint-3],
        xVoltage[nStopPoint-2],
        xVoltage[nStopPoint-1],
        xVoltage[nStopPoint]
    };

    float fyCurrent[] =
    {
        yCurrent[nStartPoint],
        yCurrent[nStartPoint+1],
        yCurrent[nStartPoint+2],
        yCurrent[nStartPoint+3],
        yCurrent[nStartPoint+4],
        yCurrent[nStopPoint-4],
        yCurrent[nStopPoint-3],
        yCurrent[nStopPoint-2],
        yCurrent[nStopPoint-1],
        yCurrent[nStopPoint]
    };

    // 求平均值
    float fMeanX = 0;
    float fMeanY = 0;
    float fMeanX2 = 0;
    float fMeanXY = 0;
    for(int i = 0; i < 10; i++)
    {
        fMeanX += fxVoltage[i];
        fMeanY += fyCurrent[i];
        fMeanX2 += fxVoltage[i]*fxVoltage[i];
        fMeanXY += fxVoltage[i]*fyCurrent[i];
    }
    fMeanX = fMeanX/10;
    fMeanY = fMeanY/10;
    fMeanX2 = fMeanX2/10;
    fMeanXY = fMeanXY/10;

    //求斜率
    if(fabs(fMeanX2-fMeanX*fMeanX) < 1.0e-7)
    {
        return false;
    }

    float k = (fMeanXY-fMeanX*fMeanY)/(fMeanX2-fMeanX*fMeanX);

    //求截距
    float b = fMeanY - k*fMeanX;

    // 计算峰值和面积
    for(int i = nStartPoint; i <= nStopPoint; i++)
    {
        baseCurrent = k*xVoltage[i] + b;
        if((yCurrent[i] - baseCurrent) > peakCurrent)
        {
            peakCurrent = yCurrent[i] - baseCurrent;
            peakVoltage = xVoltage[i];
        }

        if(yCurrent[i] - baseCurrent > 0 && i < nStopPoint)
        {
            area += (yCurrent[i] - baseCurrent)*(xVoltage[i+1] - xVoltage[i]);
        }
    }

    return area;
}

/**
 * @brief 曲线拟合法计算电流峰值
 * @return 峰值电流，double。
 */
float CurveData::CalculatePeak(float beginVoltage, float endVoltage, float windowFrom, float windowTo)
{
    float peakVoltage = 0;
    float peakCurrent = -10000;
    float baseCurrent = 0;
    const int length = m_length;
    float xVoltage[length];
    float yCurrent[length];
    memcpy(&yCurrent[0], &m_data.get()[0], sizeof(float)*length);

    if(length <= 0)
    {
        return peakCurrent;
    }

    for(int i = 0; i < length; i++)
    {
        xVoltage[i] = beginVoltage + i*(endVoltage - beginVoltage)/(length - 1);
    }

    // 计算起点值
    int nStartPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowFrom)
        {
            nStartPoint = i;
            break;
        }
    }

    // 计算终点值
    int nStopPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowTo)
        {
            nStopPoint = i;
            break;
        }
    }

    // 起点和终点间数据点数必须大于等于10个
    if(nStopPoint - nStartPoint + 1 < 10)
    {
        return peakCurrent;
    }

    // 获取线性拟合的数据
    float fxVoltage[] =
    {
        xVoltage[nStartPoint],
        xVoltage[nStartPoint+1],
        xVoltage[nStartPoint+2],
        xVoltage[nStartPoint+3],
        xVoltage[nStartPoint+4],
        xVoltage[nStopPoint-4],
        xVoltage[nStopPoint-3],
        xVoltage[nStopPoint-2],
        xVoltage[nStopPoint-1],
        xVoltage[nStopPoint]
    };

    float fyCurrent[] =
    {
        yCurrent[nStartPoint],
        yCurrent[nStartPoint+1],
        yCurrent[nStartPoint+2],
        yCurrent[nStartPoint+3],
        yCurrent[nStartPoint+4],
        yCurrent[nStopPoint-4],
        yCurrent[nStopPoint-3],
        yCurrent[nStopPoint-2],
        yCurrent[nStopPoint-1],
        yCurrent[nStopPoint]
    };

    // 求平均值
    float fMeanX = 0;
    float fMeanY = 0;
    float fMeanX2 = 0;
    float fMeanXY = 0;
    for(int i = 0; i < 10; i++)
    {
        fMeanX += fxVoltage[i];
        fMeanY += fyCurrent[i];
        fMeanX2 += fxVoltage[i]*fxVoltage[i];
        fMeanXY += fxVoltage[i]*fyCurrent[i];
    }
    fMeanX = fMeanX/10;
    fMeanY = fMeanY/10;
    fMeanX2 = fMeanX2/10;
    fMeanXY = fMeanXY/10;

    //求斜率
    if(fabs(fMeanX2-fMeanX*fMeanX) < 1.0e-7)
    {
        return false;
    }

    float k = (fMeanXY-fMeanX*fMeanY)/(fMeanX2-fMeanX*fMeanX);

    //求截距
    float b = fMeanY - k*fMeanX;

    // 计算峰值
    for(int i = nStartPoint; i <= nStopPoint; i++)
    {
        baseCurrent = k*xVoltage[i] + b;
        if((yCurrent[i] - baseCurrent) > peakCurrent)
        {
            peakCurrent = yCurrent[i] - baseCurrent;
            peakVoltage = xVoltage[i];
        }

        if(peakCurrent < 0)
        {
            peakCurrent = 0;
        }
    }

    return peakCurrent;
}

/**
 * @brief 曲线拟合法计算电流峰值对应电压
 * @return 电压，double。
 */
float CurveData::GetPeakVolt(float beginVoltage, float endVoltage, float windowFrom, float windowTo)
{
    float peakVoltage = 0;
    float peakCurrent = -10000;
    float baseCurrent = 0;
    const int length = m_length;
    float xVoltage[length];
    float yCurrent[length];
    memcpy(&yCurrent[0], &m_data.get()[0], sizeof(float)*length);

    if(length <= 0)
    {
        return peakVoltage;
    }

    for(int i = 0; i < length; i++)
    {
        xVoltage[i] = beginVoltage + i*(endVoltage - beginVoltage)/(length - 1);
    }

    // 计算起点值
    int nStartPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowFrom)
        {
            nStartPoint = i;
            break;
        }
    }

    // 计算终点值
    int nStopPoint = 0;
    for(int i = 0; i < length; i++)
    {
        if(xVoltage[i] >= windowTo)
        {
            nStopPoint = i;
            break;
        }
    }

    // 起点和终点间数据点数必须大于等于10个
    if(nStopPoint - nStartPoint + 1 < 10)
    {
        return peakVoltage;
    }

    // 获取线性拟合的数据
    float fxVoltage[] =
    {
        xVoltage[nStartPoint],
        xVoltage[nStartPoint+1],
        xVoltage[nStartPoint+2],
        xVoltage[nStartPoint+3],
        xVoltage[nStartPoint+4],
        xVoltage[nStopPoint-4],
        xVoltage[nStopPoint-3],
        xVoltage[nStopPoint-2],
        xVoltage[nStopPoint-1],
        xVoltage[nStopPoint]
    };

    float fyCurrent[] =
    {
        yCurrent[nStartPoint],
        yCurrent[nStartPoint+1],
        yCurrent[nStartPoint+2],
        yCurrent[nStartPoint+3],
        yCurrent[nStartPoint+4],
        yCurrent[nStopPoint-4],
        yCurrent[nStopPoint-3],
        yCurrent[nStopPoint-2],
        yCurrent[nStopPoint-1],
        yCurrent[nStopPoint]
    };

    // 求平均值
    float fMeanX = 0;
    float fMeanY = 0;
    float fMeanX2 = 0;
    float fMeanXY = 0;
    for(int i = 0; i < 10; i++)
    {
        fMeanX += fxVoltage[i];
        fMeanY += fyCurrent[i];
        fMeanX2 += fxVoltage[i]*fxVoltage[i];
        fMeanXY += fxVoltage[i]*fyCurrent[i];
    }
    fMeanX = fMeanX/10;
    fMeanY = fMeanY/10;
    fMeanX2 = fMeanX2/10;
    fMeanXY = fMeanXY/10;

    //求斜率
    if(fabs(fMeanX2-fMeanX*fMeanX) < 1.0e-7)
    {
        return false;
    }

    float k = (fMeanXY-fMeanX*fMeanY)/(fMeanX2-fMeanX*fMeanX);

    //求截距
    float b = fMeanY - k*fMeanX;

    // 计算峰值
    for(int i = nStartPoint; i <= nStopPoint; i++)
    {
        baseCurrent = k*xVoltage[i] + b;
        if((yCurrent[i] - baseCurrent) > peakCurrent)
        {
            peakCurrent = yCurrent[i] - baseCurrent;
            peakVoltage = xVoltage[i];
        }

        if(peakCurrent < 0)
        {
            peakCurrent = 0;
        }
    }

    return peakVoltage;
}


}
