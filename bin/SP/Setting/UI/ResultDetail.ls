setting.ui.resultDetail =
{
    showPrivilege=  RoleType.Maintain,
    measure =
    {
        widget = nil,
        data =
        {
            width = 630,
            high = 120,
            xLabel = "Time/min",
            yLabel = "AD",
            editWidth = 65,
            {
                name = "C0",
                text = "参考C0",
                row = 1,
                col = 1,
                width = 80,
            },
            {
                name = "C1",
                text = "参考C1",
                row = 1,
                col = 2,
                width = 80,
            },
            {
                name = "Ct",
                text = "参考Ct",
                row = 1,
                col = 3,
                width = 80,
            },
            {
                name = "gainRef",
                text = "参考增益",
                row = 1,
                col = 4,
                width = 80,
            },
            {
                name = "S0",
                text = "测量S0",
                row = 2,
                col = 1,
                width = 80,
            },
            {
                name = "S1",
                text = "测量S1",
                row = 2,
                col = 2,
                width = 80,
            },
            {
                name = "St",
                text = "测量St",
                row = 2,
                col = 3,
                width = 80,
            },
            {
                name = "gainMea",
                text = "测量增益",
                row = 2,
                col = 4,
                width = 80,
            },
            {
                name = "CF",
                text = "稳定系数CF",
                row = 3,
                col = 1,
                width = 80,
            },
            {
                name = "consistency",
                text = "抑制率(%)",
                row = 3,
                col = 2,
                width = 80,
            },
            --[[{
                name = "RLI",
                text = "相对发光(%)",
                row = 3,
                col = 3,
                width = 80,
            },--]]
        },
        wave =
        {
            width = 740,
            high = 380,
            {
                name = "reference",
                text = "参考",
                acronym = "R",
                color = GlobalColor.blue,
            },
            {
                name = "measure",
                text = "测量",
                acronym = "M",
                color = GlobalColor.red,
            },
        },
        updateRecord = function(record)
            local refCurveIndexStart = 0
            local meaCurveIndexStart = 0
            local C0Start = 0
            local CtStart = 0
            local S0Start = 0
            local StStart = 0
            local IRStart = 0
            local gainRefStart = 0
            local gainMeaStart = 0
            --local RLIStart = 0
            local CFStart = 0
            local size = 0

            local recordFields = setting.fileFormat.measure[setting.fileFormat.measure.current]
            for _,v in ipairs(recordFields) do
                if v.name == "consistency" then
                    IRStart = size
                elseif v.name == "CF" then
                    CFStart = size
                elseif v.name == "C0" then
                    C0Start = size
                elseif v.name == "Ct" then
                    CtStart = size
                elseif v.name == "S0" then
                    S0Start = size
                elseif v.name == "St" then
                    StStart = size
                elseif v.name == "gainRef" then
                    gainRefStart = size
                elseif v.name == "gainMea" then
                    gainMeaStart = size
                --elseif v.name == "RLI" then
                    --RLIStart = size
                elseif v.name == "refCurveIndex" then
                    refCurveIndexStart = size
                elseif v.name == "meaCurveIndex" then
                    meaCurveIndexStart = size
                end
                size = size + v.size
            end

            local _, IR = record:GetFloat(IRStart)
            local _, CF = record:GetFloat(CFStart)
            local _, C0 = record:GetInt(C0Start)
            local _, Ct = record:GetInt(CtStart)
            local _, S0 = record:GetInt(S0Start)
            local _, St = record:GetInt(StStart)
            local _, gainRef = record:GetFloat(gainRefStart)
            local _, gainMea = record:GetFloat(gainMeaStart)
            --local _, RLI = record:GetFloat(RLIStart)
            local _, refCurveIndex = record:GetInt(refCurveIndexStart)
            local _, meaCurveIndex = record:GetInt(meaCurveIndexStart)

            local curveManager = LF82CurveManager.Instance()
            local refCurve = curveManager:GetCurve(refCurveIndex)
            local meaCurve = curveManager:GetCurve(meaCurveIndex)
            local C1 = math.floor(refCurve:GetData(1))
            local S1 = math.floor(meaCurve:GetData(1))

            --print("C0 = "..C0..",Ct = "..Ct..",S0 = "..S0..",St = "..St..",IR = "..IR..",RLI = "..RLI..",CF = "..CF..",refCurveIndex = "..refCurveIndex..",meaCurveIndex = "..meaCurveIndex)
            setting.ui.resultDetail.measure.widget:ResetScaleValue()
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("consistency", string.format("%.2f", IR))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("CF", string.format("%.2f", CF))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("C0", tostring(C0))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("Ct", tostring(Ct))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("S0", tostring(S0))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("St", tostring(St))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("C1", tostring(C1))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("S1", tostring(S1))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("gainRef", string.format("%.0f", gainRef))
            setting.ui.resultDetail.measure.widget:UpdateDataInfo("gainMea", string.format("%.0f", gainMea))
            --setting.ui.resultDetail.measure.widget:UpdateDataInfo("RLI", string.format("%.2f", RLI))
            setting.ui.resultDetail.measure.widget:UpdateWaveInfo("reference", tostring(refCurveIndex))
            setting.ui.resultDetail.measure.widget:UpdateWaveInfo("measure", tostring(meaCurveIndex))
        end,
    },
}

return setting.ui.resultDetail
