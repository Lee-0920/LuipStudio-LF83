#include "LF82CurveManager.h"
#include <QApplication>
#include "LF82CurveData.h"

using namespace std;
using namespace System;
namespace  ResultDetail
{


unique_ptr<LF82CurveManager> LF82CurveManager::m_instance(nullptr);


LF82CurveManager::LF82CurveManager()
{

}

LF82CurveManager::~LF82CurveManager()
{

}

LF82CurveManager* LF82CurveManager::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new LF82CurveManager);
    }

    return m_instance.get();
}


void LF82CurveManager::Init()
{
    m_curveFile = new LF82CurveFile(QApplication::applicationDirPath().toStdString() + "/../LuipData/CurveFile.bin", 1024,
                                Controller::API::Version("1.0.0.0"), "CurveFile", ((60 * 1024 * 1024) / 1024));
    if (!m_curveFile->Load())
    {
        m_curveFile->ClearCurve();
    }
}

LF82CurveFile* LF82CurveManager::GetCurveFile()
{
    return m_curveFile;
}

Uint32 LF82CurveManager::AddCurve(LF82CurveData curve)
{
    return m_curveFile->AddCurve(curve);
}

LF82CurveData LF82CurveManager::GetCurve(Uint32 index)
{
    return m_curveFile->GetCurve(index);
}
}
