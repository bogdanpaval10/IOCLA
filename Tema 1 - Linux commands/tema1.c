#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_HOME 5

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void touch (Dir* parent, char* name) {
    struct Dir* curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // verificare daca exista un director
        if(strcmp(curr_dir->name, name) == 0) {
            printf("File already exists\n");
            return;
        }
        curr_dir = curr_dir->next;
    }

    struct File* curr_file = parent->head_children_files;
    while(curr_file != NULL) { // verificare daca exista fisierul
        if(strcmp(curr_file->name, name) == 0) {
            printf("File already exists\n");
            return;
        }
        curr_file = curr_file->next;
    }

    curr_file = parent->head_children_files;
    if(curr_file != NULL)
        while(curr_file->next != NULL)
            curr_file = curr_file->next;
    struct File *new_file = malloc(sizeof(File)); // creeaza fisierul
    new_file->name = malloc(sizeof(name));
    strcpy(new_file->name, name);
    new_file->parent = parent;
    new_file->next = NULL;
    if(parent->head_children_files == NULL)
        parent->head_children_files = new_file;
    else
        curr_file->next = new_file;
}

void mkdir (Dir* parent, char* name) {
    struct Dir* curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // verificare daca exista directorul
        if(strcmp(curr_dir->name, name) == 0) {
            printf("Directory already exists\n");
            return;
        }
        curr_dir = curr_dir->next;
    }

    struct File* curr_file = parent->head_children_files;
    while(curr_file != NULL) { // verificare daca exista un fisier
        if(strcmp(curr_file->name, name) == 0) {
            printf("Directory already exists\n");
            return;
        }
        curr_file = curr_file->next;
    }

    curr_dir = parent->head_children_dirs;
    if(curr_dir != NULL)
        while(curr_dir->next != NULL)
            curr_dir = curr_dir->next;
    struct Dir *new_dir = malloc(sizeof(Dir)); // creeaza directorul
    new_dir->name = malloc(sizeof(name));
    strcpy(new_dir->name, name);
    new_dir->parent = parent;
    new_dir->next = NULL;
    new_dir->head_children_dirs = NULL;
    new_dir->head_children_files = NULL;
    if(parent->head_children_dirs == NULL)
        parent->head_children_dirs = new_dir;
    else
        curr_dir->next = new_dir;
}

void ls (Dir* parent) {
    struct Dir* curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // afiseaza numele directoarelor
        printf("%s\n", curr_dir->name);
        curr_dir = curr_dir->next;
    }

    struct File* curr_file = parent->head_children_files;
    while(curr_file != NULL) { // afiseaza numele fisierelor
        printf("%s\n", curr_file->name);
        curr_file = curr_file->next;
    }
}

void rm (Dir* parent, char* name) {
    struct File* curr_file = parent->head_children_files;
    while(curr_file != NULL) { // cauta pozitia fisierului de sters
        if(strcmp(curr_file->name, name) == 0)
            break;
        curr_file = curr_file->next;
    }

    if(curr_file == NULL)
        printf("Could not find the file\n");
    else { // strege fisierul cerut
        curr_file = parent->head_children_files;
        if(strcmp(curr_file->name, name) == 0) {
            parent->head_children_files = curr_file->next;
            free(curr_file->name);
            free(curr_file);
        } else {
            while(curr_file != NULL) {
                if(strcmp(curr_file->next->name, name) == 0)
                    break;
                curr_file = curr_file->next;
            }
            struct File* del = curr_file->next;
            curr_file->next = del->next;
            free(del->name);
            free(del);
        }
    }
}

void free_dir(Dir** home) { // functie care elibereaza recursiv memoria
    struct File *prev, *curr = (*home)->head_children_files; // unui director
    while(curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev->name); // elibereaza memoria fisierelor din director
        free(prev);
    }
    free((*home)->name); // elibereaza memoria directorului
    free(*home);
}

void rmdir (Dir* parent, char* name) {
    struct Dir* curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // cauta pozitia directorul de sters
        if(strcmp(curr_dir->name, name) == 0)
            break;
        curr_dir = curr_dir->next;
    }

    if(curr_dir == NULL)
        printf("Could not find the dir\n");
    else {
        curr_dir = parent->head_children_dirs;
        if(strcmp(curr_dir->name, name) == 0) {
            parent->head_children_dirs = curr_dir->next;
            free_dir(&(curr_dir));
        } else {
            while(curr_dir != NULL) {
                if(strcmp(curr_dir->next->name, name) == 0)
                    break;
                curr_dir = curr_dir->next;
            }
            struct Dir* del = curr_dir->next; // sterge directorul
            curr_dir->next = del->next;
            free_dir(&(del));
        }
    }
}

