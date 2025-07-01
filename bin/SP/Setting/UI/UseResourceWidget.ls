setting.ui.useResourceWidget =
{
    rowCount = 2,
    superRow = 0,
    administratorRow = 0,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    {
        name = "pumpPipe",
        text = "泵管接头",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    {
        name = "PTFEPipe",
        text = "PTFE硬管",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    checkMaterialLife = function()
       MaterialLifeManager.CheckAllMaterialLife()
    end,
    changeMaterialLife = function(name)
        MaterialLifeManager.Reset(setting.materialType[name])
    end,
}

return setting.ui.useResourceWidget
