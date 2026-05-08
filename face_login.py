from deepface import DeepFace
import cv2
import sys
import os
import time  # ← zid hatha

def main():
    if len(sys.argv) < 2:
        print("NON")
        sys.stdout.flush()
        return

    photo_reference = sys.argv[1]

    if not os.path.exists(photo_reference):
        print("NO_FILE")
        sys.stdout.flush()
        return

    # Capturer photo depuis webcam
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("NO_CAM")
        sys.stdout.flush()
        return

    time.sleep(1.5)  # ← zid hatha (webcam ttstabilise)

    ret, frame = cap.read()
    cap.release()

    if not ret:
        print("NO_CAM")
        sys.stdout.flush()
        return

    # Sauvegarder photo temporaire
    temp_path = "temp_capture.jpg"
    cv2.imwrite(temp_path, frame)

    # Comparer avec la photo référence
    try:
        result = DeepFace.verify(
            img1_path=photo_reference,
            img2_path=temp_path,
            enforce_detection=True,
            silent=True
        )
        if result["verified"]:
            print("OK")
        else:
            print("NON")
    except Exception as e:
        print("NO_FACE")
    finally:
        if os.path.exists(temp_path):
            os.remove(temp_path)

    sys.stdout.flush()

main()