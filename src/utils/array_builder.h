#pragma once

#include <stdlib.h>

#define ARRAY_BUILDER_INIT_CAPACITY 10

#define ARRAY_BUILDER_STRUCT_DECLARE(type) \
    struct { \
        type* data; \
        size_t size; \
        size_t capacity; \
    }

#define ARRAY_BUILDER_INIT(builder) { \
    (builder).data = malloc((ARRAY_BUILDER_INIT_CAPACITY + 1) * sizeof(*(builder).data)); \
    (builder).size = 0; \
    (builder).capacity = ARRAY_BUILDER_INIT_CAPACITY; \
    if ((builder).data) (builder).data[0] = 0; \
}

#define ARRAY_BUILDER(name, type) \
    ARRAY_BUILDER_STRUCT_DECLARE(type) name; \
    ARRAY_BUILDER_INIT(name)

#define ARRAY_BUILDER_PUSH(builder, value) { \
    if ((builder).size == (builder).capacity) { \
        (builder).capacity *= 2; \
        (builder).data = realloc((builder).data, ((builder).capacity + 1) * sizeof(*(builder).data)); \
    } \
    (builder).data[(builder).size++] = value; \
    (builder).data[(builder).size] = 0; /* Maintain null termination */ \
}

#define ARRAY_BUILDER_FINALIZE(builder) ({ \
    typeof((builder).data) final_array = realloc((builder).data, ((builder).size + 1) * sizeof(*(builder).data)); \
    (builder).data = NULL; \
    final_array; \
})

#define ARRAY_BUILDER_FREE(builder) { \
    free((builder).data); \
    (builder).data = NULL; \
    (builder).size = 0; \
    (builder).capacity = 0; \
}