void cd(Dir** target, char *name) {
    if(strcmp(name, "..") == 0) { // verifica tipul comenzii cd
        if((*target)->parent == NULL)
            return;
        else
            *target = (*target)->parent;
    } else {
        struct Dir* curr_dir = (*target)->head_children_dirs;
        while(curr_dir != NULL) { // cauta pozitia directorul de inlocuit
            if(strcmp(curr_dir->name, name) == 0)
                break;
            curr_dir = curr_dir->next;
        }
        if(curr_dir == NULL)
            printf("No directories found!\n");
        else {
            curr_dir = (*target)->head_children_dirs;
            while(curr_dir != NULL) {
                if(strcmp(curr_dir->name, name) == 0)
                    break;
                curr_dir = curr_dir->next;
            }
            *target = curr_dir; // inlocuieste directorul
        }
    }
}

char* pwd (Dir* target) {
    struct Dir* curr = target;
    char *copy, *path = malloc(sizeof(char));
    while(strcmp(curr->name, "home") != 0) {
        copy = malloc(strlen(path) + 1);
        if(strcmp(curr->name, target->name) != 0)
            strcpy(copy, path); // copiaza calea veche
        path = realloc(path, strlen(path) + strlen(curr->name) + 2);
        strcpy(path, "/");
        strcat(path, curr->name);
        if(strcmp(curr->name, target->name) != 0)
            strcat(path, copy); // creeaza noua cale
        free(copy);
        curr = curr->parent;
    }
    copy = malloc(strlen(path) + 1);
    strcpy(copy, path);
    path = realloc(path, strlen(path) + strlen(curr->name) + 2);
    strcpy(path, "/");
    strcat(path, "home"); // adauga home la calea finala
    strcat(path, copy);
    free(copy);
    return path;
}

void stop (Dir* target) {
    return; 
}

void tree (Dir* target, int level) {
    struct Dir *curr, *copy_target = target;
    int okk = 0;
    if(level == 0 && target->head_children_dirs != NULL)
        target = target->head_children_dirs;
    if(level == 0 && target->head_children_dirs == NULL)
        if(target->head_children_files== NULL) 
            if(strcmp(target->name, copy_target->name) == 0)
                target = NULL;
    curr = target;

    while(curr != NULL) { // parcurgere fiecare director
        int copy = level, ok = 1;
        while(copy != 0) {
            printf(" ");
            copy--;
        }
        printf("%s\n", curr->name); // afisare nume director
        if(curr->head_children_dirs != NULL) {
            ok = 0; // apelare recursiv pentru copiii directorului
            tree(curr->head_children_dirs, level + 4);
        }
        struct File *curr_file = curr->head_children_files;
        while(curr_file != NULL) {
            int copy = level;
            if(ok == 0)
                copy += 4;
            while(copy != 0) {
                printf(" ");
                copy--;
            }
            printf("%s\n", curr_file->name); // afisare nume fisier
            okk = 1;
            curr_file = curr_file->next;
        }
        curr = curr->next;
    }
    struct File *curr_file = copy_target->head_children_files;
    if(curr_file != NULL && okk == 0) { // afisare toate fisierele din director
        while(curr_file != NULL) {
            printf("%s\n", curr_file->name);
            curr_file = curr_file->next;
        }
    }
}

