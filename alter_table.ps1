$c = New-Object System.Data.Odbc.OdbcConnection('Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;')
$c.Open()

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "UPDATE PRODUCTION SET ID_EMP = 1 WHERE ID_EMP IS NULL"
    $cmd.ExecuteNonQuery()
} catch {}

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "ALTER TABLE PRODUCTION MODIFY ID_EMP NOT NULL"
    $cmd.ExecuteNonQuery()
} catch {}

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "ALTER TABLE PRODUCTION MODIFY TYPE_HUILE NULL"
    $cmd.ExecuteNonQuery()
} catch {}

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "ALTER TABLE PRODUCTION ADD TEMPERATURE_MOYENNE NUMBER(5,2) CHECK (TEMPERATURE_MOYENNE > 0)"
    $cmd.ExecuteNonQuery()
} catch { Write-Output "TEMPERATURE_MOYENNE catch: $_" }

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "ALTER TABLE PRODUCTION ADD DUREE_PRESSAGE NUMBER CHECK (DUREE_PRESSAGE > 0)"
    $cmd.ExecuteNonQuery()
} catch { Write-Output "DUREE_PRESSAGE catch: $_" }

try {
    $cmd = $c.CreateCommand()
    $cmd.CommandText = "ALTER TABLE PRODUCTION ADD OBSERVATION VARCHAR2(255)"
    $cmd.ExecuteNonQuery()
} catch { Write-Output "OBSERVATION catch: $_" }

Write-Output "Done"
$c.Close()
