$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()
$cmd = $c.CreateCommand()
$cmd.CommandText = "SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'PRODUCTION'"
$rdr = $cmd.ExecuteReader()
while($rdr.Read()){ Write-Output "$($rdr[0]) $($rdr[1])" }
$c.Close()
