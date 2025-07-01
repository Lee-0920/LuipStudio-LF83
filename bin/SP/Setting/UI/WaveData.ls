setting.ui.waveData =
{
    {
        content = "consistency",
        title = "抑制率(%)",
        yUpLimit = 100, 
        yLowLimit = -100,
        width = 612,
        height = 165,
        {
            name = "standard",
            text = "质控样",
            color = GlobalColor.red,
        },
        {
            name = "blank",
            text = "空白水",
            color = GlobalColor.green,
        },
        {
            name = "sample",
            text = "水样",
            color = GlobalColor.blue,
        },
    },
    {
        content = "C0",
        title = "初始参考发光度",          
        yUpLimit = 20000000,
        yLowLimit = 0, 
        width = 612,
        height = 165,
        {
            name = "standard",
            text = "质控样",
            color = GlobalColor.red,
        },
        {
            name = "blank",
            text = "空白水",
            color = GlobalColor.green,
        },
        {
            name = "sample",
            text = "水样",
            color = GlobalColor.blue,
        },
    },
    --[[{
        content = "CF", 
        title = "稳定系数",          
        yUpLimit = 2,     
        yLowLimit = 0,
        width = 612,
        height = 165,
        {
            name = "standard",
            text = "质控样",
            color = GlobalColor.red,
        },
        {
            name = "blank",
            text = "空白水",
            color = GlobalColor.green,
        },
        {
            name = "sample",
            text = "水样",
            color = GlobalColor.blue,
        },
    },--]]
    --unitChange = function(value, changeType) return ArguUnitChange(value, changeType) end,
}
