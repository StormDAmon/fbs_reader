cd %~dp0
.\flatc\flatc.exe --grpc --cpp test_fbs.fbs
.\..\fbs_reader\x64\Release\fbs_reader.exe test_fbs.fbs

pause