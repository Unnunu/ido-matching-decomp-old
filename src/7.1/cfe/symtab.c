#include "common.h"

// .data
static int D_1001D430 = 0;
static unsigned int D_1001D434 = 0;

// .bss
static TreeNode*** B_1002BAC0;
static unsigned int* B_1002BAC4;
static unsigned int* B_1002BAC8;

int func_00419B80(TreeNode*, TreeNode*);

TreeNode* func_00417880(TreeNode* type) {
    unsigned int sp3C;
    unsigned int s0;
    int i;

    switch (TREE_CODE(type)) {
        case Struct_type:
            ((void)((STRUCT_TYPE(type).sname != EMPTY)||__assert("STRUCT_TYPE(type).sname != EMPTY", "symtab.c", 114)));
            sp3C = ID_DECL(STRUCT_TYPE(type).sname).level;
            break;
        case Enum_type:
            ((void)((ENUM_TYPE(type).ename != EMPTY)||__assert("ENUM_TYPE(type).ename != EMPTY", "symtab.c", 118)));
            sp3C = ID_DECL(ENUM_TYPE(type).ename).level;
            break;
        default:
            ((void)((FALSE)||__assert("FALSE", "symtab.c", 122)));
            break;
    }

    if (sp3C >= D_1001D434) {
        s0 = D_1001D434 + 16;
        if (D_1001D434 == 0) {
            B_1002BAC0 = Malloc(4 * 16);
            B_1002BAC4 = Malloc(4 * 16);
            B_1002BAC8 = Malloc(4 * 16);
        } else {
            B_1002BAC0 = Realloc(B_1002BAC0, 4 * s0);
            B_1002BAC4 = Realloc(B_1002BAC4, 4 * s0);
            B_1002BAC8 = Realloc(B_1002BAC4, 4 * s0); // BUG !!! TODO: figure out how to trigger it 
        }

        for (i = D_1001D434; i < s0; i++) {
            B_1002BAC0[i] = NULL;
            B_1002BAC4[i] = 0;
            B_1002BAC8[i] = 0;
        }

        D_1001D434 = s0;
    }

    if (B_1002BAC4[sp3C] >= B_1002BAC8[sp3C]) {
        s0 = B_1002BAC8[sp3C] + 32;
        if (B_1002BAC8[sp3C] == 0) {
            B_1002BAC0[sp3C] = Malloc(4 * 32);
        } else {
            B_1002BAC0[sp3C] = Realloc(B_1002BAC0[sp3C], 4 * s0);
        }
        B_1002BAC8[sp3C] = s0;
    }

    B_1002BAC0[sp3C][B_1002BAC4[sp3C]] = type;
    B_1002BAC4[sp3C]++;
    return type;
}

void func_00417BD8(int level) {
    unsigned int i;

    if (level < D_1001D434) {
        TreeNode** list = B_1002BAC0[level];
        for (i = 0; i < B_1002BAC4[level]; i++) {
            TreeNode* type = list[i];
            switch (TREE_CODE(type)) {
                case Struct_type:
                    TREE_TYPE(type) = NULL;
                    TREE_ATTRIBUTE(type) = 0;
                    STRUCT_TYPE(type).sname = NULL;
                    STRUCT_TYPE(type).members = NULL;
                    list[i] = NULL;
                    break;
                case Enum_type:
                    TREE_TYPE(type) = NULL;
                    TREE_ATTRIBUTE(type) = 0;
                    ENUM_TYPE(type).ename = NULL;
                    ENUM_TYPE(type).literals = NULL;
                    list[i] = NULL;
                    break;
                default:
                    __assert("FALSE", "symtab.c", 216);
                    break;
            }
        }

        B_1002BAC4[level] = 0;
    }
}

TreeNode* func_00417D1C(TreeNode* t) {
    int unused;
    TreeNode* copy;
    MemCtx* savedCtx;    

    savedCtx = tree_handle;
    tree_handle = general_handle;
    copy = duplicate_node(t);
    tree_handle = savedCtx;
    return copy;
}

