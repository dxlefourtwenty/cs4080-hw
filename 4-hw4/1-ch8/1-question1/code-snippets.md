### AstPrinter.java
```
// added for ch8-q1
@Override
public String visitVariableExpr(Expr.Variable expr) {
  return expr.name.lexeme;
}

// added for ch8-q1
@Override
public String visitAssignExpr(Expr.Assign expr) {
  return parenthesize("assign " + expr.name.lexeme, expr.value);
}

```
### RpnPrinter.java
```
@Override
public String visitVariableExpr(Expr.Variable expr) {
  return expr.name.lexeme;
}

@Override
public String visitAssignExpr(Expr.Assign expr) {
  return expr.value.accept(this) + " " + expr.name.lexeme + " =";
}

```
### GenerateAst.java
```
defineAst(outputDir, "Expr", Arrays.asList(
      "Assign      : Token name, Expr value", // added for ch8-q1
      "Binary      : Expr left, Token operator, Expr right",
      "Grouping    : Expr expression",
      "Literal     : Object value",
      "Unary       : Token operator, Expr right",
      "Conditional : Expr condition, Expr thenBranch, Expr elseBranch",
      "Call        : Expr callee, Token paren, List<Expr> arguments",
      "Variable    : Token name" // added for ch8-q1
    ));

defineAst(outputDir, "Stmt", Arrays.asList(
      "Block      : List<Stmt> statements", // added for ch8-q1
      "Expression : Expr expression",
      "Print      : Expr expression",
      "Var        : Token name, Expr initializer" // added for ch8-q1
    ));
```

### Interpeter.java
```
// added for ch8-q1
  String interpret(Expr expression) {
    try {
      Object value = evaluate(expression);
      return stringify(value);
    } catch (RuntimeError error) {
      Lox.runtimeError(error);
      return null;
    }
  }
```

### Lox.java
```
// modified for ch8-q1
  private static void runPrompt() throws IOException {
    InputStreamReader input = new InputStreamReader(System.in);
    BufferedReader reader = new BufferedReader(input);

    for (;;) {
      hadError = false;

      System.out.print("> ");
      Scanner scanner = new Scanner(reader.readLine());
      List<Token> tokens = scanner.scanTokens();

      Parser parser = new Parser(tokens);
      Object syntax = parser.parseRepl();

      // if there's a syntax error, skip
      if (hadError) continue;

      if (syntax instanceof List) {
        interpreter.interpret((List<Stmt>)syntax);
      } else if (syntax instanceof Expr) {
        String result = interpreter.interpret((Expr)syntax);
        if (result != null) {
          System.out.println("= " + result);
        }
      }
    }
```

### Parser.java
```
private boolean allowExpressions; // added for ch8-q1
private boolean foundExpression = false; // added for ch8-q1

// modified for ch8-q1
  private Stmt expressionStatement() {
    Expr expr = expression();

    if (allowExpressions && isAtEnd()) {
      foundExpression = true;
    } else {
      consume(SEMICOLON, "Expect ';' after expression.");
    }
    return new Stmt.Expression(expr);
  }

// added for ch8-q1
  Object parseRepl() {
    foundExpression = false;
    allowExpressions = true;
    List<Stmt> statements = new ArrayList<>();
    while (!isAtEnd()) {
      statements.add(declaration());

      if (foundExpression) {
        Stmt last = statements.get(statements.size() - 1);
        return ((Stmt.Expression) last).expression;
      }

      allowExpressions = false;
    }

    return statements;
  }
```
```
```
```
```
```
```
```
