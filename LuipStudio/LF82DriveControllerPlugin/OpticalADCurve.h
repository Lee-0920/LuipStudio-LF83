#ifndef OPTICALADCURVE_H
#define OPTICALADCURVE_H
#include "LuipShare.h"
#include <memory>
#include <vector>


namespace Controller
{

/**
 * @brief 光学信号曲线类
 * @details
 */
class LUIP_SHARE OpticalADCurve
{
public:
    OpticalADCurve(int length);

    float* GetWholeData(void);

    float GetData(int index)const;
    void SetData(int index, float data);
    int GetLength()const;
    int GetSize()const;
    float GetBegin()const;
    float GetEnd()const;

private:
    std::shared_ptr<float> m_data;
    int m_index;
    int m_length;
    float m_begin;
    float m_end;

};

}

#endif // OPTICALADCURVE_H

