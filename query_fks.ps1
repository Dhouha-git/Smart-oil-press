$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()
$cmd = $c.CreateCommand()
$cmd.CommandText = "SELECT constraint_name FROM user_constraints WHERE table_name = 'PRODUCTION' AND constraint_type = 'R'"
$rdr = $cmd.ExecuteReader()
while($rdr.Read()){ Write-Output "$($rdr[0])" }
$c.Close()
