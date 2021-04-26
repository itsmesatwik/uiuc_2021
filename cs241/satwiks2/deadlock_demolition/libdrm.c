/**
 * Deadlock Demolition Lab
 * CS 241 - Spring 2019
 */

//satwiks2
//khirano2
 
#include "graph.h"
#include "dictionary.h"
#include "libdrm.h"
#include "set.h"
#include<stdio.h>
#include <pthread.h>


static graph *dependency = NULL;
static pthread_mutex_t g;

struct drm_t {
    pthread_mutex_t m;
};

int found_cycle(void *arg, dictionary *dict) {
    if (dictionary_contains(dict, arg))
        return 1;
    else {
        dictionary_set(dict, arg, NULL);
        vector *neighbours = graph_neighbors(dependency, arg);
        for (size_t i = 0; i < vector_size(neighbours); i++) {
            int cycle = found_cycle(vector_get(neighbours, i), dict);
            if (cycle)
                return 1;
        }
    }
    return 0;
}

int check_cycle(graph* arg) {
    //return 0;
    int cycle = 0;
    vector *vertices = graph_vertices(arg);
    for (size_t i = 0; i < vector_size(vertices); i++) {
        dictionary *colored = shallow_to_int_dictionary_create();
        if (found_cycle(vector_get(vertices, i), colored)) {
            cycle = 1;
            dictionary_destroy(colored);
            break;
        }
        dictionary_destroy(colored);
    }
    return cycle;
}

drm_t *drm_init() {
    drm_t *ret = malloc(sizeof(drm_t));
    if (ret == NULL)
        return NULL;

    pthread_mutex_init(&ret->m, NULL);
    pthread_mutex_lock(&g);
    if (dependency == NULL) {
        pthread_mutex_init(&g, NULL);
        dependency = shallow_graph_create();
    }
    graph_add_vertex(dependency, ret);
    pthread_mutex_unlock(&g);
    /* Your code here */
    return ret;
}

int drm_post(drm_t *drm, pthread_t *thread_id) {
    pthread_mutex_lock(&g);
    if (graph_contains_vertex(dependency, drm) &&
            graph_contains_vertex(dependency, thread_id)) {
        if (graph_adjacent(dependency, drm, thread_id)) {
            graph_remove_edge(dependency, drm, thread_id);
            pthread_mutex_unlock(&drm->m);
            pthread_mutex_unlock(&g);
            return 1;
        }
    }
    pthread_mutex_unlock(&g);
    /* Your code here */
    return 0;
}

int drm_wait(drm_t *drm, pthread_t *thread_id) {

    pthread_mutex_lock(&g);
    if (!graph_contains_vertex(dependency, thread_id))
        graph_add_vertex(dependency, thread_id);

    // IF DRM DOESN"T HAVE AN EDGE FROM IT TO SOMETHING THEN IT'S FREE
    if (graph_contains_vertex(dependency, drm)) {
        if (graph_vertex_degree(dependency, drm) == 0) {
            pthread_mutex_unlock(&g);
            pthread_mutex_lock(&(drm->m));
            pthread_mutex_lock(&g);
            graph_add_edge(dependency, drm, thread_id);
            pthread_mutex_unlock(&g);
            //drm->thread_id = *thread_id;
            return 1;
        }
        else {
            graph_add_edge(dependency, thread_id, drm);
            if (check_cycle(dependency))
                graph_remove_edge(dependency, thread_id, drm);
            else {
                pthread_mutex_unlock(&g);
                pthread_mutex_lock(&(drm->m));
                pthread_mutex_lock(&g);
                graph_remove_edge(dependency, thread_id, drm);
                graph_add_edge(dependency, drm, thread_id);
                pthread_mutex_unlock(&g);
                return 1;
            }
        }
    }
    pthread_mutex_unlock(&g);
    return 0;
}

void drm_destroy(drm_t *drm) {
    pthread_mutex_destroy(&(drm->m));
    free(drm);
    drm = NULL;
    /* Your code here */
    return;
}
