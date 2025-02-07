#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct ParseSymbol {
    LinkedListEntry link;
    int unk_04;
    int unk_08;
} ParseSymbol;

struct TreeNode;

typedef struct Symbol {
    struct TreeNode* constVal; // TODO come up with better name
    ParseSymbol* unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int namelen;
    char name[1];
} Symbol;

void enter_id(struct TreeNode* id);

#endif
