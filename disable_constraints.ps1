$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()

$cmd1 = $c.CreateCommand()
$cmd1.CommandText = "ALTER TABLE PRODUCTION DISABLE CONSTRAINT FK_PRODUCTION_AGRI"
$cmd1.ExecuteNonQuery()

$cmd2 = $c.CreateCommand()
$cmd2.CommandText = "ALTER TABLE PRODUCTION DISABLE CONSTRAINT FK_PRODUCTION_EMP"
$cmd2.ExecuteNonQuery()

$cmd3 = $c.CreateCommand()
$cmd3.CommandText = "ALTER TABLE PRODUCTION DISABLE CONSTRAINT CHK_TYPE_HUILE_PROD"
$cmd3.ExecuteNonQuery()

$c.Close()
Write-Output "Constraints disabled successfully."
