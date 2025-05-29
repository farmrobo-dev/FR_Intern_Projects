from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive
import os
import time
import sys

# Configuration
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
WATCH_FOLDER = os.path.join(BASE_DIR, "UPLOAD")
CHECK_INTERVAL = 3  # seconds
CLIENT_SECRETS = os.path.join(BASE_DIR, "client_secrets.json")

def authenticate_drive():
    if not os.path.exists(CLIENT_SECRETS):
        print(f"ERROR: {CLIENT_SECRETS} not found in {BASE_DIR}. Please add it and restart.")
        sys.exit(1)
    gauth = GoogleAuth()
    try:
        gauth.LocalWebserverAuth()
    except Exception as e:
        print(f"Google authentication failed: {e}")
        sys.exit(1)
    return GoogleDrive(gauth)

drive = authenticate_drive()

def file_exists_on_drive(filename):
    file_list = drive.ListFile({
        'q': f"title='{filename}' and trashed=false"
    }).GetList()
    return len(file_list) > 0

def upload_to_drive(filepath):
    filename = os.path.basename(filepath)
    file_drive = drive.CreateFile({'title': filename})
    file_drive.SetContentFile(filepath)
    try:
        file_drive.Upload()
        return True
    except Exception as e:
        print(f"Failed to upload {filename}: {e}")
        return False

def monitor_and_upload():
    print(f"Monitoring folder: {WATCH_FOLDER}")
    while True:
        try:
            files = [
                f for f in os.listdir(WATCH_FOLDER)
                if os.path.isfile(os.path.join(WATCH_FOLDER, f))
            ]
            for filename in files:
                filepath = os.path.join(WATCH_FOLDER, filename)
                if file_exists_on_drive(filename):
                    print(f"{filename} already exists on Google Drive. Deleting local copy.")
                    os.remove(filepath)
                else:
                    print(f"Uploading {filename} to Google Drive...")
                    if upload_to_drive(filepath):
                        print(f"Uploaded {filename} successfully.")
                        os.remove(filepath)
                    else:
                        print(f"Failed to upload {filename}. Will retry later.")
        except Exception as e:
            print(f"Error: {e}")
        time.sleep(CHECK_INTERVAL)

if __name__ == "__main__":
    os.makedirs(WATCH_FOLDER, exist_ok=True)
    print("Press Ctrl+C to stop the script.")
    try:
        monitor_and_upload()
    except KeyboardInterrupt:
        print("\nStopped by user.")