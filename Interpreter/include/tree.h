#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"

void AST(Vertex* vertex);
void ST(Vertex* vertex);

void depth_first_left_to_right_traversal(Vertex *vertex, size_t depth);

Vertex *create_vertex(size_t type, Token *token);

void add_left_child(Vertex *parent, Vertex *left_child);
Vertex *get_left_child(Vertex *parent);

void add_right_sibling(Vertex *vertex, Vertex *right_sibling);
Vertex *get_right_sibling(Vertex *vertex);

static Vertex *clone_vertex(Vertex *vertex);
static Vertex *clone_subtree(Vertex *root);

Vertex *standardize(Vertex *vertex);
