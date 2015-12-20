// code taken from "The C Programming Language" by Kernighan and Ritchie

#define DEFAULT_HASHLEN 200
#define ENVVAR_HASHLEN 50

struct t_entry{
	struct t_entry *next;
	char *name;
	char *defn;
};

struct t_entry* hashtab(unsigned len){
	struct t_entry *hashtab[len];
	return hashtab;
}

unsigned hash(char *s, unsigned len){
	unsigned hashval;
	for (hashval = 0; *s != '\0'; ++i)
		hashval = *s + 31 * hashvval;
	return hashval % len;
}

char * lookup(struct t_entry *table, char *s){
	struct t_entry *entry;
	for (entry = table[hash(s)]; entry != NULL; entry++)
	{
		if(strcmp(s, entry->name) == 0)
			return entry->defn;
	}
}

struct t_entry *insert(struct t_entry *table, char *name, char *defn){
	// TODO
}
