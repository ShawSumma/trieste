
#include "../tris.h"
#include <stdio.h>
#include <inttypes.h>

void file_write_tri_table(FILE *out, tri_table_t *table);
void file_write_tri(FILE *out, tri_table_t *table, tri_t world);

#define TRI_NAME "tri-%" PRIu64

void file_write_tri_part(FILE *out, tri_t tri) {
    if (tri.type == TYPE_COLOR) {
        fprintf(out, "%i %i %i rgb", (int) tri.color.r, (int) tri.color.g, (int) tri.color.b);
    } else {
        fprintf(out, TRI_NAME, tri.id);
    }
}

void file_write_tri_table(FILE *out, tri_table_t *table) {
    for (size_t index = 0; index < (size_t) table->prime; index++) {
        tri_t tri = table->tris[index];
        if (tri.type == TYPE_RECURSIVE) {
            fprintf(out, "/" TRI_NAME " {\n", tri.id);
            fprintf(out, "  /" TRI_NAME, tri.id);
            fprintf(out, " ");
            file_write_tri_part(out, tri.children[POS_CENTER]);
            fprintf(out, " ");
            file_write_tri_part(out, tri.children[POS_TOP]);
            fprintf(out, " ");
            file_write_tri_part(out, tri.children[POS_LEFT]);
            fprintf(out, " ");
            file_write_tri_part(out, tri.children[POS_RIGHT]);
            fprintf(out, " tri def\n");
            fprintf(out, "  " TRI_NAME "\n", tri.id);
            fprintf(out, "} def\n\n");
        }
    }
}

void file_write_tri(FILE *out, tri_table_t *table, tri_t world) {
    file_write_tri_table(out, table);
    fprintf(out, "\n");
    fprintf(out, "/on-init " TRI_NAME " def\n", world.id);
}
