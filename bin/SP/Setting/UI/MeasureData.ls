setting.ui = {}
setting.ui.measureData =
{
    columnCount = 6,
    resultTypeList =
    {
        "全部",
        "水样",
        "质控样",
        "空白水",
        "核查",
    },
    resultMarkList =
    {
        "全部",
        "正常",
        "离线",
        "维护",
        "故障",
        "校准",
        "超标",
    },
    {
        name = "dateTime",
        text = "时间",
        format = "yyyy-MM-dd hh:mm:ss",
        width = 164,
    },
    {
        name = "consistency",
        text = "抑制率(%)",
        format = "%.1f",
        width = 76,--88
    },
    {
        name = "RLI",
        text = "相对发光度(%)",
        format = "%.1f",
        width = 94,--105
    },
    {
        name = "CF",
        text = "稳定系数",
        format = "%.2f",
        width = 76,--95
    },
    {
        name = "mode",
        text = "标识",
        width = 68,--90
    },
    {
        name = "resultType",
        text = "类型",
        width = 70,--88
    },
    --unitChange = function(value, changeType) return ArguUnitChange(value, changeType) end,
}
