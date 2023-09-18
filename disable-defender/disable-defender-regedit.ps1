$regpath='HKLM:\SYSTEM\CurrentControlSet\Services'
Set-ItemProperty -Path ($regpath+"\WinDefend") -Name Start -Value 4
Set-ItemProperty -Path ($regpath+"\Sense") -Name Start -Value 4
Set-ItemProperty -Path ($regpath+"\WdFilter") -Name Start -Value 4
Set-ItemProperty -Path ($regpath+"\WdNisDrv") -Name Start -Value 4
Set-ItemProperty -Path ($regpath+"\WdNisSvc") -Name Start -Value 4
Set-ItemProperty -Path ($regpath+"\WdBoot") -Name Start -Value 4