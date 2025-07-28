#ifndef WAXC
#define WAXC

bool WVERBOSE = true;
#define printinfo       if (WVERBOSE) printf

#include "parser.c"
#include "tac.c"
#include "to_c.c"

#define TARG_C     1

void
print_help()
{
    printf(" _____                                           \n");
    printf("|||'  |                                          \n");
    printf("|''   |                                          \n");
    printf("|_WAX_| Compiler                                 \n\n");
    printf("built " __DATE__ "                               \n\n");
    printf("USAGE: waxc [options] code.wax                   \n\n");
    printf("OPTIONS:                                         \n");
    printf("--c     path/out.c     transpile to c            \n");
    printf("--json  path/out.json  syntax tree to JSON file  \n");
    printf("--tokens               print tokenization        \n");
    printf("--ast                  print abstract syntax tree\n");
    printf("--silent               don't print info          \n");
    printf("--help                 print this message        \n");
}


void
transpile(int targ, const char *input_file, const char *path, bool print_tok, bool print_ast)
{
    str_t buf = read_file_ascii(input_file);
    list_t tokens = tokenize(buf);
    map_t included;
    map_t defs;
    map_clear(&included);
    map_clear(&defs);

    if (targ == TARG_C) {
        defs_addbool(&defs, "TARGET_C", 0);
    }

    printinfo("[info] running preprocessor...\n");
    preprocess(input_file, &tokens, &included, &defs);
    if (print_tok) {
        print_tokens(&tokens);
    }

    printinfo("[info] constructing syntax tree...\n");
    expr_t *tree = syntax_tree(&tokens);
    printinfo("[info] compiling syntax tree...\n");
    map_t functable;
    map_t stttable;
    map_clear(&functable);
    map_clear(&stttable);
    compile_syntax_tree(tree, &functable, &stttable);


    if (print_ast) {
        print_stttable(&stttable);
        print_functable(&functable);
        print_syntax_tree(tree, 0);
    }

    printinfo("[info] generating target code: %s\n", path);
    str_t modname = base_name(path);
    str_t out;
    if (targ == TARG_C) {
        out = tree_to_c(modname, tree, &functable, &stttable);
    }
    write_file_ascii(path, out.data);
    freex();
}


int
main(int argc, char **argv)
{
    const char *path_c = NULL;
    const char *input_file = NULL;

    bool print_ast = false;
    bool print_tok = false;

    int i = 1;
    while (i < argc) {
        if (!strcmp(argv[i], "--c")) {
            path_c = argv[i+1];
            i+=2;
        } else if (!strcmp(argv[i], "--ast")) {
            print_ast = true;
            i++;
        } else if (!strcmp(argv[i], "--tokens")) {
            print_tok = true;
            i++;
        } else if (!strcmp(argv[i], "--silent")) {
            WVERBOSE = false;
            i++;
        } else if (!strcmp(argv[i], "--help")) {
            print_help();
            exit(0);
        } else {
            if (input_file) {
                printf("[error] cannot parse commandline argument %s.\n", argv[i]);
                printf("exiting with commandline parsing failure.\n");
                exit(1);
            } else {
                input_file = argv[i];
                i++;
            }
        }
    }

    if (input_file == NULL) {
        printf("[warn] no input file. (try '--help' for usage.)\n");
        exit(0);
    }

    if (path_c) {
        printinfo("[info] transpiling '%s' to C...\n", input_file);
        transpile(TARG_C, input_file, path_c, print_tok, print_ast);
    }

    return 0;
}

#endif
