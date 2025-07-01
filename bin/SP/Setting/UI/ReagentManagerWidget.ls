setting.ui.reagentManager =
{
    rowCount = 5,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    superRow = 0,
    administratorRow = 0,
    changeReagent = function(name, vol)
        ReagentRemainManager.ChangeReagent(setting.liquidType[name], vol)
    end,
    checkReagentDate = function()
       ReagentRemainManager.CheckAllReagentDate()
    end,
    {
        name = "bacteria",
        text = "发光菌",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    {
        name = "salt",
        text = "渗透压调节液",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    {
        name = "standard",
        text = "质控液",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    {
        name = "blank",
        text = "空白水",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    {
        name = "waste",
        text = "废液桶",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
}

return setting.ui.reagentManager
