#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);

  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");

  return 0;
}


/* beargit add <filename>
 *
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");  // removes the newline
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);  // copy over each line to newindex
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}


/* beargit rm <filename>
 *
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char* filename) {
  /* COMPLETE THE REST */
  FILE *index = fopen(".beargit/.index", "r");
  FILE *newindex = fopen(".beargit/.newindex", "w");
  int found = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), index)) {
    strtok(line, "\n");
    if (!strcmp(line, filename))
      found = 1;
    else
      fprintf(newindex, "%s\n", line);
  }
  fclose(index);
  fclose(newindex);
  if (!found) {
    fprintf(stderr, "ERROR: File %s not tracked\n", filename);
    fs_rm(".beargit/.newindex");
    return 3;
  }
  fs_mv(".beargit/.newindex", ".beargit/.index") ;
  return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char* go_bears = "GO BEARS!";

int is_commit_msg_ok(const char* msg) {
  /* COMPLETE THE REST */
  int len = strlen(go_bears);
  if (sizeof(msg) > MSG_SIZE)
    return 0;

  int k = 0;
  for (int i = 0; msg[i] != '\0'; i++) {
    if (msg[i] == go_bears[k])
      k++;
    else
      k = 0;

    if (k == len)
      return 1;
  }
  return 0;
}

void next_commit_id(char* commit_id) {
  /* COMPLETE THE REST */
  const char *choices = "61c";
  char choice;
  srand(time(0));
  for (int i=0; i < 40; i++) {
    choice = choices[rand() % 3];
    commit_id[i] = choice;
  }
}

int beargit_commit(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);

  /* COMPLETE THE REST */
  char filename[FILENAME_SIZE];
  filename[0] = 0;
  char dest_dir[strlen(".beargit/") + COMMIT_ID_SIZE];
  char dest[sizeof(dest_dir) + FILENAME_SIZE];

  read_string_from_file(".beargit/.index", filename, sizeof(filename));
  if (!*filename) {
    fprintf(stderr, "ERROR: nothing tracked, so nothing to commit.\n");
    return 1;
  }
  // create commit_dir
  sprintf(dest_dir, ".beargit/%s", commit_id);
  fs_mkdir(dest_dir);

  // copy tracked files over
  FILE *index = fopen(".beargit/.index", "r");
  while (fgets(filename, sizeof(filename), index)) {
    sprintf(dest, "%s/%s", dest_dir, filename);
    filename[strlen(filename) - 1] = '\0'; // remove \n
    fs_cp(filename, dest);
  }

  // copy over metadata
  sprintf(dest, "%s/.index", dest_dir);
  fs_cp(".beargit/.index", dest);

  sprintf(dest, "%s/.prev", dest_dir);
  fs_cp(".beargit/.prev", dest);

  write_string_to_file(".beargit/.prev", commit_id);

  sprintf(dest, "%s/.msg", dest_dir);
  write_string_to_file(dest, msg);

  fclose(index);
  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {
  /* COMPLETE THE REST */
  FILE *index = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  int num_files = 0;

  fprintf(stdout, "Tracked files:\n\n");
  while(fgets(line, sizeof(line), index)) {
    fprintf(stdout, "  %s", line);
    num_files++;
  }

  char *plural = num_files > 1 || num_files == 0 ? "files" : "file";
  fprintf(stdout, "\n%d %s total\n", num_files, plural);
  return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */

int beargit_log() {
  /* COMPLETE THE REST */
  char commit_id[COMMIT_ID_SIZE];
  char msg[MSG_SIZE];
  char msg_f[sizeof(".beargit/.msg") + COMMIT_ID_SIZE];
  char prev_f[sizeof(".beargit/.prev") + COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, sizeof(commit_id));

  if (*commit_id == '0') {
    fprintf(stderr, "ERROR: There are no commits!\n");
    return 1;
  }

  while (*commit_id != '0') {
    sprintf(msg_f, ".beargit/%s/.msg", commit_id);
    read_string_from_file(msg_f, msg, sizeof(msg));
    fprintf(stdout, "\ncommit %s\n    %s\n", commit_id, msg);
    sprintf(prev_f, ".beargit/%s/.prev", commit_id);
    read_string_from_file(prev_f, commit_id, sizeof(commit_id));
  }

  return 0;
}
