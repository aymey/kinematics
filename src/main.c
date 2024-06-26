#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

#define AMOUNT 5
#define LENGTH 100
#define ORIGIN (Vector2){GetScreenWidth()/2.0, GetScreenHeight()/2.0}
#define SEGMENT(start, length, child) (Segment){start, {0.0f, 0.0f}, length, 0.0f, child}

typedef struct Segment {
    Vector2 start;
    Vector2 end;
    size_t length;
    double angle;
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
    double theta = segment->angle;
    Vector2 delta_line = {
        segment->length * cos(theta),
        segment->length * sin(theta),
    };
    segment->end = Vector2Add(segment->start, delta_line);
}

void force_start(Segment *segment, Vector2 start) {
    segment->start = start;
    calculate_end(segment);
}

void update_segment(Segment *segment) {
    if(segment->child != NULL)
        follow(segment, segment->child->start);
    else
        follow(segment, GetMousePosition());

    calculate_end(segment);
}

void draw_segments(Segment *segments, size_t amount) {
    force_start(&segments[amount-1], ORIGIN);

    for(size_t i = amount-2; i > 0; i--)
        force_start(&segments[i], segments[i+1].end);

    for(size_t i = 0; i < amount; i++)
        DrawLineEx(segments[i].start, segments[i].end, 5.0, RAYWHITE);
}

int main(void) {
    InitWindow(800, 600, "kinematics");
    SetTargetFPS(60);

    Segment segments[AMOUNT];
    segments[0] = SEGMENT(ORIGIN, LENGTH, NULL);
    for(size_t i = 1; i < AMOUNT; i++)
        segments[i] = SEGMENT(ORIGIN, LENGTH, &segments[i-1]);

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
