#include "ast.h"

static void add(char *name);

static char *get_node_type(Ast *node);

static void asm_operators_print(Ast *node);

static void asm_assign_print(Ast *node);

static void asm_expression_print(Ast *node, char *name);


static void asm_loop_print(Ast *node);

static void print_tree(Ast *node, int offset);

static char *tmp_next();

static char *label_next();

static char **variable_list;
static uint64_t counter = 0;
static int label_count = 0;
static int tmp_count = 0;

Ast *add_node(Node_Type type, char *value, Ast *left, Ast *right) {
  Ast *node = malloc(sizeof(Ast));
  node->type = type;
  if (value == NULL) {
    node->value = NULL;
  } else {
    node->value = strdup(value);
  }
  node->left = left;
  node->right = right;

  if (type == Root) {
    print_tree(node, 0);
    if(node && node->right){
       asm_operators_print(node->right->left);
    }
  }
  return node;
}

Ast *add_variable(char *value, Ast *variables) {
  Ast *node = malloc(sizeof(Ast));
  node->type = Variables;
  if (value == NULL) {
    node->value = NULL;
  } else {
    node->value = strdup(value);
  }
  node->right = variables;

  add(node->value);

  return node;
}

Ast *add_assignment(char *name, Ast *expression) {
  Ast *node = malloc(sizeof(Ast));
  node->type = Operator;
  node->value = ":=";
  node->left = get_variable(name);
  node->right = expression;
  return node;
}

Ast *add_condition(Node_Type type, Ast *condition, Ast *condition_block,
                   Ast *else_block) {
  Ast *node = malloc(sizeof(Ast));
  node->type = type;
  node->value = NULL;
  node->left = condition;

  if (else_block) {
    node->right =
        add_node(Condition_Expression, NULL, condition_block, else_block);
  } else {
    node->right = add_node(Condition_Expression, NULL, condition_block, NULL);
  }
  return node;
}

static void add(char *name) {
  static uint64_t current_size = 0;
  if (current_size == 0) {
    variable_list = (malloc(sizeof(char *) * 16));
    current_size += 16;
  }

  if (current_size == counter - 1) {
    variable_list =
        (realloc(variable_list, sizeof(uint64_t) * (current_size + 16)));
    current_size += 16;
  }

  variable_list[counter] = name;
  counter++;
}

// get variable by name
Ast *get_variable(char *name) {
  for (uint64_t i = 0; i < counter; i++) {
    if (strcmp(variable_list[i], name) == 0) {
      Ast *node = malloc(sizeof(Ast));
      node->type = Identifier;
      node->value = name;
      return node;
    }
  }
}

Ast *get_constant(char *value) {
  Ast *node = malloc(sizeof(Ast));
  node->type = Identifier;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void print_tree(Ast *node, int off) {
  if (node == NULL) {
    return;
  }

  off ++;
  print_tree(node->right, off);
  for (int i = 1; i < off; i++) {
    if (off > 1 && i >= off - 1) {
        printf("");
    } else {
      printf(" ");
    }
  }

  char *node_type = get_node_type(node);

  printf("[%s]", node_type);
  if (node->value) {
    printf("(%s)\n", node->value);
  } else {
    printf("\n");
  }

  print_tree(node->left, off);
}

void asm_operators_print(Ast *node) {
  if (!node) {
    return;
  }

  switch (node->type) {
  case Operators:
    asm_operators_print(node->left);
    asm_operators_print(node->right);
    break;

  case Operator:
    asm_assign_print(node);
    break;

  case Loop:
    asm_loop_print(node);
    break;
  default:
    printf("error\n");
    break;
  }
}



static void asm_loop_print(Ast *node) {
  if (!node) {
    return;
  }

  Ast *operators = node->left;
  Ast *expression = node->right;

  char *l = label_next();
  char *tmp_name = tmp_next();
  printf("%s:\n", l);
  asm_operators_print(operators);
  asm_expression_print(expression, tmp_name);
  printf("cmp_true %s %s\n", tmp_name, l);
}

void asm_expression_print(Ast *node, char *name) {
  if (!node) {
    return;
  }

  Ast *left = node->left;
  Ast *right = node->right;

  char *tmp_name = NULL;

  if (left) {
    switch (left->type) {
    case Expression:
      tmp_name = tmp_next();
      asm_expression_print(left, tmp_name);
      break;

    case Const:
    case Identifier:
      tmp_name = left->value;
      break;

    default:
      printf("ERROR\n");
      tmp_name = "ERROR";
      break;
    }
  }
  char *tmp_name_2 = NULL;

  if (right) {
    switch (right->type) {
    case Expression:
      tmp_name_2 = tmp_next();
      asm_expression_print(right, tmp_name_2);
      break;

    case Const:
    case Identifier:
      tmp_name_2 = right->value;
      break;

    default:
      printf("ERROR\n");
      tmp_name_2 = "ERROR";
      break;
    }
  }

  if (strcmp(node->value, "-") == 0 || strcmp(node->value, "+") == 0 || strcmp(node->value, "*") == 0 || strcmp(node->value, "/") == 0 || strcmp(node->value, ">=") == 0 || strcmp(node->value, "=<") == 0 || strcmp(node->value, "=") == 0) {
    printf("%s = %s %s %s\n", name, tmp_name, node->value, tmp_name_2);
  } else if (strcmp(node->value, "MINUS") == 0 ||
             strcmp(node->value, "NOT") == 0) {
    printf("%s = %s %s\n", name, node->value, tmp_name_2);
  } else {
    printf("ERROR\n");
  }
}

void asm_assign_print(Ast *node) {
  if (!node) {
    return;
  }

  Ast *variable = node->left;
  Ast *expression = node->right;

  char *tmp_name;

  switch (expression->type) {
  case Const:
  case Identifier:
    printf("%s %s %s\n", variable->value, node->value, expression->value);
    break;

  case Expression:
    tmp_name = tmp_next();
    asm_expression_print(expression, tmp_name);
    printf("%s %s %s\n", variable->value, node->value, tmp_name);
    break;

  default:
    printf("ASSIGN ERROR\n");
    break;
  }
}

char *tmp_next() {
  char *name = malloc(16);
  snprintf(name, 16, "tmp_%d", tmp_count);
  tmp_count += 1;
  return name;
}

char *label_next() {
  char *l = malloc(16);
  snprintf(l, 16, "label_%d", label_count);
  label_count += 1;
  return l;
}

char *get_node_type(Ast *node) {
  if (node == NULL) {
    return NULL;
  }

  switch (node->type) {
  case Root:
    return "Root";

  case Variables_Declaration:
    return "Variables_Declaration";

  case Variables:
    return "Variables";

  case Identifier:
    return "Identifier";

  case Main_block:
    return "Main_block";

  case Expression:
    return "Expression";

  case Operator:
    return "Operator";

  case Operators:
    return "Operators";

  case Const:
    return "Const";

  case Condition_Expression:
    return "Condition_Expression";

  case Loop:
    return "Loop";
   }
}