TreeNode* func_00417D74(TreeNode* t) {
    TreeNode* copy;
    TreeNode* s1;
    TreeNode* s2;

    if (t == NULL) {
        return NULL;
    }

    if (t->id <= last_stdtree_id) {
        return t;
    }

    switch (TREE_CODE(t)) {
        case Array_type:
            copy = duplicate_node(t);
            ARRAY_TYPE(copy).index_type = func_00417D74(ARRAY_TYPE(t).index_type);
            TREE_TYPE(copy) = func_00417D74(TREE_TYPE(t));
            return copy;
        case Struct_type:
            if (!(TREE_ATTRIBUTE(t) & TYPE_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & VOLATILE_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & CONST_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & UNALIGNED_ATTRIBUTE))
            {
                if (STRUCT_TYPE(t).sname != NULL && ID_DECL(STRUCT_TYPE(t).sname).level == 2) {
                    return t;
                }
            }

            copy = duplicate_node(t);
            if (TREE_CODE(t) == Struct_type || TREE_CODE(t) == Enum_type && !(TREE_ATTRIBUTE(t) & PACKED_ATTRIBUTE)) {
                if (TREE_TYPE(t) != NULL && (TREE_ATTRIBUTE(TREE_TYPE(t)) & (CONST_ATTRIBUTE | VOLATILE_ATTRIBUTE | TYPE_ATTRIBUTE))) {
                    t = TREE_TYPE(t);
                }
            }

            if (STRUCT_TYPE(t).sname != NULL) {
                if (ID_DECL(STRUCT_TYPE(t).sname).level == 2) {
                    return copy;
                } else {
                    return func_00417880(copy);
                }
            }

            TREE_TYPE(copy) = 0;
            TREE_ATTRIBUTE(copy) = 0;
            STRUCT_TYPE(copy).sname = NULL;
            STRUCT_TYPE(copy).members = NULL;
            return copy;
        case Func_type:
            copy = duplicate_node(t);

            for (s1 = FUNC_TYPE(copy).params, s2 = NULL; s1 != NULL; s1 = TREE_LINK(s1)) {
                TreeNode* v0;
                
                v0 = func_00417D1C(s1);
                TREE_LINK(v0) = NULL;
                TREE_TYPE(v0) = func_00417D74(TREE_TYPE(s1));
                if (s2 == NULL) {
                    s2 = v0;
                } else {
                    TreeNode* v1 = s2;
                    TreeNode* v2 = s2;

                    while (v1 != NULL) {
                        v2 = v1;
                        v1 = TREE_LINK(v1);
                    }
                }
            }

            FUNC_TYPE(copy).params = s2;
            TREE_TYPE(copy) = func_00417D74(TREE_TYPE(t));
            return copy;
        case Enum_type:
            if (!(TREE_ATTRIBUTE(t) & TYPE_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & VOLATILE_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & CONST_ATTRIBUTE)
                && !(TREE_ATTRIBUTE(t) & UNALIGNED_ATTRIBUTE))
            {
                if (ENUM_TYPE(t).ename != NULL && ID_DECL(ENUM_TYPE(t).ename).level == 2) {
                    return t;
                }
            }

            copy = duplicate_node(t);
            if (TREE_CODE(t) == Struct_type || TREE_CODE(t) == Enum_type && !(TREE_ATTRIBUTE(t) & PACKED_ATTRIBUTE)) {
                if (TREE_TYPE(t) != NULL && (TREE_ATTRIBUTE(TREE_TYPE(t)) & (CONST_ATTRIBUTE | VOLATILE_ATTRIBUTE | TYPE_ATTRIBUTE))) {
                    t = TREE_TYPE(t);
                }
            }

            if (ENUM_TYPE(t).ename != NULL) {
                if (ID_DECL(ENUM_TYPE(t).ename).level == 2) {
                    return copy;
                } else {
                    return func_00417880(copy);
                }
            }

            TREE_TYPE(copy) = 0;
            TREE_ATTRIBUTE(copy) = 0;
            STRUCT_TYPE(copy).sname = NULL;
            STRUCT_TYPE(copy).members = NULL;
            return copy;
        case Pointer_type:
            copy = duplicate_node(t);
            TREE_TYPE(copy) = func_00417D74(TREE_TYPE(t));
            if (TREE_TYPE(t) != POINTER_TYPE(t).base_type) {
                POINTER_TYPE(copy).base_type = func_00417D74(POINTER_TYPE(t).base_type);
            }
            return copy;
        default:
            copy = duplicate_node(t);
            return copy;
    }
}

