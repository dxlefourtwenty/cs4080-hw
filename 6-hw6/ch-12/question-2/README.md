### Resolver.java
```
private void resolveFunction(Expr.Function function, FunctionType type) {
  FunctionType enclosingFunction = currentFunction;
  currentFunction = type;

  beginScope();

  // check for null (getter has no parameters)
  if (function.params != null) {
    for (Token param : function.parameters) {
      declare(param);
      define(param);
    }
  }
  resolve(function.body);
  endScope();
  currentFunction = enclosingFunction;
}

```

### Parser.java
```
private Stmt.Function function(String kind) {
  Token name = consume(IDENTIFIER, "Expect " + kind + " name.");

  // Allow omitting parameter list for method getters
  List<Token> parameters = null;

  // Only require parens if NOT a method, OR if parens are actually present
  if (!kind.equals("method") || check(LEFT_PAREN))  {
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    parameters = new ArrayList<>();
    if (!check(RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }

        parameters.add(consume(IDENTIFIER, "Expect parameter name."));
      } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
  }

  consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
  List<Stmt> body = block();
  return new Stmt.Function(name, new Expr.Function(paremeters, body));
}
```

### LoxFunction.java
```
 @Override
public Object call(Interpreter interpreter,
                    List<Object> arguments) {
  Environment environment = new Environment(closure);

  // check for null (getters have no parameters)
  if (declaration.parameters != null) {
    for (int i = 0; i < declaration.parameters.size(); i++) {
      environment.define(declaration.parameters.get(i).lexeme,
          arguments.get(i));
    }
  }

  try {
    interpreter.executeBlock(declaration.body, environment);
  } catch (Return returnValue) {
    if (isInitializer) return closure.getAt(0, "this");

    return returnValue.value;
  }

  if (isInitializer) return closure.getAt(0, "this");
  return null;
}

public boolean isGetter() {
  return declaration.parameters == null;
}

```

### Interpreter.java
```
@Override
public Object visitGetExpr(Expr.Get expr) {
  Object object = evaluate(expr.object);
  if (object instanceof LoxInstance) {
    Object result = ((LoxInstance)object).get(expr.name);

    // If result is a getter, execute it immediately
    if (result instanceof LoxFunction &&
        ((LoxFunction)result).isGetter()) {
      result = ((LoxFunction)result).call(this, null);
    }

    return result;
  }

  throw new RuntimeError(expr.name,
      "Only instances have properties.");
}
```
