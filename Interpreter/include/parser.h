#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "tree.h"

Vertex *parse(TokenStream* stream);

/* Expressions */
static Vertex *E();
static Vertex *Ew();

/* Tuple Expressions */
static Vertex *T();
static Vertex *Ta();
static Vertex *Tc();

/* Boolean Expressions */
static Vertex *B();
static Vertex *Bt();
static Vertex *Bs();
static Vertex *Bp();

/* Arithmetic Expressions */
static Vertex *A();
static Vertex *At();
static Vertex *Af();
static Vertex *Ap();

/* Rators And Rands */
static Vertex *R();
static Vertex *Rn();

/* Definitions */
static Vertex *D();
static Vertex *Da();
static Vertex *Dr();
static Vertex *Db();

/* Variables */
static Vertex *Vb();
static Vertex *Vl();
