#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

#define AMOUNT 20
#define LENGTH 10
#define SEGMENT(start, length, child) (Segment){start, {0.0f, 0.0f}, length, 0.0f, 0.0f, child}

typedef struct Segment {
    Vector2 start;
    Vector2 end;    // "private"
    size_t length;
    double absolute_angle;   // absolue angle. inherits from parent
    double angle;            // relative angle. unique & disconnected
    struct Segment *child;
} Segment;

void follow(Segment *segment, Vector2 target) {
    Vector2 direction = Vector2Subtract(target, segment->start);

    segment->angle = atan2(direction.y, direction.x);

    direction = Vector2Normalize(direction);
    direction = Vector2Scale(direction, segment->length);
    direction = Vector2Scale(direction, -1);
    segment->start = Vector2Add(target, direction);
}

void calculate_end(Segment *segment) {
    double theta = segment->angle + segment->absolute_angle;
    Vector2 delta_line = {
        segment->length * cos(theta),
        segment->length * sin(theta),
    };
    segment->end = Vector2Add(segment->start, delta_line);
    // segment->angle = theta;
}

void update_segment(Segment *segment) {
    if(segment->child != NULL) {
        follow(segment, segment->child->start);
    } else {
        follow(segment, GetMousePosition());
    }

    calculate_end(segment);
}

void draw_segments(Segment *segment, size_t amount) {
    for(size_t i = 0; i < amount; i++) {
        DrawLineEx(segment[i].start, segment[i].end, 5.0, RAYWHITE);
    }
}

int main(void) {
    InitWindow(800, 600, "kinematics");
    SetTargetFPS(60);

    Segment segments[AMOUNT];
    segments[0] = SEGMENT(Vector2Zero(), LENGTH, NULL);
    for(size_t i = 1; i < AMOUNT; i++)
        segments[i] = SEGMENT(Vector2Zero(), LENGTH, &segments[i-1]);

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