void mv(Dir* parent, char *oldname, char *newname) {
    int ok_dir = 0, ok_file = 0;
    struct Dir* curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // verificare daca exista directorul vechi
        if(strcmp(curr_dir->name, oldname) == 0)
            break;
        curr_dir = curr_dir->next;
    }

    struct File* curr_file = parent->head_children_files;
    while(curr_file != NULL) { // verificare daca exista fisierul vechi
        if(strcmp(curr_file->name, oldname) == 0)
            break;
        curr_file = curr_file->next;
    }
    if(curr_dir == NULL && curr_file == NULL) {
        printf("File/Director not found\n");
        return;
    }else {
        if(curr_dir != NULL)
            ok_dir = 1;
        if(curr_file != NULL)
           ok_file = 1;
    }

    curr_dir = parent->head_children_dirs;
    while(curr_dir != NULL) { // verifica daca exista numele nou in directoare
        if(strcmp(curr_dir->name, newname) == 0)
            break;
        curr_dir = curr_dir->next;
    }
    if(curr_dir != NULL) {
        printf("File/Director already exists\n");
        return;
    }
    curr_file = parent->head_children_files;
    while(curr_file != NULL) { // verifica daca exista numele nou in fisiere
        if(strcmp(curr_file->name, newname) == 0)
            break;
        curr_file = curr_file->next;
    }
    if(curr_file != NULL) {
        printf("File/Director already exists\n");
        return;
    }

    if(ok_dir == 1) { // trebuie modificat un director
        mkdir(parent, newname);
        struct Dir *dir_del, *new_dir = parent->head_children_dirs;
        while(new_dir != NULL) { // se cauta directorul de modificat
            if(strcmp(new_dir->name, oldname) == 0)
                dir_del = new_dir;
            if(strcmp(new_dir->name, newname) == 0)
                break;
            new_dir = new_dir->next;
        }
        curr_dir = dir_del->head_children_dirs;
        while(curr_dir != NULL) { // se creeaza directorele din director vechi
            mkdir(new_dir, curr_dir->name);
            curr_dir = curr_dir->next;
        }
        curr_file = dir_del->head_children_files;
        while(curr_file != NULL) { // se creeaza fisierele din director vechi
            touch(new_dir, curr_file->name);
            curr_file= curr_file->next;
        }
        rmdir(parent, oldname);
    }
    if(ok_file == 1) { // trebuie modificat un fisier
        touch(parent, newname);
        rm(parent, oldname);
    }
}


void free_all(Dir** home) { // functie care elibereaza memoria recursiv
    struct Dir *prev, *curr = *home;
    int ok = 0;
    if((*home)->head_children_dirs != NULL)
        ok = 1;
    while(curr != NULL) {
        prev = curr;
        curr = curr->next;
        if(prev->head_children_dirs == NULL)
            free_dir(&prev); // se apeleaza functia care elibereaza memoria
        else {               // unui director
            free_all(&(prev->head_children_dirs));
        }
    }
    if(ok == 1)
        free_dir(home);
}

int main () {

    struct Dir *copy_home, *home = malloc(sizeof(Dir));
    char *command, *name, *name_new, *line, *p;
    home->name = malloc(MAX_HOME * sizeof(char));
    strcpy(home->name, "home"); // se creeaza directorul home
    home->parent = NULL;
    home->head_children_files = NULL;
    home->head_children_dirs = NULL;
    home->next = NULL;
    copy_home = home;

	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
        line = malloc(MAX_INPUT_LINE_SIZE);
		fgets(line, MAX_INPUT_LINE_SIZE, stdin); // citire linie comenzi
		p = strtok(line, " \n");
        command = malloc(strlen(p) + 1);
		strcpy(command, p);
		if (strcmp(command, "touch") == 0) { //  daca este comanda touch
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
			touch(home, name);
            free(name);
		}
		if (strcmp(command, "mkdir") == 0) { // daca este comanda mkdir
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
			mkdir(home, name);
            free(name);
		}
		if (strcmp(command, "ls") == 0) // daca este comanda ls
            ls(home);
		if (strcmp(command, "rm") == 0) { // daca este comanda rm
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
			rm(home, name);
            free(name);
		}
		if (strcmp(command, "rmdir") == 0) { // daca este comanda rmdir
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
			rmdir(home, name);
            free(name);
		}
		if (strcmp(command, "cd") == 0) { // daca este comanda cd
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
            cd((&home), name);
            free(name);
		}
		if (strcmp(command, "tree") == 0) // daca este comanda tree
		    tree(home, 0);
		if (strcmp(command, "pwd") == 0) { // daca este comanda pwd
		    char *path = pwd(home);
            printf("%s\n", path);
            free(path);
		}
        if (strcmp(command, "mv") == 0) { // daca este comanda mv
			p = strtok(NULL," \n");
            name = malloc(strlen(p) + 1);
			strcpy(name, p);
			p = strtok(NULL," \n");
            name_new = malloc(strlen(p) + 1);
			strcpy(name_new, p);
			mv(home, name, name_new);
            free(name);
            free(name_new);
		}
        if (strcmp(command, "stop") == 0) { // daca este comanda stop
			stop(home);
            free(command);
            free(line);
            break;
		}
        free(command);
        free(line);
	} while (1);

	free_all(&copy_home);

	return 0;
}
