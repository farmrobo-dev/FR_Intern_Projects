from ultralytics import YOLO
import cv2
import serial
import time

def scale_to_angle(value, min_input, max_input, min_angle=0, max_angle=180):
    """Scale a value from a given range to a servo motor angle range."""
    return int(((value - min_input) / (max_input - min_input)) * (max_angle - min_angle) + min_angle)

# serial connection
try:
    ser = serial.Serial('COM3', 9600, timeout=2) 
    print("Serial connection established.")
except serial.SerialException as e:
    print("Error opening serial port:", e)
    exit(1)

# yolo model
model = YOLO(r"C:\\Users\\sai kiran\\OneDrive\\Desktop\\face_detection\\yolov11n-face.pt")

# camera resolution
ws, hs = 1280, 720
cap = cv2.VideoCapture(0)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, ws)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, hs)

if not cap.isOpened():
    print("Error: Unable to access the camera.")
    exit(1)

servo_position = 90  
previous_face_x = None
movement_threshold = 10
face_not_found_counter = 0  

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Unable to read frame from the camera.")
            break

        # yolo face detection
        results = model.predict(source=frame, save=False, conf=0.5)
        max_confidence = 0
        best_face = None

        for result in results[0].boxes.data:
            x1, y1, x2, y2, conf, cls = result[:6]

            # Focus only on faces with confidence >= 0.80
            if conf >= 0.80 and conf > max_confidence:
                max_confidence = conf
                best_face = (x1, y1, x2, y2, conf)

        if best_face:
            face_not_found_counter = 0  # Reset counter since a face is found
            x1, y1, x2, y2, conf = best_face
            label = f"Confidence: {conf:.2f}"

           
            cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
            cv2.putText(frame, label, (int(x1), int(y1) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

            
            face_x_center = (x1 + x2) / 2

           
            if previous_face_x is not None:
                displacement = abs(face_x_center - previous_face_x)
                if displacement > movement_threshold:  
                    if face_x_center < previous_face_x:  
                        servo_position += 5  
                    elif face_x_center > previous_face_x:  
                        servo_position -= 5  

                    
                    servo_position = max(0, min(180, servo_position))

                    
                    ser.write(f"{servo_position}\n".encode())
                    time.sleep(0.05)

            previous_face_x = face_x_center  
        else:
            face_not_found_counter += 1

            
            if face_not_found_counter > 10:  
                for angle in range(0, 181, 5):  # from 0 to 180 degrees
                    ser.write(f"{angle}\n".encode())
                    time.sleep(0.05)

                    #camera active 
                    ret, frame = cap.read()
                    if not ret:
                        break

                    results = model.predict(source=frame, save=False, conf=0.5)
                    for result in results[0].boxes.data:
                        x1, y1, x2, y2, conf, cls = result[:6]
                        if conf >= 0.80:
                            face_not_found_counter = 0  
                            best_face = (x1, y1, x2, y2, conf)
                            servo_position = angle  #update servo to current angle 
                            ser.write(f"{servo_position}\n".encode())
                            break

                    if best_face:
                        break

                if not best_face:  
                    for angle in range(180, -1, -5):  
                        ser.write(f"{angle}\n".encode())
                        time.sleep(0.05)

                        
                        ret, frame = cap.read()
                        if not ret:
                            break

                        results = model.predict(source=frame, save=False, conf=0.5)
                        for result in results[0].boxes.data:
                            x1, y1, x2, y2, conf, cls = result[:6]
                            if conf >= 0.80:
                                face_not_found_counter = 0  
                                best_face = (x1, y1, x2, y2, conf)
                                servo_position = angle  
                                ser.write(f"{servo_position}\n".encode())
                                break

                        if best_face:
                            break

    
        cv2.imshow("Live Face Tracking", frame)

        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except Exception as e:
    print("An error occurred:", e)
finally:
    cap.release()
    ser.close()
    cv2.destroyAllWindows()
