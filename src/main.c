#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

#define AMOUNT 20
#define SEGMENT(start, length, child) (Segment){start, {0.0f, 0.0f}, length, 0.0f, 0.0f, child}

typedef struct Segment {
    Vector2 start;
    Vector2 end;    // "private"
    size_t length;
    float absolute_angle;   // absolue angle. inherits from parent
    float angle;            // relative angle. unique & disconnected
    struct Segment *child;
} Segment;

void update_segment(Segment *segment) {
    segment->angle += 0.001*sin(GetTime());

    float theta = segment->angle + segment->absolute_angle;
    Vector2 delta_line = {
        segment->length * cos(theta),
        segment->length * sin(theta),
    };
    segment->end = Vector2Add(segment->start, delta_line);

    if(segment->child != NULL) {
        segment->child->absolute_angle = theta;
        segment->child->start = segment->end;
    }
}

void draw_segments(Segment *segment, size_t amount) {
    for(size_t i = 0; i < amount; i++) {
        DrawLineEx(segment[i].start, segment[i].end, 5.0, RAYWHITE);
    }
}

int main(void) {
    InitWindow(800, 600, "kinematics");
    SetTargetFPS(60);

    const Vector2 origin = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};

    Segment segments[AMOUNT];
    segments[0] = SEGMENT(origin, 50, NULL);
    for(size_t i = 1; i < AMOUNT; i++) {
        segments[i] = SEGMENT(origin, 50, &segments[i-1]);

    while(!WindowShouldClose()) {
        for(size_t i = 0; i < AMOUNT; i++)
            update_segment(segments + i);

        BeginDrawing();
            ClearBackground(GRAY);
            draw_segments(segments, AMOUNT);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
