config.scheduler =
{
    measure =		-- 测量排期
    {
        mode = _G.MeasureMode.Trigger,	-- 自动测量模式
        interval = 2,		-- 自动测量间隔周期
        timedPoint =		--
        {
            true, true, true, true, true,
            true, true, true, true, true,
            true, true, true, true, true,
            true, true, true, true, true,
            true, true, true, true,
        },
    },
    calibrate =		-- 定标排期
    {
        enable = false,		-- 是否开着
        interval= 168,		-- 间隔周期
    },
    clean =			-- 清洗排期
    {
        enable = false,		--
        interval= 168,		--
    },
    check =			-- 质控排期
    {
        enable = false,		--
        interval= 168,		--
    },
    blank =			-- 空白水排期
    {
        enable = false,		--
        interval= 168,		--
    },
}
