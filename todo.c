#include "todo.h"
#include <stdlib.h>
#include <sqlite3.h>
#include <stdio.h>

int add_user(sqlite3 *db, user_t * user) {
	int rc;
	sqlite3_stmt * stmt;
	sqlite3_prepare(db, "INSERT INTO users (name, create_tm, update_tm) VALUES (%s, %s, %s);", -1, &stmt, NULL);	
	return rc;
};

int delete_user(sqlite3 *db, todo_id_t id) {
	return 0;
};

int add_task(sqlite3 *db, const user_t *user, task_t *task) {
	return 0;
};

int get_all_tasks(sqlite3 *db, const user_t *user, task_t **tasks, size_t *cnt) {
	task_t *task;
	*tasks = malloc(sizeof(tasks));
	task = *tasks;
	task->user_id = 1;
	task->id = 1;
	task->description = "Hello";
	*cnt = 1;
	return 0;
};

int delete_task(sqlite3 *db, const user_t* user, todo_id_t id) {
	return 0;
};

int update_task(sqlite3 *db, const user_t* user, todo_id_t id, task_t * task) {
	return 0;
};
