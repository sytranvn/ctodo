#include "todo.h"
#include <stdio.h>

int main() {
	sqlite3* db;
	sqlite3_open("todo.db", &db);
	user_t* usr;
	task_t* task, *tasks;
	size_t cnt;
	get_all_tasks(db, usr, &tasks, &cnt);
		printf("Tasks:\n");
	for (int i = 0; i<cnt; ++i) {
		task = tasks+i;
		printf("%lld: %s\n", task->user_id, task->description);
	}
	sqlite3_close(db);
	return 0;
}
