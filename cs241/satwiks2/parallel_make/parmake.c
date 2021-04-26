/**
 * Parallel Make Lab
 * CS 241 - Spring 2019
 */
 

#include "graph.h"
#include "queue.h"
#include "format.h"
#include "vector.h"
#include "parser.h"
#include "parmake.h"
#include "dictionary.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>



/*
 * Using positive states to represent number of children and using negative to represent states
 */
int VISIT =  1;
#define FAILED -2
#define FINISHED -3
#define INQUEUE -4
#define EXECUTING -6


queue * task_queue;

vector * initial_targets;

graph * dependency_graph;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

int find_cycle(dictionary * visited, void* target);
void run_graph(void * target);
int set_finished(const char * target);
time_t get_time_modified(const char * filename);
void update_parents(const char * target);
void update_parents_fail(const char * target);


//DFS algorithm checks if neighbour has already been seen using a dictionary and sets cycle to 1 if it has been seen, pretty trivial

int find_cycle(dictionary * visited, void* target) {
    vector * dependencies = graph_neighbors(dependency_graph, target);
    dictionary_set(visited, target, &VISIT);

    rule_t * target_rule =  graph_get_vertex_value(dependency_graph, target);
    size_t dep = vector_size(dependencies);
    if (target_rule->state != INQUEUE) 
        target_rule->state = (int) dep;

    int cycle = 0;
    for (size_t i = 0; i < dep; i++) {
        char * child_vertex = vector_get(dependencies, i);
        if (dictionary_contains(visited, child_vertex)) {
            cycle = 1;
        }
        else {
         cycle = find_cycle(visited, child_vertex);
        }

        if (cycle) {
            target_rule->state = FAILED;
            break;
        }
    }

    if (!cycle && target_rule->state == 0 && target_rule->state != INQUEUE) {
        queue_push(task_queue, target);
        target_rule->state = INQUEUE;
    } 
    
    dictionary_remove(visited,target);
    vector_destroy(dependencies);
    return cycle;
}


int set_finished(const char * target) {
    //check if file on disk or not
    if (access(target, F_OK)) {
        return 0;
    }
    // get modified time and tally against modification time of the dependencies
    time_t mod_time = get_time_modified(target);
    int satisfied = 1;
    vector * dependencies = graph_neighbors(dependency_graph, (void *) target);
    for (size_t i = 0; i < vector_size(dependencies); i++) {
        char * child_vertex = (char *) vector_get(dependencies, i);
        if (access(child_vertex, F_OK)) {
            return 0;
        }

        time_t child_mod_time = get_time_modified(child_vertex);
        double diff = difftime(mod_time, child_mod_time);
        if (diff < 0) {
            return 0;
        }
    }
    vector_destroy(dependencies);

    return satisfied;
}



time_t get_time_modified(const char * filename) {
    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_mtime;
}


//Child finishes and updates the parents by removing itself as not a dependency anymnore
void update_parents(const char * target) {
    vector * parents = graph_antineighbors(dependency_graph, (void *)target);
    for (size_t i = 0; i < vector_size(parents); i++) {
        char * parent = vector_get(parents, i);
        rule_t * parent_r = (rule_t *)graph_get_vertex_value(dependency_graph, (void *) parent);
        //remove a dependency
        parent_r->state--;

        if (parent_r->state == 0) {
            queue_push(task_queue, parent);
            parent_r->state = INQUEUE;
        }
    }
    vector_destroy(parents);
}

//Mark parents as failed when child failed to immediately stop
void update_parents_fail(const char * target) {
    vector * parents = graph_antineighbors(dependency_graph, (void *) target);
    //antineghbours gives parents and iterate through it changing state to FAILED
    for (size_t i = 0; i < vector_size(parents); i++) {
        char * parent_t = vector_get(parents, i);
        rule_t * parent_r = (rule_t *)graph_get_vertex_value(dependency_graph, (void *) parent_t);
        parent_r->state = FAILED;
        update_parents_fail(parent_t);
    }
    vector_destroy(parents);
}


// Run the vertices of graphs, if a child fails, other children must still be executed

void run_graph(void * target) {
    rule_t * rule = (rule_t *) graph_get_vertex_value(dependency_graph,target);
    pthread_mutex_lock(&m1);
    rule->state = EXECUTING;
    pthread_mutex_unlock(&m1);
    int failed = 0;
    //check if the target or its dependencies are files on disk and if the modified time is satisfied, if it is set as finished
    if (set_finished(target)) {
        pthread_mutex_lock(&m1);
        rule->state = FINISHED;
        update_parents(target);
        pthread_mutex_unlock(&m1);
        return;
    }
    // otherwise run the commands that are yet to be run and change the state accordingly
    vector * cmds = rule->commands;
    for (size_t i = 0; i < vector_size(cmds); i++) {
        char * cmd = (char *) vector_get(cmds, i);
        int executed = system(cmd);
        if (executed != 0) {
            failed = 1;
            pthread_mutex_lock(&m1);
            rule->state = FAILED;
            pthread_mutex_unlock(&m1);
            break;
        }
    }

    //update number of dependencies only if success otherwise set to failure
    pthread_mutex_lock(&m1);
    if (failed == 0) {
        rule->state = FINISHED;
        update_parents(target);
    } else {
        update_parents_fail(target);
    }

    pthread_mutex_unlock(&m1);
    return;
}


// check if all the targets have been satisfied or if they have failed and finish if they have
int targets_finished() {
    pthread_mutex_lock(&m1);
    for (size_t i = 0; i < vector_size(initial_targets); i++) {
        char * target = vector_get(initial_targets, i);
        rule_t * rule = graph_get_vertex_value(dependency_graph, target);
        if(rule->state != FINISHED && rule->state != FAILED) {
            pthread_mutex_unlock(&m1);
            return 0;
        }
    }
 
    queue_push(task_queue, NULL);  
    pthread_mutex_unlock(&m1);
    return 1;
}


void * para_task(void * graph) {
    while(!targets_finished()) {
        char * target = queue_pull(task_queue);
        // Finished
        if (target == NULL) {
            break;
        }
        run_graph(target);
    }
    queue_push(task_queue, NULL);
    return graph;
}


int parmake(char *makefile, size_t num_threads, char **targets) {
    dependency_graph = parser_parse_makefile(makefile, targets);
    initial_targets = graph_neighbors(dependency_graph, "");
    task_queue = queue_create(-1);

    //check for cycles
    char * target;
    for (size_t i = 0; i < vector_size(initial_targets); i++) {
        dictionary * colored = int_to_int_dictionary_create();
        target = vector_get(initial_targets, i);
        if (find_cycle(colored, target)) {
            vector_erase(initial_targets, i--);
            print_cycle_failure(target);
        }  
        dictionary_destroy(colored);
    }

    //run the threads
    pthread_t tids[num_threads];
    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&tids[i], NULL, &para_task, dependency_graph);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(tids[i], NULL);
    }
    queue_destroy(task_queue);
    vector_destroy(initial_targets);
    graph_destroy(dependency_graph);
    return 0;
}
