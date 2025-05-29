# Google Drive Upload Automation

This project automatically uploads files from a local `UPLOAD` folder to your Google Drive using PyDrive.  
After a file is uploaded, it is deleted from the local folder.

---

## Features

- **Automatic upload:** Any file placed in the `UPLOAD` folder is uploaded to your Google Drive.
- **No duplicates:** If a file already exists on Drive, it is not uploaded again.
- **Local cleanup:** Files are deleted from the local `UPLOAD` folder after successful upload.
- **Batch file launcher:** Start the process with a double-click on a batch file.

---

## Folder Structure

```
FR_Intern_Projects/
└── FR_GOOGLEDRIVE/
    ├── upload_from_upload_folder.bat
    ├── upload_to_drive.py
    ├── .gitignore
    └── UPLOAD/
        └── (your files to upload)
```

---

## Setup Instructions

### 1. Clone the Repository

```sh
git clone <your-repo-url>
cd FR_Intern_Projects/FR_GOOGLEDRIVE
```

### 2. Install Dependencies

Make sure you have Python installed.  
Install PyDrive:

```sh
pip install pydrive
```

### 3. Google API Credentials

1. Go to [Google Cloud Console](https://console.cloud.google.com/).
2. Create a project and enable the Google Drive API.
3. Create OAuth client credentials (Desktop app).
4. Download the `client_secrets.json` file.
5. **Place `client_secrets.json` in the `FR_GOOGLEDRIVE` folder.**

6. https://console.cloud.google.com/auth/audience
7. later visit this add add the test user gmail address and now the testing workflow will get passed 

> **Note:**  
> `client_secrets.json` is ignored by git for security.  
> Do **not** share or commit this file.

### 4. Usage

1. Place files you want to upload in the `UPLOAD` folder.
2. Double-click `upload_from_upload_folder.bat` to start the uploader.
3. The script will:
    - Authenticate with Google (browser window will open the first time)
    - Monitor the `UPLOAD` folder
    - Upload new files to your Google Drive
    - Delete files from `UPLOAD` after successful upload

---

## Batch File

**upload_from_upload_folder.bat**
```bat
@echo off
REM Always use client_secrets.json in the same folder as this script
set CLIENT_SECRETS=%~dp0client_secrets.json

cd /d "%~dp0"
python upload_to_drive.py "%CLIENT_SECRETS%"
pause
```

---

## Python Script

**upload_to_drive.py**  
(See this repo for the full script.)

---

## Security

- Your `client_secrets.json` is **never** pushed to the repository.
- The `.gitignore` file ensures this.

---

## Troubleshooting

- If authentication fails, ensure your `client_secrets.json` is valid and in the correct folder.
- If you see permission errors, check your Google API settings and OAuth consent screen.

---

## License

This project is for internal use.  
Do not share your Google API credentials.

---

**Enjoy automated uploads to Google Drive!**
