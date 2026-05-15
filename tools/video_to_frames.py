import cv2
import os

def video_to_frames(video_path, output_folder, frame_interval=1):
    os.makedirs(output_folder, exist_ok=True)
    
    cap = cv2.VideoCapture(video_path)
    
    if not cap.isOpened():
        print(f"Error: Cannot open video file {video_path}")
        return
    
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    
    print(f"Video info:")
    print(f"  Total frames: {total_frames}")
    print(f"  FPS: {fps}")
    print(f"  Duration: {total_frames / fps:.2f} seconds")
    print(f"  Output folder: {output_folder}")
    print(f"  Frame interval: {frame_interval}")
    
    frame_count = 0
    saved_count = 0
    
    while True:
        ret, frame = cap.read()
        
        if not ret:
            break
        
        if frame_count % frame_interval == 0:
            output_path = os.path.join(output_folder, f"frame_{saved_count:06d}.jpg")
            cv2.imwrite(output_path, frame)
            saved_count += 1
            
            if saved_count % 100 == 0:
                print(f"Processed {saved_count} frames...")
        
        frame_count += 1
    
    cap.release()
    
    print(f"\nCompleted!")
    print(f"  Total frames processed: {frame_count}")
    print(f"  Frames saved: {saved_count}")

if __name__ == "__main__":
    video_path = os.path.join(os.path.dirname(__file__), "Video_20260124164522602.avi")
    output_folder = os.path.join(os.path.dirname(__file__), "video_frames")
    
    video_to_frames(video_path, output_folder, frame_interval=1)
