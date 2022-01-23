#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hm_t {
    struct entry_t **entries;
    int length;
};

struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
};

int hash(const char *const key, int length) {
    int val = 0;
    for (size_t i = 0; i < strlen(key); ++i) {
        val = 43 * val + key[i];
    }
    return (val % length);
}

/// création d'un pointeur vers une hm
hm *hm_create(unsigned int length) {
    hm *hash_map = (hm *)malloc(sizeof(hm));
    hash_map->length = length;
    hash_map->entries = (entry **)malloc(sizeof(entry *) * length);

    for (unsigned int i = 0; i < length; i++) {
        hash_map->entries[i] = NULL;
    }

    return hash_map;
}

/// destruction de la hm et libération de la mémoire
void hm_destroy(hm **hash_map) {
    for (int i = 0; i < (*hash_map)->length; i++) {
        while ((*hash_map)->entries[i] != NULL) {
            entry *tmp = (*hash_map)->entries[i];
            (*hash_map)->entries[i] = (*hash_map)->entries[i]->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
    }
    free((*hash_map)->entries);
    free(*hash_map);
    hash_map = NULL;
}

entry *create_entry(const char *const key, const char *const val) {
    entry *current = (entry *)malloc(sizeof(entry));
    current->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    current->value = (char *)malloc(sizeof(char) * (strlen(val) + 1));
    strcpy(current->key, key);
    strcpy(current->value, val);
    current->next = NULL;

    return current;
}

/// insère la paire key-value dans la hm. si key est déjà présente
/// écraser value dans la hm.
hm *hm_set(hm *hash_map, const char *const key, const char *const value) {
    int index = hash(key, hash_map->length);
    entry *current = hash_map->entries[index];

    if (current != NULL) {
        strcpy(current->value, value);
        return hash_map;
    }


    if(current != NULL){
      while (current->next != NULL)
        current = current->next;
      current->next = create_entry(key, value);
    }else{
      current = create_entry(key, value);
    }    

    if (hash_map->entries[index] == NULL) {
        hash_map->entries[index] = current;
    } else {
        entry *last_entry = hash_map->entries[index];
        
        while (last_entry->next != NULL)
            last_entry = last_entry->next;
        
        last_entry->next = current;
    }
    return hash_map;
}

/// retourne la valeur associé à la clé, key
char *hm_get(const hm *const hash_map, const char *const key) {
    int index = hash(key, hash_map->length);
    entry *to_get = hash_map->entries[index];

    if(hash_map->entries[index] == NULL)
        return NULL;

    while (to_get != NULL && strcmp(to_get->key, key) != 0)
        to_get = to_get->next;

    return to_get == NULL ? NULL : to_get->value;
}

/// retire une clé de la hm et la retourne
char *hm_rm(hm *hash_map, const char *const key) {
    int index = hash(key, hash_map->length);
    entry *to_remove = hash_map->entries[index];
    entry *previous = hash_map->entries[index];

    if(hash_map->entries[index] == NULL)
        return NULL;

    while (to_remove != NULL && strcmp(to_remove->key, key) != 0){
        previous = to_remove;
        to_remove = to_remove->next;
    }
    
    previous->next = to_remove->next;
    to_remove->next = NULL;

    return to_remove->value;
}

/// convertit la hm en chaîne de caractères
char *hm_to_str(const hm *const hash_map) {
}

/// affiche le contenu de la hm
void hm_print(const hm *const hash_map) {
    for (int i = 0; i < hash_map->length; i++) {
        entry * tmp = hash_map->entries[i];
        while (tmp != NULL) {
            printf("id: %d Key: %s ", i, tmp->key);
            printf("Value: %s\n", tmp->value);
            tmp = tmp->next;
        }
    }
}