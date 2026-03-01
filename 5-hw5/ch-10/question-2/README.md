### GenerateAst.java
```
defineAst(outputDir, "Expr", Arrays.asList(
      "Assign      : Token name, Expr value", 
      "Binary      : Expr left, Token operator, Expr right",
      "Grouping    : Expr expression",
      "Literal     : Object value",
      "Logical     : Expr left, Token operator, Expr right",
      "Unary       : Token operator, Expr right",
      "Conditional : Expr condition, Expr thenBranch, Expr elseBranch",
      "Function    : List<Token> parameters, List<Stmt> body",
      "Call        : Expr callee, Token paren, List<Expr> arguments",
      "Variable    : Token name" 
    ));

defineAst(outputDir, "Stmt", Arrays.asList(
      "Block      : List<Stmt> statements", 
      "Break      : ",
      "Expression : Expr expression",
      "If         : Expr condition, Stmt thenBranch," + 
                  " Stmt elseBranch",
      "Print      : Expr expression",
      "Return     : Token keyword, Expr value",
      "Var        : Token name, Expr initializer",
      "Function   : Token name, Expr.Function function",
      "While      : Expr condition, Stmt body"
    ));

```

### LoxFunction.java
```
class LoxFunction implements LoxCallable {
  private final String name;
  private final Expr.Function declaration;
  private final Environment closure;

  LoxFunction(String name, Expr.Function declaration, Environment closure) {
    this.name = name;
    this.closure = closure;
    this.declaration = declaration;
  }

  @Override
  public int arity() {
    return declaration.parameters.size();
  }

  @Override
  public String toString() {
    if (name == null) return "<fn>";
    return "<fn " + name + ">";
  }

  //...
}
```

### Parser.java
```
private Stmt declaration() {
  try {
    if (check(FUN) && checkNext(IDENTIFIER)) {
      consume(FUN, null);
      return function("Function");
    }
    if (match(VAR)) return varDeclaration();

    return statement();
  } catch (ParseError error) {
    synchronize();
    return null;
  }
}

//...

private Expr primary() {
  if (match(FALSE)) return new Expr.Literal(false);
  if (match(TRUE)) return new Expr.Literal(true);
  if (match(NIL)) return new Expr.Literal(null);
  if (match(FUN)) return functionBody("function");

  if (match(NUMBER, STRING)) {
    return new Expr.Literal(previous().literal);
  }

  if (match(IDENTIFIER)) {
    return new Expr.Variable(previous());
  }

  if (match(LEFT_PAREN)) {
    Expr expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return new Expr.Grouping(expr);
  }

  throw error(peek(), "Expect expression.");
}

//...

private Stmt.Function function(String kind) {
  Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
  return new Stmt.Function(name, functionBody(kind));
}

private Expr.Function functionBody(String kind) {
  consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
  List<Token> parameters = new ArrayList<>();
  if (!check(RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 8) {
        error(peek(), "Can't have more than 8 parameters.");
      }

      parameters.add(consume(IDENTIFIER, "Expect parameter name."));
    } while (match(COMMA));
  }
  consume(RIGHT_PAREN, "Expect ')' after parameters.");

  consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
  List<Stmt> body = block();
  return new Expr.Function(parameters, body);
}

private boolean checkNext(TokenType tokenType) {
  if (isAtEnd()) return false;
  if (tokens.get(current + 1).type == EOF) return false;
  return tokens.get(current + 1).type == tokenType;
}
```

### Interpreter.java
```
@Override
public Void visitFunctionStmt(Stmt.Function stmt) {
    String fnName = stmt.name.lexeme;
    environment.define(fnName, new LoxFunction(fnName, stmt.function, environment));
    return null;
}

@Override
public Object visitFunctionExpr(Expr.Function expr) {
    return new LoxFunction(null, expr, environment);
}
```
