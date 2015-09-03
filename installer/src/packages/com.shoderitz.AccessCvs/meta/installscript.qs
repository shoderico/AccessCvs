
function Component()
{
    // remove unnecessary pages
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
}

// here we are creating the operation chain which will be processed at the real installation part later
Component.prototype.createOperations = function()
{
    // call default implementation to actually install the files
    component.createOperations();

    component.addOperation(
        "Execute",
        "regsvr32 /s    " + '"' + "@TargetDir@\\AccessCvs.dll" + '"',
        "UNDOEXECUTE",
        "regsvr32 /s /u " + '"' + "@TargetDir@\\AccessCvs.dll" + '"'
        );
}
