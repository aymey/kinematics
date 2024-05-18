#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct Segment {
    Vector2 start;
    Vector2 end;    // "private"
    size_t length;
    float angle;
    struct Segment *child;
} Segment;

void update_segment(Segment *segment) {
    segment->angle += 0.01;

    Vector2 delta_line = {
        segment->length * cos(segment->angle),
        segment->length * sin(segment->angle),
    };
    segment->end = Vector2Add(segment->start, delta_line);

    if(segment->child != NULL)
        segment->child->start = segment->end;
}

void draw_segments(Segment *segment, size_t amount) {
    for(size_t i = 0; i < amount; i++) {
        DrawLineEx(segment[i].start, segment[i].end, 5.0, segment[i].child != NULL ? RAYWHITE : GREEN);
    }
}

int main(void) {
    InitWindow(800, 600, "kinematics");
    SetTargetFPS(60);

    const Vector2 origin = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};

    Segment b = { {0.0, 0.0}, {0, 0}, 100, 1.4, NULL};
    Segment a = { origin, {0, 0}, 100, 0, &b};
    Segment segments[2] = { a, b };
    segments[0].child = &segments[1];

    while(!WindowShouldClose()) {
        for(size_t i = 0; i < 2; i++)
            update_segment(segments + i);

        BeginDrawing();
            ClearBackground(GRAY);
            draw_segments(segments, 2);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