void func_004181C8(TreeNode* t, TreeNode* arg1) {
    TreeNode* type = TREE_TYPE(t);
    MemCtx* savedCtx = tree_handle;
    TreeNode* a2 = NULL;
    TreeNode* s7;
    TreeNode* s0;
    TreeNode* v0;

    if (TREE_ATTRIBUTE(t) & EXTERN_ATTRIBUTE) {
        ID_DECL(t).flags |= 0x100; // TODO make defines
    }

    for (s7 = arg1; s7 != NULL; s7 = ID_DECL(s7).overloads) {
        for (s0 = s7; s0 != NULL; s0 = ID_DECL(s0).hidden) {
            if ((TREE_ATTRIBUTE(s0) & EXTERN_ATTRIBUTE) && ID_DECL(s0).level == 2 ||
                (TREE_ATTRIBUTE(s0) & STATIC_ATTRIBUTE) && ID_DECL(s0).level == 2)
            {
                if (ID_DECL(s0).level != ID_DECL(t).level) {
                    if (!(TREE_ATTRIBUTE(s0) & (EXTERN_ATTRIBUTE | STATIC_ATTRIBUTE))
                        || ID_DECL(s0).context == NULL
                        || ID_DECL(s0).level != 2
                        || TREE_LOCATION(s0) != -1)
                    {
                        func_00419B80(t, s0);
                    }
                }
                a2 = s0;
            }
        }
    }

    if (a2 != NULL && ID_DECL(a2).level == 2) {
        if (TREE_ATTRIBUTE(a2) & INTR_ATTRIBUTE) {
            TREE_ATTRIBUTE(t) |= INTR_ATTRIBUTE;
        }
    } else {
        tree_handle = general_handle;
        v0 = duplicate_node(t);
        if (a2 != NULL) {
            ID_DECL(v0).blkno = ID_DECL(a2).blkno;
        }
        ID_DECL(v0).flags &= ~0x100;
        ID_DECL(v0).level = 2;
        ID_DECL(v0).st_list = NULL;
        ID_DECL(v0).overloads = NULL;
        ID_DECL(v0).hidden = NULL;
        TREE_TYPE(v0) = type;
        TREE_TYPE(v0) = func_00417D74(type);
        enter_id(v0);
        if (!options[OPTION_CPLUSPLUS] && !(options[OPTION_ANSI_MODE] & 1) && !(TREE_ATTRIBUTE(v0) & EXTERN_ATTRIBUTE)) {
            ID_DECL(v0).context = NULL;
        }
        TREE_ATTRIBUTE(t) = TREE_ATTRIBUTE(v0);
        tree_handle = savedCtx;
    }
}

