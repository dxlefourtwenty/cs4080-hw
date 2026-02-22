### GenerateAst.java
```
defineAst(outputDir, "Stmt", Arrays.asList(
      "Block      : List<Stmt> statements", 
      "Break      : ", // added for ch9-q3
      "Expression : Expr expression",
      "If         : Expr condition, Stmt thenBranch," + 
                  " Stmt elseBranch",
      "Print      : Expr expression",
      "Var        : Token name, Expr initializer",
      "While      : Expr condition, Stmt body"
    ));

private static void defineType(
      PrintWriter writer, String baseName,
      String className, String fieldList) {
  writer.println("  static class " + className + " extends " +
      baseName + " {");

  // Constructor.
  writer.println("    " + className + "(" + fieldList + ") {");

  // Store parameters in fields.
  // modified for ch9-q3
  String[] fields;
  if (fieldList.isEmpty()) {
    fields = new String[0];
  } else {
    fields = fieldList.split(", ");
  }

  for (String field : fields) {
    String name = field.split(" ")[1];
    writer.println("      this." + name + " = " + name + ";");
  }

  writer.println("    }");

  // Visitor pattern.
  writer.println();
  writer.println("    @Override");
  writer.println("    <R> R accept(Visitor<R> visitor) {");
  writer.println("      return visitor.visit" +
      className + baseName + "(this);");
  writer.println("    }");

  // Fields.
  writer.println();
  for (String field : fields) {
    writer.println("    final " + field + ";");
  }

  writer.println("  }");
}

```
### Scanner.java
```
keywords.put("break", BREAK); // added for ch9-q3
```

### TokenType.java
```
// Keywords.
AND, BREAK, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
```

### Parser.java
```
private int loopDepth = 0;

private Stmt statement() {
  if (match(BREAK)) return breakStatement();
  if (match(FOR)) return forStatement();
  if (match(IF)) return ifStatement();
  if (match(PRINT)) return printStatement();
  if (match(LEFT_BRACE)) return new Stmt.Block(block());

  return expressionStatement();
}


private Stmt breakStatement() {
  consume(SEMICOLON, "Expect ';' after 'break'.");
  return new Stmt.Break();
}

private Stmt forStatement() {
  consume(LEFT_PAREN, "Expect '(' after 'for'.");

  Stmt initializer;
  if (match(SEMICOLON)) {
    initializer = null;
  } else if (match(VAR)) {
    initializer = varDeclaration();
  } else {
    initializer = expressionStatement();
  }

  Expr condition = null;
  if (!check(SEMICOLON)) {
    condition = expression();
  }
  consume(SEMICOLON, "Expect ';' after loop condition.");

  Expr increment = null;
  if (!check(RIGHT_PAREN)) {
    increment = expression();
  }
  consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  try {
    loopDepth++;

    Stmt body = statement();

    if (increment != null) {
      body = new Stmt.Block(Arrays.asList(
          body,
          new Stmt.Expression(increment)));
    }

    if (condition == null) condition = new Expr.Literal(true);
    body = new Stmt.While(condition, body);

    if (initializer != null) {
      body = new Stmt.Block(Arrays.asList(initializer, body));
    }

    return body;

  } finally {
    loopDepth--;
  }
}

private Stmt whileStatement() {
  consume(LEFT_PAREN, "Expect '(' after 'while'.");
  Expr condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after condition.");

  try {
    loopDepth++;

    Stmt body = statement();
    return new Stmt.While(condition, body);

  } finally {
    loopDepth--;
  }
}

private Stmt breakStatement() {
  if (loopDepth == 0) {
    error(previous(), "Must be inside a loop to use 'break'.");
  }
  consume(SEMICOLON, "Expect ';' after 'break'.");
  return new Stmt.Break();
}
```

### Interpreter.java
```
private static class BreakException extends RuntimeException {}

@Override
public Void visitBreakStmt(Stmt.Break stmt) {
  throw new BreakException();
}

@Override
public Void visitWhileStmt(Stmt.While stmt) {
  try {
    while (isTruthy(evaluate(stmt.condition))) {
      execute(stmt.body);
    }
  } catch (BreakException ex) {
    // Do nothing.
  }
  return null;
}
```

### AstPrinter.java
```
@Override
public String visitLogicalExpr(Expr.Logical expr) {
  return parenthesize(expr.operator.lexeme, expr.left, expr.right);
}
```
