Get-ScheduledTask "Windows Defender Cache Maintenance" | Disable-ScheduledTask
Get-ScheduledTask "Windows Defender Cleanup" | Disable-ScheduledTask
Get-ScheduledTask "Windows Defender Scheduled Scan" | Disable-ScheduledTask
Get-ScheduledTask "Windows Defender Verification" | Disable-ScheduledTask