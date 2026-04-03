$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()

$cmd = $c.CreateCommand()
$cmd.CommandText = "SELECT constraint_name FROM user_constraints WHERE table_name = 'PRODUCTION' AND constraint_type = 'C'"
$rdr = $cmd.ExecuteReader()
$constraints = @()
while($rdr.Read()){ $constraints += $rdr[0] }
$rdr.Close()

foreach ($cons in $constraints) {
    if ($cons -ne 'PRODUCTION_PK' -and $cons -notmatch 'NOT NULL' ) {
        try {
            $cmd = $c.CreateCommand()
            $cmd.CommandText = "ALTER TABLE PRODUCTION DISABLE CONSTRAINT $cons"
            $cmd.ExecuteNonQuery()
            Write-Output "Disabled $cons"
        } catch {
            Write-Output "Failed to disable $cons"
        }
    }
}
$c.Close()
