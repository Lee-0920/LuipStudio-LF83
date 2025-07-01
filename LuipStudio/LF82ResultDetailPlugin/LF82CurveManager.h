#ifndef LF82CURVEMANAGER_H
#define LF82CURVEMANAGER_H

#include "LF82CurveFile.h"
#include <memory>

namespace  ResultDetail
{

class LF82CurveManager
{
public:
    LF82CurveManager();
    ~LF82CurveManager();
    static LF82CurveManager* Instance();
    void Init();
    System::Uint32 AddCurve(LF82CurveData curve);
    LF82CurveData GetCurve(System::Uint32 index);
    LF82CurveFile* GetCurveFile();

private:
    static std::unique_ptr<LF82CurveManager> m_instance;

    LF82CurveFile *m_curveFile;
};

}
#endif // LF82CURVEMANAGER_H
