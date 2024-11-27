import math
import os
import time

def rotate_x(point, angle):
    x, y, z = point
    cos_a, sin_a = math.cos(angle), math.sin(angle)
    return x, y * cos_a - z * sin_a, y * sin_a + z * cos_a

def rotate_y(point, angle):
    x, y, z = point
    cos_a, sin_a = math.cos(angle), math.sin(angle)
    return x * cos_a + z * sin_a, y, -x * sin_a + z * cos_a

def rotate_z(point, angle):
    x, y, z = point
    cos_a, sin_a = math.cos(angle), math.sin(angle)
    return x * cos_a - y * sin_a, x * sin_a + y * cos_a, z

scale = 5
vertices = [
    (-1 * scale, -1 * scale, -1 * scale),
    (-1 * scale, -1 * scale,  1 * scale),
    (-1 * scale,  1 * scale, -1 * scale),
    (-1 * scale,  1 * scale,  1 * scale),
    ( 1 * scale, -1 * scale, -1 * scale),
    ( 1 * scale, -1 * scale,  1 * scale),
    ( 1 * scale,  1 * scale, -1 * scale),
    ( 1 * scale,  1 * scale,  1 * scale)
]

edges = [
    (0, 1), (1, 3), (3, 2), (2, 0),
    (4, 5), (5, 7), (7, 6), (6, 4),
    (0, 4), (1, 5), (2, 6), (3, 7)
]

shading = ".,-~:;=!*#$@"

angle_x = angle_y = angle_z = 0

# So much easier with python bruh.

while True:
    os.system('cls' if os.name == 'nt' else 'clear') 
    transformed_vertices = []
    
    for v in vertices:
        rotated = rotate_x(v, angle_x)
        rotated = rotate_y(rotated, angle_y)
        rotated = rotate_z(rotated, angle_z)
        transformed_vertices.append(rotated)
    
    projected = [
        (int(40 + x * 15 / (z + 10)), int(12 + y * 10 / (z + 10)), z)
        for x, y, z in transformed_vertices
    ]
    
    screen = [[' ' for _ in range(80)] for _ in range(24)]
    zbuffer = [[float('-inf') for _ in range(80)] for _ in range(24)]
    for edge in edges:
        x1, y1, z1 = projected[edge[0]]
        x2, y2, z2 = projected[edge[1]]

        for t in range(101):
            x = int(x1 + t * (x2 - x1) / 100)
            y = int(y1 + t * (y2 - y1) / 100)
            z = z1 + t * (z2 - z1) / 100
            if 0 <= x < 80 and 0 <= y < 24:
                if z > zbuffer[y][x]:  
                    zbuffer[y][x] = z
                    shade_index = int((z + 10) / 20 * (len(shading) - 1))
                    screen[y][x] = shading[max(0, min(shade_index, len(shading) - 1))]
    

    for row in screen:
        print(''.join(row))
    

    angle_x += 0.04
    angle_y += 0.08
    angle_z += 0.02
    time.sleep(0.03)

    