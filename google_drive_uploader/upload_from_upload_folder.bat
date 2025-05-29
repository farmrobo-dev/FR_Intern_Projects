@echo off
REM filepath: c:\FR_GOOGLEDRIVE\FR_Intern_Projects\FR_GOOGLEDRIVE\upload_from_upload_folder.bat

REM Set the full path to client_secrets.json here
set CLIENT_SECRETS=C:\FR_GOOGLEDRIVE\FR_Intern_Projects\FR_GOOGLEDRIVE\client_secrets.json

cd /d "%~dp0"
python upload_to_drive.py "%CLIENT_SECRETS%"
pause