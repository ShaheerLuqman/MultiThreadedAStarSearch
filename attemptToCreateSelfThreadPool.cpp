#include <iostream>
#include <cstring>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#define THREAD_NUM 4

using namespace std;

struct Task
{
    int a, b;
};

Task taskQueue[256];
int taskCount = 0;
bool all_tasks_submitted = false;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void executeTask(Task *task)
{
    usleep(50000);
    int result = task->a + task->b;
    cout << "The sum of " << task->a << " and " << task->b << " is " << result << endl;
}

void submitTask(Task task)
{
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void *startThread(void *args)
{
    while (true)
    {
        Task task;

        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0 && !all_tasks_submitted)
        {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        if (taskCount == 0 && all_tasks_submitted)
        {
            pthread_mutex_unlock(&mutexQueue);
            break;
        }

        task = taskQueue[0];
        for (int i = 0; i < taskCount - 1; i++)
        {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, nullptr);
    pthread_cond_init(&condQueue, nullptr);
    srand(time(nullptr));

    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&th[i], nullptr, &startThread, nullptr) != 0)
        {
            perror("Failed to create the thread");
        }
    }

    for (int i = 0; i < 100; i++)
    {
        Task t = {
            .a = rand() % 100,
            .b = rand() % 100};
        submitTask(t);
    }

    all_tasks_submitted = true;

    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], nullptr) != 0)
        {
            perror("Failed to join the thread");
        }
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    return 0;
}
