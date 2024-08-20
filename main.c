#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  sqlite3 *db;
  sqlite3_open("todo.db", &db);
  int rc;
  if ((rc = init_db(db)) != SQLITE_OK) {
    exit(rc);
  }
  user_t *user = NULL;
  char *username = getenv("USER");
  if ((rc=get_user(db, username, &user) )== SQLITE_DONE && user != NULL) {
      
      printf("Hi, %s\n", user->name);
  } else {
    user = malloc(sizeof(*user));

    user->name = username;
    user->pass = "42";
    if ((rc = add_user(db, user)) != SQLITE_OK) {
      fprintf(stderr, "Add user failed with error code %d\n", rc);
    } else {
      printf("Created user %s, id: %ld\n", user->name, user->id);
    }
  }


  sqlite3_close(db);
  free(user);
  return 0;
}
