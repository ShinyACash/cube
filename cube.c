#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>  // use <unistd.h> if on a unix based system. i don really care about a mac.

#define WIDTH 40
#define HEIGHT 20
#define NUM_EDGES 12

typedef struct {
    float x, y, z;
} Point3D;

typedef struct {
    int p[4];
} Face;

void normalize(Point3D* p) {
    float length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
    p->x /= length;
    p->y /= length;
    p->z /= length;
}

float dot_product(Point3D a, Point3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void rotate_x(Point3D* p, float angle) {
    float y_new = p->y * cos(angle) - p->z * sin(angle);
    float z_new = p->y * sin(angle) + p->z * cos(angle);
    p->y = y_new;
    p->z = z_new;
}

void rotate_y(Point3D* p, float angle) {
    float x_new = p->x * cos(angle) + p->z * sin(angle);
    float z_new = -p->x * sin(angle) + p->z * cos(angle);
    p->x = x_new;
    p->z = z_new;
}

void project(Point3D p, int* x, int* y) {
    *x = (int)(p.x + WIDTH / 2);
    *y = (int)(p.y + HEIGHT / 2);
}

void draw(Point3D* cube, Face* faces, int num_faces) {
    char screen[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j] = ' ';
        }
    }

    Point3D light_dir = { 1.0f, 1.0f, 1.0f };
    normalize(&light_dir);

    char shaders[] = { '@', '#', '*', '%', '!', '.' };

    for (int i = 0; i < num_faces; i++) {
        Face face = faces[i];

        
        Point3D v1 = cube[face.p[1]];
        Point3D v2 = cube[face.p[2]];
        Point3D v3 = cube[face.p[3]];

        Point3D edge1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
        Point3D edge2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

        Point3D normal = {
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        };

        normalize(&normal);  //I miss Unity vector3d :((

        float shade_factor = dot_product(normal, light_dir);
        if (shade_factor < 0) shade_factor = 0;  

        int intensity = (int)(shade_factor * 5);  
        char shade = shaders[intensity];

        for (int j = 0; j < 4; j++) {
            int p1 = face.p[j];
            int p2 = face.p[(j + 1) % 4];

            Point3D p1_proj = cube[p1];
            Point3D p2_proj = cube[p2];

            int x1, y1, x2, y2;
            project(p1_proj, &x1, &y1);
            project(p2_proj, &x2, &y2);

            int dx = abs(x2 - x1), dy = abs(y2 - y1), sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1; 
            int err = dx - dy;
            while (1) {
                if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
                    screen[y1][x1] = shade;
                }
                if (x1 == x2 && y1 == y2) break;
                int e2 = err * 2;
                if (e2 > -dy) { err -= dy; x1 += sx; }
                if (e2 < dx) { err += dx; y1 += sy; }
            }
        }
    }

    for (int i = 0; i < num_faces; i++) {
        Face face = faces[i];

        for (int j = 0; j < 4; j++) {
            int p1 = face.p[j];
            int p2 = face.p[(j + 1) % 4];

            Point3D p1_proj = cube[p1];
            Point3D p2_proj = cube[p2];

            int x1, y1, x2, y2;
            project(p1_proj, &x1, &y1);
            project(p2_proj, &x2, &y2);

            if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT)
                screen[y1][x1] = '#';  
            if (x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT)
                screen[y2][x2] = '#'; 
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    float scale = 5.0f;  

    Point3D cube[8] = {
        {-scale, -scale, -scale},
        { scale, -scale, -scale},
        { scale,  scale, -scale},
        {-scale,  scale, -scale},
        {-scale, -scale,  scale},
        { scale, -scale,  scale},
        { scale,  scale,  scale},
        {-scale,  scale,  scale}
    };

    Face faces[6] = {
        {{0, 1, 2, 3}},  
        {{4, 5, 6, 7}},  
        {{0, 1, 5, 4}},  
        {{1, 2, 6, 5}},  
        {{2, 3, 7, 6}},  
        {{3, 0, 4, 7}}   
    };

    float angle_x = 0.05, angle_y = 0.03;  
    while (1) {
        for (int i = 0; i < 8; i++) {
            rotate_x(&cube[i], angle_x);
            rotate_y(&cube[i], angle_y);
        }

        system("cls");
        draw(cube, faces, 6);
        Sleep(10);
    }

    return 0;
}
