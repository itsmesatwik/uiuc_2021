/**
 * Savvy Scheduler
 * CS 241 - Spring 2019
 *
 * satwiks2
 */
#include "libpriqueue/libpriqueue.h"
#include "libscheduler.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "print_functions.h"
typedef struct _job_info {
    int id;
    double queueArrivalTime;
    double priority;
    double totalRunTimeNeeded;
    double timeSpentRunning;
    double startTime;
    double endTime;
    /* Add whatever other bookkeeping you need into this struct. */
} job_info;

priqueue_t pqueue;
scheme_t pqueue_scheme;
comparer_t comparision_func;
int jobs;
double tatal_wait;
double tatal_turnaround;
double tatal_response;

void scheduler_start_up(scheme_t s) {
    switch (s) {
    case FCFS:
        comparision_func = comparer_fcfs;
        break;
    case PRI:
        comparision_func = comparer_pri;
        break;
    case PPRI:
        comparision_func = comparer_ppri;
        break;
    case PSRTF:
        comparision_func = comparer_psrtf;
        break;
    case RR:
        comparision_func = comparer_rr;
        break;
    case SJF:
        comparision_func = comparer_sjf;
        break;
    default:
        printf("Did not recognize scheme\n");
        exit(1);
    }
    priqueue_init(&pqueue, comparision_func);
    pqueue_scheme = s;
    /* Put any set up code you may need here */
    jobs = 0;
    tatal_wait = 0.0;
    tatal_turnaround = 0.0;
    tatal_response = 0.0;
}

static int break_tie(const void *a, const void *b) {
    return comparer_fcfs(a, b);
}

int comparer_fcfs(const void *a, const void *b) {
    /* If job a arrived before job b, return 1
     * Else return -1
     */
    job_info* a_info = (job_info*)(((job*)a)->metadata);
    job_info* b_info = (job_info*)(((job*)b)->metadata);
    
    if (a_info->queueArrivalTime == b_info->queueArrivalTime) {
        return 0;
    }
    if (a_info->queueArrivalTime < b_info->queueArrivalTime) {
        return -1;
    }

    return 1;
}

int comparer_ppri(const void *a, const void *b) {
    /* Complete as is */
    return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) {

    job_info* a_info = (job_info*)(((job*)a)->metadata);
    job_info* b_info = (job_info*)(((job*)b)->metadata);

    if (a_info->priority < b_info->priority) {
        return -1;
    }
    if (a_info->priority > b_info->priority) {
        return 1;
    }

    return break_tie(a, b);
}

int comparer_psrtf(const void *a, const void *b) {
    job_info* a_info = (job_info*)(((job*)a)->metadata);
    job_info* b_info = (job_info*)(((job*)b)->metadata);

    
    if ((a_info->totalRunTimeNeeded - a_info->timeSpentRunning) > (b_info->totalRunTimeNeeded - b_info->timeSpentRunning)) {
        return 1;
    }

    if ((a_info->totalRunTimeNeeded - a_info->timeSpentRunning) < (b_info->totalRunTimeNeeded - b_info->timeSpentRunning)) {
        return -1;
    }
    return break_tie(a, b);
}

int comparer_rr(const void *a, const void *b) {
    return comparer_pri(a, b);
}

int comparer_sjf(const void *a, const void *b) {
    job_info* a_info = (job_info*)(((job*)a)->metadata);
    job_info* b_info = (job_info*)(((job*)b)->metadata);

    
    if (a_info->totalRunTimeNeeded > b_info->totalRunTimeNeeded) {
        return 1;
    }

    if (a_info->totalRunTimeNeeded < b_info->totalRunTimeNeeded) {
        return -1;
    }return break_tie(a, b);
}

/* Do not allocate stack space or initialize ctx. These will be overwritten by gtgo */
void scheduler_new_job(job *newjob, int job_number, double time,
                       scheduler_info *sched_data) {
    /**
     * Populate newjob->metadata with info found in scheduler_info.
     * Contents of newjob->metadata cannot be a pointer to sched_data.
     * Once you've set up newjob, offer it to the queue.
     */
    jobs++;
    job_info *new_job_info = malloc(sizeof(job_info));
    new_job_info->startTime = -1.0;
    new_job_info->endTime = -1.0; 
    new_job_info->id = job_number;
    new_job_info->queueArrivalTime = time;
    new_job_info->priority = sched_data->priority;
    new_job_info->totalRunTimeNeeded = sched_data->running_time;
    new_job_info->timeSpentRunning = 0.0;
    
    
    newjob->metadata = (void*)new_job_info;
    priqueue_offer(&pqueue, newjob);
}

job *scheduler_quantum_expired(job *job_evicted, double time) {
    /**
     * Called at the end of every time quantum.
     * If there is no job currently running, job_evicted will be NULL.
     * If the current scheme is not preemptive and job_evicted is not NULL, return job_evicted.
     * In all other cases, if job_evicted is not NULL,
     * place it back on the queue and return a ptr to the next job that should be run.
     * It is possible for the next job to be the same as job_evicted.
     *
     * Update the job's timeSpentRunning/startTime as needed.
     */
    if (pqueue_scheme != PPRI && pqueue_scheme != PSRTF && job_evicted && pqueue_scheme != RR) {
        job_info* info = (job_info*)job_evicted->metadata;
        
        if (info->startTime == -1.0) {
            info->startTime = time;
        }
        
        info->timeSpentRunning = (time - info->startTime);
        
        return job_evicted;
    } 
    
    if (job_evicted) {
        job_info* evicted_info = (job_info*)job_evicted->metadata;    

        evicted_info->timeSpentRunning = (time - evicted_info->startTime); 
       
        if (pqueue_scheme == RR) {
            evicted_info->priority += 100;
        }

        priqueue_offer(&pqueue, job_evicted);    

        job* next_job = (job*)priqueue_poll(&pqueue);
        
        if (next_job) {
            job_info* next_job_info = (job_info*)next_job->metadata;

            if (next_job_info->startTime == -1.0) {
                next_job_info->startTime = time;
            }
        }

        return next_job;
    }
    else {
        job* next_job = (job*)priqueue_poll(&pqueue);
        
        if (next_job) {
            job_info* next_job_info = (job_info*)next_job->metadata;

            if (next_job_info->startTime == -1.0) {
                next_job_info->startTime = time;
            }
        }

        return next_job;
    }
}

void scheduler_job_finished(job *job_done, double time) {
    /**
     * Called when a job is finished.
     * Updates global statistics and frees any buffers allocated for metadata.
     */
    job_info* finished_job = (job_info*)(job_done->metadata);
    finished_job->endTime = time;

    tatal_response += (finished_job->startTime - finished_job->queueArrivalTime);
    tatal_turnaround +=  (finished_job->endTime - finished_job->queueArrivalTime);
    tatal_wait += (finished_job->endTime - finished_job->queueArrivalTime - finished_job->totalRunTimeNeeded);
    free(job_done->metadata);
}

static void print_stats() {
    fprintf(stderr, "turnaround     %f\n", scheduler_average_turnaround_time());
    fprintf(stderr, "total_waiting  %f\n", scheduler_average_waiting_time());
    fprintf(stderr, "total_response %f\n", scheduler_average_response_time());
}

double scheduler_average_waiting_time() {
    return tatal_wait/((double) jobs);
}

double scheduler_average_turnaround_time() {
    return tatal_turnaround/((double) jobs);
}

double scheduler_average_response_time() {
    return tatal_response/((double) jobs);
}

void scheduler_show_queue() {
    // Implement this if you need it!
}

void scheduler_clean_up() {
    priqueue_destroy(&pqueue);
    print_stats();
}