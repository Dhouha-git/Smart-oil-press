$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()
$cmd = $c.CreateCommand()
$cmd.CommandText = "SELECT search_condition FROM user_constraints WHERE constraint_name = 'CHK_TYPE_HUILE_PROD'"
$rdr = $cmd.ExecuteReader()
while($rdr.Read()){ Write-Output "$($rdr[0])" }
$c.Close()
