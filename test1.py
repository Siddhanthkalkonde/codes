import cv2
import argparse
import numpy as np
from ultralytics import YOLO
from realsense_depth import *
import supervision as sv
import math
from finalbot_arduino import MotorControl

motor_controller = MotorControl()

num_boxes = 0
point = (320, 240)

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="YOLOv8 live")
    parser.add_argument(
        "--webcam-resolution",
        default=[700, 700],
        nargs=2,
        type=int
    )
    args = parser.parse_args()
    return args

def find_average_rgb(image, detections):
    boxes = detections.xyxy[:, :4].astype(int)
    average_rgb_values = [] 
    
    for box in boxes:
        roi = image[box[1]:box[3], box[0]:box[2]]
        average_rgb = np.mean(roi, axis=(0, 1))
        average_rgb_values.append(average_rgb)

    return np.mean(average_rgb_values, axis=0)

def count_bounding_boxes(detections):
    boxes = detections.xyxy[:, :4].astype(int)
    return int(len(boxes))

def get_color_region(x, width):
    third_width = width // 3
    if 0 <= x < third_width:
        return "Blue"
    elif third_width <= x < 2 * third_width:
        return "Green"
    else:
        return "Red"

def calculate_midpoint(box):
    x1, y1, x2, y2 = box
    return ((x1 + x2) // 2, (y1 + y2) // 2)

def perform_detection(frame, model, box_annotator, depth_frame):
    result = model(frame)[0]
    detections = sv.Detections.from_yolov8(result)

    frame = box_annotator.annotate(
        scene=frame,
        detections=detections,
    )

    average_rgb = find_average_rgb(frame, detections)
    print("Average RGB Values:", average_rgb)

    global num_boxes
    num_boxes = count_bounding_boxes(detections)
    print("Number of Bounding Boxes:", num_boxes)

    height, width, _ = frame.shape
   
    third_width = width // 3

    # Draw Cartesian plane with unit markings
    draw_cartesian_plane(frame)

    for box in detections.xyxy[:, :4].astype(int):
        box_center_x = (box[0] + box[2]) // 2
        color_region = get_color_region(box_center_x, width)
        print(f"Object detected in the {color_region} region")

        # Set the variable based on the color region
        color_code = 0 if color_region == 'Green' else 1 if color_region == 'Blue' else 2

        if color_region == 'Green':
            motor_controller.forward(0.3)
        elif color_region == 'Red':
            motor_controller.stop(0)
            motor_controller.cammotor(color_code)
        elif color_region == 'Blue':
            motor_controller.stop(0)
            motor_controller.cammotor(color_code)

        # Calculate the midpoint of the bounding box
        box_midpoint = calculate_midpoint(box)
        print("Midpoint of bounding box:", box_midpoint)

        angle = calculate_angle(frame , box_midpoint)
        print("Angle = ", angle)

        # Print the quadrant
        quadrant = get_quadrant(box_midpoint)
        print(f"Quadrant: {quadrant}")

        # Draw line from origin to midpoint
        plot_line_from_origin(frame, box_midpoint)

        # Draw circle at the midpoint of the bounding box
        cv2.circle(frame, box_midpoint, 4, (0, 255, 0), -1)

        # Calculate distance from the midpoint using depth information
        distance = depth_frame[box_midpoint[1], box_midpoint[0]]
        
        # Display the distance and angle on the frame
        cv2.putText(frame, "{:.2f}mm".format(distance), (box_midpoint[0], box_midpoint[1] - 20), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 0), 2)
        cv2.putText(frame, "{:.2f} degrees".format(angle), (box_midpoint[0], box_midpoint[1] + 20), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 0), 2)

    # Draw a red circle at the reference point
    cv2.circle(frame, point, 4, (0, 0, 255), -1)

    # Calculate distance from the reference point using depth information
    distance = depth_frame[point[1], point[0]]

    # Display the distance of the reference point on the frame
    cv2.putText(frame, "{}mm".format(distance), (point[0], point[1] - 20), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 0), 2)

    cv2.line(frame, (third_width, 0), (third_width, height), (255, 0, 0), 2)  # Blue
    cv2.line(frame, (2 * third_width, 0), (2 * third_width, height), (0, 255, 0), 2)  # Green

    return frame

def draw_cartesian_plane(frame):
    height, width, _ = frame.shape
    
    # Draw x-axis
    cv2.line(frame, (0, height), (width, height), (0, 255, 0), 1)
    
    # Draw y-axis
    cv2.line(frame, (width // 2, height), (width // 2, 0), (0, 255, 0), 1)
    
    # Draw origin
    origin = (width // 2, height)
    cv2.circle(frame, origin, 3, (0, 0, 255), -1)
    
    # Add text for x and y axes
    cv2.putText(frame, 'X', (width - 20, height + 20), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.putText(frame, 'Y', (width // 2 + 20, 20), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # Marking units on x-axis
    for i in range(width // 2 + 40, width, 40):
        cv2.line(frame, (i, height - 5), (i, height + 5), (0, 255, 0), 1)
        cv2.putText(frame, str(i - width // 2), (i - 10, height + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

    # Marking units on y-axis
    for i in range(40, height, 40):
        cv2.line(frame, (width // 2 - 5, height - i), (width // 2 + 5, height - i), (0, 255, 0), 1)
        cv2.putText(frame, str(i), (width // 2 + 10, height - i + 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

def calculate_angle(frame , midpoint):
    # Calculate the angle between the y-axis and the line from origin to the midpoint
    origin_x, origin_y = frame.shape[1] // 2, frame.shape[0] - 1
    angle_radians = math.atan2(origin_x - midpoint[0], origin_y - midpoint[1])
    angle_degrees = math.degrees(angle_radians)
    return angle_degrees


def plot_line_from_origin(frame, midpoint):
    # Draw a line from origin to the midpoint
    height, width, _ = frame.shape
    origin = (width // 2, height)
    cv2.line(frame, origin, midpoint, (255, 0, 0), 2)

def get_quadrant(midpoint):
    x, y = midpoint
    # Check the x and y coordinates of the midpoint to determine the quadrant
    if x >= 320 and y >= 240:
        return "First Quadrant"
    elif x < 320 and y >= 240:
        return "Second Quadrant"
    elif x < 320 and y < 240:
        return "Third Quadrant"
    else:
        return "Fourth Quadrant"

def detection_output():
    args = parse_arguments()
    frame_width, frame_height = args.webcam_resolution

    dc = DepthCamera()

    model = YOLO("/home/sid/trial/trained_models/ball.pt")
    
    box_annotator = sv.BoxAnnotator(
        thickness=2,
        text_thickness=2,   
        text_scale=1
    )

    while True:
        ret, depth_frame, color_frame = dc.get_frame()

        color_frame = perform_detection(color_frame, model, box_annotator, depth_frame)
        
        cv2.imshow("detections", color_frame)
        # cv2.imshow("depth", depth_frame)

        if (cv2.waitKey(30) == 27):
            break

    cv2.destroyAllWindows()

if __name__ == "__main__":
    detection_output()