void enter_id(TreeNode* id) {
    TreeNode* s1;
    unsigned int a0;

    if (TREE_CODE(id) != Id_decl) {
        __assert("TREE_CODE(id) == Id_decl", "symtab.c", 489);
    }

    if (ID_DECL(id).id == anonymous || ID_DECL(id).id == NULL) {
        mips_st(id);
    } else if (TREE_ATTRIBUTE(id) & WEAK_ATTRIBUTE) {
        mips_st(id);
    } else {
        if (debug_arr['T'] > 0) {
            fprintf(dbgout, "Entering '%s'(overload class %d, symbol %x, scope %d, overloads %x, hidden %x) from instance table\n",
                    ID_DECL(id).id->name, ID_DECL(id).oclass, ID_DECL(id).id, ID_DECL(id).level, ID_DECL(id).overloads, ID_DECL(id).hidden);
        }

        s1 = ID_DECL(id).id->constVal;
        if (s1 == NULL) {
            if (!(TREE_ATTRIBUTE(id) & EXTERN_ATTRIBUTE) &&
                !(!(TREE_ATTRIBUTE(id) & STATIC_ATTRIBUTE) && TREE_CODE(TREE_TYPE(id)) == Func_type && ID_DECL(id).init_value == NULL)
                || ID_DECL(id).level != 2 || ID_DECL(id).context == NULL)
            {
                if (ID_DECL(id).sclass != 7) { // TODO define
                    mips_st(id);
                }
            }
            ID_DECL(id).id->constVal = id;
            ID_DECL(id).id->unk_10 = 0;
        } else {
            a0 = ID_DECL(id).level;
            if (ID_DECL(s1).level > a0) {
                TreeNode* v0;
                for (v0 = ID_DECL(s1).hidden; v0 != NULL; v0 = ID_DECL(v0).hidden) {
                    s1 = v0;
                    if (ID_DECL(s1).level <= a0) {
                        break;
                    }
                }
            }

            if (ID_DECL(s1).level == a0) {
                if (func_00419B80(id, s1) && ((TREE_ATTRIBUTE(id) & EXTERN_ATTRIBUTE) || TREE_CODE(TREE_TYPE(id)) == Func_type)) {
                    if (ID_DECL(id).level >= (function_scope == NULL ? -1 : function_scope->unk_04)) {
                        func_004181C8(id, s1);
                    }
                }
            } else {
                if ((TREE_ATTRIBUTE(id) & EXTERN_ATTRIBUTE) || TREE_CODE(TREE_TYPE(id)) == Func_type) {
                    if (a0 >= (function_scope == NULL ? -1 : function_scope->unk_04)) {
                        TreeNode* t1;
                        TreeNode* v00;
                        int a00;

                        for (t1 = s1, a00 = FALSE; t1 != NULL && !a0; t1 = ID_DECL(t1).hidden) {
                            for (v00 = t1; v00 != NULL && !a0; v00 = ID_DECL(v00).overloads) {
                                if (!(TREE_ATTRIBUTE(v00) & EXTERN_ATTRIBUTE) &&
                                    !(TREE_ATTRIBUTE(v00) & STATIC_ATTRIBUTE) && TREE_CODE(TREE_TYPE(v00)) == Func_type
                                    || ID_DECL(id).sclass == 5 || ID_DECL(id).sclass == 6)
                                {
                                    a0 = TRUE;
                                    ID_DECL(id).blkno = ID_DECL(v00).blkno;
                                }
                            }
                        }
                    }
                }
                // TODO finish this shit
            }
        }
    }
}

void func_0041894C(TreeNode* id) {
    Scope* v0;

    if (ID_DECL(id).level != current_scope->unk_04) {
        if (ID_DECL(id).level == 2) {
            v0 = file_scope;
        } else if (id != NULL && TREE_CODE(id) == Id_decl && ID_DECL(id).init_value != NULL && TREE_CODE(TREE_TYPE(id)) == Func_type) {
            v0 = current_scope->unk_08;
        } else if (TREE_CODE(TREE_TYPE(id)) == Label_type) {
            v0 = function_scope;
        } else {
            __assert("FALSE", "symtab.c", 590);
        }
    }

    if (TREE_CODE(TREE_TYPE(id)) == Func_type && ID_DECL(id).level == 2) {
        if (v0->unk_18 == NULL) {
            ID_DECL(id).st_list = v0->unk_00;
            v0->unk_18 = id;
            v0->unk_00 = id;
        } else {
            ID_DECL(id).st_list = ID_DECL(v0->unk_18).st_list;
            ID_DECL(v0->unk_18).st_list = id;
        }
    } else {
        ID_DECL(id).st_list = v0->unk_00;
        v0->unk_00 = id;
    }
}