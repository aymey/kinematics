#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct Segment {
    Vector2 start;
    Vector2 end;
    size_t length;
    int angle;
    struct Segment *parent;
} Segment;

void draw_segment(Segment *segment) {
    const Vector2 origin = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};

    if(segment->parent != NULL)
        segment->start = segment->parent->end;

    segment->end = (Vector2){
        segment->length * cos(segment->angle),
        segment->length * sin(segment->angle),
    };
    segment->end = Vector2Add(segment->start, segment->end);

    Vector2 start = Vector2Add(segment->start, origin);
    Vector2 end = Vector2Add(segment->end, origin);

    DrawLineEx(start, end, 5.0, RAYWHITE);

    // printf("start: %f, %f\nend: %f, %f\nlength: %ld\n angle: %d\n\n\n",
    //         start.x, start.y, end.x, end.y, segment->length, segment->angle);
}

void draw_segments(Segment *segment, size_t amount) {
    for(size_t i = 0; i < amount; i++) {
        draw_segment(&segment[i]);
    }
}

int main(void) {
    InitWindow(800, 600, "kinematics");
    SetTargetFPS(60);

    Segment a = { {0.0f, 0.0f}, {0, 0}, 100, 0, NULL};
    Segment b = { {0.0f, 0.0f}, {0, 0}, 100, 0, &a};
    Segment segments[2] = { a, b };

    while(!WindowShouldClose()) {
        BeginDrawing();
            draw_segments(segments, 2);
            ClearBackground(GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
